// library.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

using namespace std;

#define LOGIN_TIMEOUT 30
#define MAXBUFLEN 255

SQLHENV henv = NULL; //�������
SQLHDBC conn = NULL; //���Ӿ��
SQLHSTMT hstmt = NULL; //�����
SQLRETURN retcode;

void DBTest();
void check_Book();
void CheckBookType(int type);
void borrow_Book();
void return_Book();
void check_Record(char *reader);
void add_Book();
void AddOneBook(char*id, char*title, char*catalog, char*author, char*press, int year, double price, int total);
void Reader_Manag();
void delete_Reader();
void add_Reader();
void AddOneReader(char*id, char*name, char*department, char*type);
void change_Reader(char*id);

int main()
{
	DBTest();
	return 0;
}
void DBTest()
{
	int choice = 0;//����û�ѡ��
	/*******************************��ʼ����**********************************/
	//���价�����
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	//���û�������
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if (!SQL_SUCCEEDED(retcode))
	{
		cout << "AllocEnvHandle error!" << endl;
		system("pause");
	}
	//�������Ӿ��
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &conn);
	if (!SQL_SUCCEEDED(retcode))
	{
		cout << "AllocDbcHandle error!" << endl;
		system("pause");
	}
	//�������ݿ�
	retcode = SQLConnect(conn, (SQLCHAR*)"mysql64", SQL_NTS, (SQLCHAR*)"root", SQL_NTS, (SQLCHAR*)"018311", SQL_NTS);
	if (!SQL_SUCCEEDED(retcode))
	{
		cout << "SQL_Connect error!" << endl;
		system("pause");
	}
	/*******************************���岿��**********************************/
	cout << "------------------------------------------------------------" << endl;
	cout << '\t' << '\t' << '\t' << "ͼ�����ϵͳ" << endl;

	while (1) {
		//����Ŀ¼
		cout << "------------------------------------------------------------" << endl;
		cout << '\t' << "[1].ͼ���ѯ" << '\t' << "[2].����" << '\t' << "[3].����" << endl;
		cout << '\t' << "[4].ͼ�����" << '\t' << "[5].����֤����" << '\t' << "[0].�˳�ϵͳ" << endl;
		cout << "------------------------------------------------------------" << endl;
		cout << "��������Ҫ�ķ�����: ";
		cin >> choice;

		switch (choice)
		{
		case 0:
			//�ͷ������
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
				cout << "free hstmt error!" << endl;
			//�Ͽ����ݿ�����
			retcode = SQLDisconnect(conn);
			if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
				cout << "disconnected error!" << endl;
			//�ͷ����Ӿ��
			retcode = SQLFreeHandle(SQL_HANDLE_DBC, conn);
			if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
				cout << "free hdbc error!" << endl;
			//�ͷŻ���������
			retcode = SQLFreeHandle(SQL_HANDLE_ENV, henv);
			if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
				cout << "free henv error!" << endl;

			cout << "------------------------------------------------------------" << endl;
			cout << '\t' << '\t' << "-=  ��лʹ�ã��ټ��� =-" << endl;
			cout << "------------------------------------------------------------" << endl;
			system("pause");
			exit(0);
		case 1:
			check_Book();
			break;
		case 2:
			borrow_Book();
			break;
		case 3:
			return_Book();
			break;
		case 4:
			add_Book();
			break;
		case 5:
			Reader_Manag();
			break;
		default:
			cout << "�����Ŵ���" << endl;
		}
	}
	/*******************************��������**********************************/
	MessageBox(NULL, TEXT("ִ�гɹ�"), TEXT("����"), MB_OK);
}
void check_Book()
{
	int choice = 0;//����û�ѡ��
	int type = 0;//��Ų�ѯ����

	//���ͼ����Ϣ�ı���
	SQLCHAR book_id[20] = { 0 }, title[30] = { 0 }, catalog[30] = { 0 }, author[30] = { 0 }, press[30] = { 0 };
	SQLINTEGER publish_year = 0, total_number = 0, stock = 0;
	SQLDOUBLE price = 0.0;
	SQLLEN length;

	while (1) {
		cout << "[1].��ѯȫ��" << '\t' << "[2].��������ѯ" <<  '\t' << "0.�˳�" << endl;
		cout << "��������Ҫ�ķ����ţ�";
		cin >> choice;
		switch (choice)
		{
		case 0:
			return;
		case 1:
			//����ִ�������
			retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
			//ִ��SQL���
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)"select * from book", SQL_NTS);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				//������
				SQLBindCol(hstmt, 1, SQL_C_CHAR, book_id, sizeof(book_id), &length);
				SQLBindCol(hstmt, 2, SQL_C_CHAR, title, sizeof(title), &length);
				SQLBindCol(hstmt, 3, SQL_C_CHAR, catalog, sizeof(catalog), &length);
				SQLBindCol(hstmt, 4, SQL_C_CHAR, author, sizeof(author), &length);
				SQLBindCol(hstmt, 5, SQL_C_CHAR, press, sizeof(press), &length);
				SQLBindCol(hstmt, 6, SQL_C_SHORT, &publish_year, 0, &length);
				SQLBindCol(hstmt, 7, SQL_C_DOUBLE, &price, 0, &length);
				SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_number, 0, &length);
				SQLBindCol(hstmt, 9, SQL_C_SHORT, &stock, 0, &length);

				int count = 0;
				cout << "--------------------------------------------------------------------------------------------------------------" << endl;
				while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
				{
					if (count == 0)
					{
						cout << "��ѯ������£�" << endl;
						cout << "���\t\t����\t\t���\t\t����\t������\t\t���\t�۸�\t�ܲ�����\t���" << endl;
						cout << "--------------------------------------------------------------------------------------------------------------" << endl;
					}
					cout << book_id << '\t' << title << '\t' << catalog << '\t' << author << '\t' << press << '\t' << publish_year << '\t';
					printf("%.2f", price);
					cout << '\t' << total_number << '\t' << '\t' << stock << endl;
					count++;
				}
				if (count == 0) cout << "������ͼ�飡" << endl;
				cout << "--------------------------------------------------------------------------------------------------------------" << endl;
			}
			//�ͷ������
			retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
				cout << "free hstmt error!" << endl;
			break;
		case 2:
			cout << "[0].���" << '\t' << "[1].����" << '\t' << "[2].���" << '\t' << "[3].����" << '\t' << "[4].������" << '\t' << "[5].���" << '\t' << "[6].�۸�" << endl;
			cout << "��ѡ���ѯ������";
			cin >> type;
			if (type >= 0 && type <= 6)
				CheckBookType(type);
			else
				cout << "��ѯ���ͱ�Ŵ���" << endl;
			break;
		default:
			cout << "�����Ŵ���" << endl;

		}
	}
}
void CheckBookType(int type)
{
	//���ͼ����Ϣ�ı���
	SQLCHAR book_id[20] = { 0 }, title[30] = { 0 }, catalog[30] = { 0 }, author[30] = { 0 }, press[30] = { 0 };
	SQLINTEGER publish_year = 0, total_number = 0, stock = 0;
	SQLDOUBLE price = 0.00;
	SQLLEN length;

	char query[200] = "select * from book where ";//��ѯ���
	string column;
	switch (type)
	{
	case 0: column = "book_id"; break;
	case 1: column = "title"; break;
	case 2: column = "catalog"; break;
	case 3: column = "author"; break;
	case 4: column = "press"; break;
	case 5: column = "publish_year"; break;
	case 6: column = "price"; break;
	}
	if (type == 5 || type == 6)//range
	{
		char lower[20], upper[20];
		cout << "�����ѯ��Χ(�½� �Ͻ�): ";
		cin >> lower >> upper;
		strcat_s(query, column.c_str());
		strcat_s(query, " >= ");
		strcat_s(query, lower);
		strcat_s(query, " and ");
		strcat_s(query, column.c_str());
		strcat_s(query, " <= ");
		strcat_s(query, upper);
	}
	else//exact
	{
		char check[30];
		cout << "�����ѯֵ: ";
		cin >> check;
		strcat_s(query, column.c_str());
		strcat_s(query, " = '");
		strcat_s(query, check);
		strcat_s(query, "'");
	}

	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
	//ִ��SQL���
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		//������
		SQLBindCol(hstmt, 1, SQL_C_CHAR, book_id, sizeof(book_id), &length);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, title, sizeof(title), &length);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, catalog, sizeof(catalog), &length);
		SQLBindCol(hstmt, 4, SQL_C_CHAR, author, sizeof(author), &length);
		SQLBindCol(hstmt, 5, SQL_C_CHAR, press, sizeof(press), &length);
		SQLBindCol(hstmt, 6, SQL_C_SHORT, &publish_year, 0, &length);
		SQLBindCol(hstmt, 7, SQL_C_DOUBLE, &price, 0.00, &length);
		SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_number, 0, &length);
		SQLBindCol(hstmt, 9, SQL_C_SHORT, &stock, 0, &length);

		int count = 0;
		cout << "--------------------------------------------------------------------------------------------------------------" << endl;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			if (count == 0)
			{
				cout << "��ѯ������£�" << endl;
				cout << "���\t\t����\t\t���\t\t����\t������\t\t���\t�۸�\t�ܲ�����\t���" << endl;
				cout << "--------------------------------------------------------------------------------------------------------------" << endl;
			}
			cout << book_id << '\t' << title << '\t' << catalog << '\t' << author << '\t' << press << '\t' << publish_year << '\t';
			printf("%.2f", price);
			cout << '\t' << total_number << '\t' << '\t' << stock << endl;
			count++;
		}
		if (count == 0) cout << "�޷��������Ľ��" << endl;
		cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	}
	//�ͷ������
	retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
		cout << "free hstmt error!" << endl;
	return;
}
void borrow_Book()
{
	char book[20], reader[20];
	cout << "�����뿨��: ";
	cin >> reader;
	
	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);

	/************�Ƿ���ڸÿ���****************/
	char query[200] = "select * from reader where reader_id = '";//��ѯ���
	strcat_s(query, reader);
	strcat_s(query, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		int count = 0;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
			count++;
		if (!count)
		{
			cout << "�ÿ��Ų����ڣ�" << endl;
			return;
		}
	}

	/************��ѯ�����¼****************/
	check_Record(reader);

	cout << "��������Ҫ���ĵ�ͼ����ţ�";
	cin >> book;

	/************�Ƿ���ڸ�����Լ����������Ƿ�Ϊ��****************/
	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
	SQLINTEGER stock = 0;
	SQLLEN length;
	int new_stock_num;
	int zero = 0;
	char query1[200] = "select * from book where book_id = '";//��ѯ���
	strcat_s(query1, book);
	strcat_s(query1, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query1, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		//������
		SQLBindCol(hstmt, 9, SQL_C_SHORT, &stock, 0, &length);
		int count2 = 0;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			if (stock == 0)
			{
				cout << "��������Ϊ�㣡" << endl;
				zero = 1;
			}
			else
				new_stock_num = stock - 1;
			count2++;
		}
			
		if (!count2)
		{
			cout << "����Ų����ڣ�" << endl;
			return;
		}
	}
	if (zero)//����Ϊ��
	{
		SQLCHAR min_due_date[20] = { 0 };
		char query0[200] = "SELECT min(due_date) FROM record WHERE book_id = '";
		strcat_s(query0, book);
		strcat_s(query0, "'");
		retcode = SQLExecDirect(hstmt, (SQLCHAR*)query0, SQL_NTS);
		
		SQLBindCol(hstmt, 1, SQL_C_CHAR, min_due_date, sizeof(min_due_date), &length);
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			cout << "���������黹ʱ��Ϊ��" << min_due_date << endl;
		}
		return;
	}
	/************����������һ****************/
	char new_stock[10];
	_itoa_s(new_stock_num, new_stock, 10, 10);
	char query2[200] = "UPDATE book SET stock = ";
	strcat_s(query2, new_stock);
	strcat_s(query2, " where book_id = '");
	strcat_s(query2, book);
	strcat_s(query2, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query2, SQL_NTS);

	/************�����µĽ����¼****************/
	char query3[200] = "insert into record(book_id,reader_id,borrow_date,due_date,return_date,isreturned) values('";
	strcat_s(query3, book);
	strcat_s(query3, "','");
	strcat_s(query3, reader);
	strcat_s(query3, "',NOW(),date_add(NOW(), interval 1 MONTH),NULL,'N')");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query3, SQL_NTS);
	cout << "����ɹ�������һ�����ڹ黹��" << endl;

	//�ͷ������
	retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
		cout << "free hstmt error!" << endl;
}
void return_Book()
{
	char book[20], reader[20];
	cout << "�����뿨��: ";
	cin >> reader;

	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);

	/************�Ƿ���ڸÿ���****************/
	char query[200] = "select * from reader where reader_id = '";//��ѯ���
	strcat_s(query, reader);
	strcat_s(query, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		int count = 0;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
			count++;
		if (!count)
		{
			cout << "�ÿ��Ų����ڣ�" << endl;
			return;
		}
	}

	/************��ѯ�����¼****************/
	check_Record(reader);

	cout << "��������Ҫ�黹��ͼ����ţ�";
	cin >> book;

	/************�Ƿ���ڸ����****************/
	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
	SQLINTEGER stock = 0;
	SQLLEN length;
	int new_stock_num;
	char query1[200] = "select * from book where book_id = '";//��ѯ���
	strcat_s(query1, book);
	strcat_s(query1, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query1, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		//������
		SQLBindCol(hstmt, 9, SQL_C_SHORT, &stock, 0, &length);
		int count2 = 0;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			new_stock_num = stock + 1;
			count2++;
		}
		if (!count2)
		{
			cout << "����Ų����ڣ�" << endl;
			return;
		}
	}

	/************�Ƿ���ڸý����¼�Լ��Ƿ��ѻ�****************/
	int r_flag = 0;
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
	SQLCHAR isreturned[2] = { 0 };
	char query0[200] = "select * from record where book_id = '";//��ѯ���
	strcat_s(query0, book);
	strcat_s(query0, "' and reader_id = '");
	strcat_s(query0, reader);
	strcat_s(query0, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query0, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		//������
		SQLBindCol(hstmt, 7, SQL_C_CHAR, isreturned, sizeof(isreturned), &length);
		int count0 = 0;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			count0++;
			if (!strcmp((char*)isreturned, "N")) r_flag = 1;//����δ����¼
		}
		if (!count0)
		{
			cout << "�ý����¼�����ڣ�" << endl;
			return;
		}
		if (!r_flag)
		{
			cout << "�����ѹ黹��" << endl;
			return;
		}
	}

	/************����������һ****************/
	char new_stock[10];
	_itoa_s(new_stock_num, new_stock, 10, 10);
	char query2[200] = "UPDATE book SET stock = ";
	strcat_s(query2, new_stock);
	strcat_s(query2, " where book_id = '");
	strcat_s(query2, book);
	strcat_s(query2, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query2, SQL_NTS);

	/************�ڽ����¼�����ӻ���ʱ�����ѻ����****************/
	char query3[200] = "UPDATE record SET return_date = now() WHERE book_id = '";
	strcat_s(query3, book);
	strcat_s(query3, "' and reader_id = '");
	strcat_s(query3, reader);
	strcat_s(query3, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query3, SQL_NTS);

	char query4[200] = "UPDATE record SET isreturned = 'Y' WHERE book_id = '";
	strcat_s(query4, book);
	strcat_s(query4, "' and reader_id = '");
	strcat_s(query4, reader);
	strcat_s(query4, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query4, SQL_NTS);
	cout << "����ɹ���" << endl;

	//�ͷ������
	retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
		cout << "free hstmt error!" << endl;
}
void check_Record(char *reader)
{

	SQLCHAR book_id[20] = { 0 }, isreturned[2] = { 0 };
	SQLINTEGER record_id = 0;
	SQLCHAR borrow_date[20] = { 0 }, due_date[20] = { 0 }, return_date[20] = { 0 };
	SQLLEN length;

	char query[200] = "select * from record where reader_id = '";
	strcat_s(query, reader);
	strcat_s(query, "'");
	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
	//ִ��SQL���
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		//������
		SQLBindCol(hstmt, 1, SQL_C_LONG, &record_id, 0, &length);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, book_id, sizeof(book_id), &length);
		SQLBindCol(hstmt, 4, SQL_C_CHAR, borrow_date, sizeof(borrow_date), &length);
		SQLBindCol(hstmt, 5, SQL_C_CHAR, due_date, sizeof(due_date), &length);
		SQLBindCol(hstmt, 6, SQL_C_CHAR, return_date, sizeof(return_date), &length);
		SQLBindCol(hstmt, 7, SQL_C_CHAR, isreturned, sizeof(isreturned), &length);

		int count = 0;
		cout << "--------------------------------------------------------------------------------------------------------------" << endl;
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			if (count == 0)
			{
				cout << "�ö��߽����¼���£�" << endl;
				cout << "��¼��\t���\t\t���ʱ��\t\tӦ��ʱ��\t\t�黹ʱ��\t\t�Ƿ��ѻ�" << endl;
				
			}
			cout << record_id << '\t' << book_id << '\t' << borrow_date << '\t' << due_date << '\t';
			if (return_date == NULL || !strcmp((char*)return_date,"")) cout << '\t' << '\t';
			else cout << return_date;
			cout << '\t' << isreturned << endl;
			count++;
			memset(return_date, 0, 20 * sizeof(SQLCHAR));
		}
		if (count == 0) cout << "�ö������޽����¼��" << endl;
		else cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	}
}
void add_Book()
{
	while (1)
	{
		cout << "[1].�������" << '\t' << "[2].�������" << '\t' << "[0].�˳�" << endl;
		int add_op;
		cin >> add_op;
		if (!add_op)
			return;
		if (add_op == 1)//������⣬��������
		{
			char id[20], title[30], catalog[30], author[30], press[30];
			int year, total;
			double price;
			cout << "����������š�������������ߡ������硢��ݡ��۸�����:" << endl;
			cin >> id >> title >> catalog >> author >> press >> year >> price >> total;
			AddOneBook(id, title, catalog, author, press, year, price, total);
		}
		else if(add_op == 2)//������⣬addbook.txt�ļ�����
		{
			string filename;
			cout << "����txt�ļ���: ";
			cin >> filename;
			ifstream fin(filename.c_str());
			if (!fin)
			{
				cout << "���ļ�ʧ�ܣ�" << endl;
				return;
			}
			while (!fin.eof())
			{
				char id[20], title[30], catalog[30], author[30], press[30];
				int year, total;
				double price;
				fin >> id >> title >> catalog >> author >> press >> year >> price >> total;
				AddOneBook(id, title, catalog, author, press, year, price, total);
			}
			fin.close();
		}
		else
			cout << "�����Ŵ���" << endl;
	}
	
}
void AddOneBook(char*id, char*title, char*catalog, char*author, char*press, int year, double price, int total)
{
	int flag = 0;
	int newtitle = 0, newcatalog = 0, newauthor = 0, newpress = 0, newyear = 0, newprice = 0;
	int new_total_num, new_stock_num;
	//���ͼ����Ϣ�ı���
	SQLCHAR old_title[30] = { 0 }, old_catalog[30] = { 0 }, old_author[30] = { 0 }, old_press[30] = { 0 };
	SQLINTEGER old_publish_year = 0, old_total_number = 0, old_stock = 0;
	SQLDOUBLE old_price = 0.00;
	SQLLEN length;

	//����ִ�������
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);

	/************�����Ƿ����и���****************/
	char query[200] = "SELECT * FROM book where book_id = '";//��ѯ���
	strcat_s(query, id);
	strcat_s(query, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		//������
		SQLBindCol(hstmt, 2, SQL_C_CHAR, old_title, sizeof(old_title), &length);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, old_catalog, sizeof(old_catalog), &length);
		SQLBindCol(hstmt, 4, SQL_C_CHAR, old_author, sizeof(old_author), &length);
		SQLBindCol(hstmt, 5, SQL_C_CHAR, old_press, sizeof(old_press), &length);
		SQLBindCol(hstmt, 6, SQL_C_SHORT, &old_publish_year, 0, &length);
		SQLBindCol(hstmt, 7, SQL_C_DOUBLE, &old_price, 0.00, &length);
		SQLBindCol(hstmt, 8, SQL_C_SHORT, &old_total_number, 0, &length);
		SQLBindCol(hstmt, 9, SQL_C_SHORT, &old_stock, 0, &length);

		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			flag = 1;//���и���

			cout << "�Ѵ��ڸ��顣";
			if (strcmp(title, (char*)old_title) != 0)
				newtitle = 1;
			if (strcmp(catalog, (char*)old_catalog) != 0)
				newcatalog = 1;
			if (strcmp(author, (char*)old_author) != 0)
				newauthor = 1;
			if (strcmp(press, (char*)old_press) != 0)
				newpress = 1;
			if (year != old_publish_year)
				newyear = 1;
			if (price != old_price)
				newprice = 1;
			if (!newtitle && !newcatalog && !newauthor && !newpress && !newyear && !newprice)
			{
				new_total_num = old_total_number + total;
				new_stock_num = old_stock + total;
			}
		}
	}

	/************���и���****************/
	if (flag)
	{
		/************������������****************/
		if (!newtitle && !newcatalog && !newauthor && !newpress && !newyear && !newprice)
		{
			char new_total[10];
			_itoa_s(new_total_num, new_total, 10, 10);
			char query1[200] = "UPDATE book SET total_number = ";
			strcat_s(query1, new_total);
			strcat_s(query1, " where book_id = '");
			strcat_s(query1, id);
			strcat_s(query1, "'");
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)query1, SQL_NTS);

			char new_stock[10];
			_itoa_s(new_stock_num, new_stock, 10, 10);
			char query2[200] = "UPDATE book SET stock = ";
			strcat_s(query2, new_stock);
			strcat_s(query2, " where book_id = '");
			strcat_s(query2, id);
			strcat_s(query2, "'");
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)query2, SQL_NTS);

			cout << "���Ӳ������������ɹ���" << endl;
			// �ͷ������
			retcode = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
				cout << "free hstmt error!" << endl;
			return;
		}
		/************�޸�ͼ����Ϣ****************/
		else
		{
			if (newtitle)
			{
				char query3[200] = "UPDATE book SET title ='";
				strcat_s(query3, title);
				strcat_s(query3, "' where book_id = '");
				strcat_s(query3, id);
				strcat_s(query3, "'");
				retcode = SQLExecDirect(hstmt, (SQLCHAR*)query3, SQL_NTS);
			}
			if (newcatalog)
			{
				char query4[200] = "UPDATE book SET catalog ='";
				strcat_s(query4, catalog);
				strcat_s(query4, "' where book_id = '");
				strcat_s(query4, id);
				strcat_s(query4, "'");
				retcode = SQLExecDirect(hstmt, (SQLCHAR*)query4, SQL_NTS);
			}
			if (newauthor)
			{
				char query5[200] = "UPDATE book SET author ='";
				strcat_s(query5, author);
				strcat_s(query5, "' where book_id = '");
				strcat_s(query5, id);
				strcat_s(query5, "'");
				retcode = SQLExecDirect(hstmt, (SQLCHAR*)query5, SQL_NTS);
			}
			if (newpress)
			{
				char query6[200] = "UPDATE book SET press ='";
				strcat_s(query6, press);
				strcat_s(query6, "' where book_id = '");
				strcat_s(query6, id);
				strcat_s(query6, "'");
				retcode = SQLExecDirect(hstmt, (SQLCHAR*)query6, SQL_NTS);
			}
			if (newyear)
			{
				char new_year[10];
				_itoa_s(year, new_year, 10, 10);
				char query7[200] = "UPDATE book SET publish_year = ";
				strcat_s(query7, new_year);
				strcat_s(query7, " where book_id = '");
				strcat_s(query7, id);
				strcat_s(query7, "'");
				retcode = SQLExecDirect(hstmt, (SQLCHAR*)query7, SQL_NTS);
			}
			if (newprice)
			{
				char new_price[10];
				sprintf_s(new_price, 10, "%.2f", price);
				char query8[200] = "UPDATE book SET price = ";
				strcat_s(query8, new_price);
				strcat_s(query8, " where book_id = '");
				strcat_s(query8, id);
				strcat_s(query8, "'");
				retcode = SQLExecDirect(hstmt, (SQLCHAR*)query8, SQL_NTS);
			}
			cout << "ͼ����Ϣ���³ɹ���" << endl;
		}
	}
	else
	{
		char new_year[10];
		_itoa_s(year, new_year, 10, 10);
		char new_price[10];
		sprintf_s(new_price, 10, "%.2f", price);
		char new_total[10];
		_itoa_s(total, new_total, 10, 10);
		char query1[200] = "INSERT INTO book VALUES('";
		strcat_s(query1, id);//book_id
		strcat_s(query1, "','");
		strcat_s(query1, title);//title
		strcat_s(query1, "','");
		strcat_s(query1, catalog);//catalog
		strcat_s(query1, "','");
		strcat_s(query1, author);//author
		strcat_s(query1, "','");
		strcat_s(query1, press);//press
		strcat_s(query1, "',");
		strcat_s(query1, new_year);//date
		strcat_s(query1, ",");
		strcat_s(query1, new_price);//price
		strcat_s(query1, ",");
		strcat_s(query1, new_total);//total_number
		strcat_s(query1, ",");
		strcat_s(query1, new_total);//stock = total
		strcat_s(query1, ")");
		retcode = SQLExecDirect(hstmt, (SQLCHAR*)query1, SQL_NTS);
		cout << "ͼ�����ɹ���" << endl;
	}

}
void Reader_Manag()
{
	int choice;
	char id[20] = { 0 };

	while (1)
	{
		cout << "[1].ɾ������֤\t[2].���ӽ���֤\t[3].����֤�޸�\t[4].���ļ�¼��ѯ\t[0].�˳�" << endl;
		cin >> choice;

		switch (choice)
		{
		case 0:
			return;
		case 1:
			delete_Reader();
			break;
		case 2:
			add_Reader();
			break;
		case 3:
			cout << "������Ҫ�޸ĵĽ���֤�ţ�";
			cin >> id;
			change_Reader(id);
			break;
		case 4:
			cout << "������Ҫ���Ľ��ļ�¼�Ľ���֤�ţ�";
			cin >> id;
			check_Record(id);
			break;
		default:
			cout << "�����Ŵ���" << endl;
		}
	}
}
void delete_Reader()
{
	char id[20] = { 0 };
	int flag = 0;
	cout << "����Ҫɾ���Ľ���֤���ţ�";
	cin >> id;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);

	/************�����Ƿ����иý���֤****************/
	char query[200] = "SELECT * FROM reader where reader_id = '";//��ѯ���
	strcat_s(query, id);
	strcat_s(query, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
			flag = 1;//���иý���֤
	}

	/************ɾ���ý���֤��������Ľ��ļ�¼****************/
	if (flag)
	{
		char query1[200] = "DELETE FROM record WHERE reader_id = '";
		strcat_s(query1, id);
		strcat_s(query1, "'");
		retcode = SQLExecDirect(hstmt, (SQLCHAR*)query1, SQL_NTS);

		char query2[200] = "DELETE FROM reader WHERE reader_id = '";
		strcat_s(query2, id);
		strcat_s(query2, "'");
		retcode = SQLExecDirect(hstmt, (SQLCHAR*)query2, SQL_NTS);
		cout << "����֤ɾ���ɹ���" << endl;
	}
	else
	{
		cout << "�ý���֤�����ڣ�" << endl;
		return;
	}
}
void add_Reader()
{
	while (1)
	{
		cout << "[1].�������" << '\t' << "[2].�������" << '\t' << "[0].�˳�" << endl;
		int add_op;
		cin >> add_op;
		if (!add_op)
			return;
		if (add_op == 1)//������ӣ���������
		{
			char id[20], name[20], department[20], type[10];
			cout << "�������뿨�š�������Ժϵ�����ѧ��/��ʦ��:" << endl;
			cin >> id >> name >> department >> type;
			AddOneReader(id, name, department, type);
		}
		else if (add_op == 2)//������ӣ�addreader.txt�ļ�����
		{
			string filename;
			cout << "����txt�ļ���: ";
			cin >> filename;
			ifstream fin(filename.c_str());
			if (!fin)
			{
				cout << "���ļ�ʧ�ܣ�" << endl;
				return;
			}
			while (!fin.eof())
			{
				char id[20], name[20], department[20], type[10];
				fin >> id >> name >> department >> type;
				AddOneReader(id, name, department, type);
			}
			fin.close();
		}
		else
			cout << "�����Ŵ���" << endl;
	}
}
void AddOneReader(char*id, char*name, char*department, char*type)
{
	int flag = 0;
	int newname = 0, newdepart = 0, newtype = 0;
	SQLCHAR old_name[20] = { 0 }, old_depart[20] = { 0 }, old_type[10] = { 0 };
	SQLLEN length;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);

	/************�����Ƿ����иý���֤****************/
	char query[200] = "SELECT * FROM reader where reader_id = '";//��ѯ���
	strcat_s(query, id);
	strcat_s(query, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		//������
		SQLBindCol(hstmt, 2, SQL_C_CHAR, old_name, sizeof(old_name), &length);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, old_depart, sizeof(old_depart), &length);
		SQLBindCol(hstmt, 4, SQL_C_CHAR, old_type, sizeof(old_type), &length);
		
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			flag = 1;//���иÿ�

			cout << "�Ѵ��ڸý���֤��";
			if (strcmp(name, (char*)old_name) != 0)
				newname = 1;
			if (strcmp(department, (char*)old_depart) != 0)
				newdepart = 1;
			if (strcmp(type, (char*)old_type) != 0)
				newtype = 1;
			if (!newname && !newdepart && !newtype)
			{
				cout << endl;
				return;
			}
		}
	}

	/************���иý���֤****************/
	if (flag)
	{
		/************�޸Ľ���֤��Ϣ****************/
		if (newname)
		{
			char query3[200] = "UPDATE reader SET reader_name ='";
			strcat_s(query3, name);
			strcat_s(query3, "' where reader_id = '");
			strcat_s(query3, id);
			strcat_s(query3, "'");
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)query3, SQL_NTS);
		}
		if (newdepart)
		{
			char query4[200] = "UPDATE reader SET department ='";
			strcat_s(query4, department);
			strcat_s(query4, "' where reader_id = '");
			strcat_s(query4, id);
			strcat_s(query4, "'");
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)query4, SQL_NTS);
		}
		if (newtype)
		{
			char query5[200] = "UPDATE reader SET reader_type ='";
			strcat_s(query5, type);
			strcat_s(query5, "' where reader_id = '");
			strcat_s(query5, id);
			strcat_s(query5, "'");
			retcode = SQLExecDirect(hstmt, (SQLCHAR*)query5, SQL_NTS);
		}
		cout << "����֤��Ϣ���³ɹ���" << endl;
	}
	/************��������֤****************/
	else
	{
		char query1[200] = "INSERT INTO reader VALUES('";
		strcat_s(query1, id);//reader_id
		strcat_s(query1, "','");
		strcat_s(query1, name);//reader_name
		strcat_s(query1, "','");
		strcat_s(query1, department);//department
		strcat_s(query1, "','");
		strcat_s(query1, type);//reader_type
		strcat_s(query1, "')");
		retcode = SQLExecDirect(hstmt, (SQLCHAR*)query1, SQL_NTS);
		cout << "����֤��ӳɹ���" << endl;
	}
}
void change_Reader(char*id)
{
	cout << "[1].�޸�����\t[2].�޸�Ժϵ\t[3].�޸�����\t[0].�˳�" << endl;
	int choice;
	cin >> choice;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, conn, &hstmt);
	char query[200] = "UPDATE reader SET ";//��ѯ���
	char name[20] = { 0 }, depart[20] = { 0 }, type[20] = { 0 };
	switch (choice)
	{
	case 0:
		return;
	case 1:
		cout << "�������޸�ֵ��";
		cin >> name;
		strcat_s(query, "reader_name = '");
		strcat_s(query, name);
		strcat_s(query, "'");
		break;
	case 2:
		cout << "�������޸�ֵ��";
		cin >> depart;
		strcat_s(query, "department = '");
		strcat_s(query, depart);
		strcat_s(query, "'");
		break;
	case 3:
		cout << "�������޸�ֵ��";
		cin >> type;
		strcat_s(query, "reader_type = '");
		strcat_s(query, type);
		strcat_s(query, "'");
		break;
	default:
		cout << "�����Ŵ���" << endl;
	}
	strcat_s(query, " WHERE reader_id = '");
	strcat_s(query, id);
	strcat_s(query, "'");
	retcode = SQLExecDirect(hstmt, (SQLCHAR*)query, SQL_NTS);
	cout << "����֤��Ϣ�޸ĳɹ���" << endl;
}


