TinyDatabase
============

Final term project @ Tongji University.

MIT License. Copy the code at your own risk.

[Project requirement](http://sse.tongji.edu.cn/zhaoqinpei/Courses/CLanguage/2013级本科班课程项目.pdf)

```
Breezewish's Tiny SQL Database
Version: 0.0.1
-c create.txt
Completed without errors in 0ms.
-i student data.txt
Completed without errors in 0ms.
Imported 5 rows.
-it
interactive> select * from student
Completed without errors in 0ms.
Affected 5 rows.
1              aaa            90             
2              bbb            90             
3              ccc            90             
4              ddd            80             
5              ddd            70             
interactive> select id, name from student where grade > 75
Completed without errors in 0ms.
Affected 4 rows.
1              aaa            
2              bbb            
3              ccc            
4              ddd            
interactive> select id, name from student where grade > 75 and (name like "a" or name = "bbb")
Completed without errors in 0ms.
Affected 2 rows.
1              aaa            
2              bbb            
interactive> delete from student where name="ddd" and grade < 80
Completed without errors in 0ms.
Affected 1 rows.
interactive> select * from student
Completed without errors in 0ms.
Affected 4 rows.
1              aaa            90             
2              bbb            90             
3              ccc            90             
4              ddd            80             
interactive> update student set name="hello" where grade > 80
Completed without errors in 0ms.
Affected 3 rows.
1              hello          90             
2              hello          90             
3              hello          90             
interactive> select * from student
Completed without errors in 0ms.
Affected 4 rows.
1              hello          90             
2              hello          90             
3              hello          90             
4              ddd            80             
interactive> update student set name="\"hello\"" where id >= 4 or id < 2
Completed without errors in 0ms.
Affected 2 rows.
1              "hello"        90             
4              "hello"        80             
interactive> select * from student
Completed without errors in 0ms.
Affected 4 rows.
1              "hello"        90             
2              hello          90             
3              hello          90             
4              "hello"        80             
interactive> exit
-h
c       file           Create table from file
i       table file     Import data to table
s       file           Select data by file
u       file           Update data by file
d       file           Delete data by file
it                     Enter interactive mode
index   0|1            Switch off/on index
q                      Quit
h                      Show this screen
-q
bye
```