use library;
create table book
( 
  book_id        varchar(20) primary key,
  title          varchar(30) not null, 
  catalog        varchar(30) not null,
  author         varchar(30),
  press          varchar(30),
  publish_year   integer,
  price          numeric(12,2),
  total_number   integer,
  stock          integer
);

create table reader 
( 
 reader_id   varchar(20) primary key,
 reader_name varchar(20) not null,
 department  varchar(20) not null,
 reader_type varchar(10)
);

create table record
( 
 record_id   int not null primary key auto_increment,
 book_id     varchar(20) not null,
 reader_id   varchar(20)  not null,
 borrow_date datetime,
 due_date    datetime,
 return_date datetime,
 isreturned  varchar(2),
 foreign key (book_id) references book(book_id),
 foreign key (reader_id) references reader(reader_id)
);

insert into book values('20161112002','初妆张爱玲','文学艺术','陶舒天','新华出版社',2014,25,1,1);
insert into reader values('20151101','姬彦雪','英语','学生');
insert into reader values('20151102','郝永宸','数学','学生');
insert into reader values('20151103','于新磊','物理','学生');
insert into reader values('20151104','殷娜梅','物理','学生');
insert into reader values('20151105','宋天鸣','生物','学生');
insert into reader values('20111217','石逸轩','英语','教师');
insert into reader values('20111202','孟灵丽','数学','教师');
insert into record(book_id,reader_id,borrow_date,due_date,return_date,isreturned) values('20161112002','20151101',NOW(),date_add(NOW(), interval 1 MONTH),NULL,'N');
update book set stock = stock - 1 where book_id = '20161112006';
UPDATE record SET isreturned = 'Y' WHERE book_id = '20161112002' and reader_id = '20151101';
UPDATE record SET return_date = now() WHERE book_id = '20161112002' and reader_id = '20151101';
SELECT min(due_date) FROM record WHERE book_id = '20161112002';
SELECT due_date FROM record WHERE record_id = (select min(record_id) from record where book_id = '20161112002');
delete from reader where reader_id = '20161101';