TinyDatabase
============

A tiny in-memory SQL database. Just a final term homework.

## Requirement

```
g++ >= 4.7
cmake >= 2.6
```

## Build

```
cmake .
make
```

## Usage

### Run

```
./tinydatabase
```

### Commands

```
c       file           Create table from file
i       table file     Import data to table
s       file           Select data by file
u       file           Update data by file
d       file           Delete data by file
it                     Enter interactive mode
q                      Quit
h                      Show help info
```

## Example

### Create database

create.txt:

```sql
create table Student 
( 
 id float(4) NOT NULL; 
 name char(100) NULL; 
 grade float(4) NULL; 
)
```

Command:

```
-c create.txt
Completed without errors in 0ms.
```

### Import data

data.txt:

```
1,aaa,90
2,bbb,90
3,ccc,90
4,ddd,80
5,ddd,70
```

Command:

```
-i student data.txt
Completed without errors in 0ms.
Imported 5 rows.
```

### Select

select.txt:

```sql
select top 2 * from Student where grade = 90
```

Command:

```
-s select.txt
Completed without errors in 0ms.
Selected 2 rows.
Data outputed to: 1352978_select_3.txt
```

Select result (1352978\_select\_3.txt):

```
1              aaa            90             
2              bbb            90             
```

### Update

update.txt:

```sql
update Student set grade = 80 where id = 5
```

Command:

```
-u update.txt
Completed without errors in 0ms.
Updated 1 rows.
Data outputed to: 1352978_update_4.txt
```

Update result (1352978\_update\_4.txt):

```
5              ddd            80             
```

### Delete

delete.txt:

```sql
delete from Student where id = 4
```

Command:

```
-d delete.txt
Completed without errors in 0ms.
Affected 1 rows.
Table data outputed to: 1352978_delete_5.txt
```

Delete result (1352978\_delete\_5.txt):

```
1              aaa            90             
2              bbb            90             
3              ccc            90             
5              ddd            80             
```

## Supported SQL Syntax

### Overview

TinyDatabase only supports the following 4 types of operations.

Create table:

```sql
CREATE TABLE table_name (
    col1 datatype(length) [NOT] NULL;
    col2 datatype(length) [NOT] NULL; 
    ...
    colN datatype(length) [NOT] NULL; 
) 
```

Select data:

```sql
SELECT [TOP n] * FROM table_name 
[WHERE where_statement]
[ORDER BY column_name [ASC|DESC] ] 
```

```sql
SELECT [TOP n] col1, col2, ... colN FROM table_name 
[WHERE where_statement]
[ORDER BY column_name [ASC|DESC] ] 
```

Update data:

```sql
UPDATE table_name SET colume_name = column_value 
[WHERE where_statement]
```

Delete data:

```sql
DELETE FROM table_name 
[WHERE where_statement]
```

The syntax of `where_statement` is:

```sql
condition1 OR|AND condition2 OR|AND condition3 ...
```

The syntax of `condition` is:

```sql
col1 OPERATION value1
```

`OPERATION` could be: `=`,`<`,`>`,`<=`,`>=`,`LIKE`.

### Example

1. Create table

    ```sql
    CREATE TABLE student (
        id		float(4) not null;
        name	char(20) not null;
        class	float(4) not null;
        score	float(4) null;
    )
    ```
    Description: Create an empty table named `student` which contains 4 columns:
    - `id` is `float`
    - `name` is `char` with size 20
    - `class` is `float`
    - `socre` is `float` which could be null

2. Select all columns
    
    ```sql
    SELECT * FROM student
    ```
    Description: Select all records from `student` table.

3. Select specific columns
    
    ```sql
    SELECT id, name FROM student
    ```
    Description: Select `id` and `name` of all records from `student` table.

4. Single WHERE condition

    ```sql
    SELECT * FROM student WHERE class = 5
    ```
    Description: Select records which `class` is `5` from `student` table.

5. Multiple WHERE conditions

    ```sql
    SELECT * FROM student WHERE class = 5 and socre > 90
    ```
    Description: Select records which `class` is `5` and `score` is greater than `90` from `student` table.

6. Match strings in WHERE conditions

    ```sql
    SELECT * FROM student WHERE name = "swx"
    ```
    Description: Select records which `name` is `swx` from `student` table.

7. Match substrings in WHERE conditions

    ```sql
    SELECT * FROM student WHERE name LIKE "wang"
    ```
    Description: Select records which `name` *contains* `wang` from `student` table.

8. Multiple WHERE conditions with brackets
    
    ```sql
    SELECT * FROM student WHERE score >= 90 AND (class = 1 OR class = 5)
    ```
    
    Description: Select records which `socre` is greater than or equal to `90` and `class` is `1` or `class` is `5` from `student` table.

9. `TOP` keyword
    
    ```sql
    SELECT TOP 1 * FROM student
    ```
    
    Description: Select the first record from `student` table.

10. Sorting records
    
    ```sql
    SELECT * FROM student ORDER BY score
    ```
    
    Description: Select all records and sort by `score` with ascending order from `student` table.

11. Descending order in sorting records
    
    ```sql
    SELECT * FROM student ORDER BY score DESC
    ```
    
    Description: Select all records and sort by `score` with descending order from `student` table.

12. Using `TOP` and `ORDER`
    
    ```sql
    SELECT TOP 1 * FROM student ORDER BY score DESC
    ```
    
    Description: Select the highest `score` record from `student` table.

13. Update data with conditions
    
    ```sql
    UPDATE student SET name = "mulberryleaf" WHERE id = 1351633
    ```
    
    Description: Set the value of `name` to `mulberryleaf` which `id` is `1351633` in `student` table.
    
    Notice: If there are multiple records with `id` equal to `1351633`, all of these records will be updated.

14. Update data with multiple conditions
    
    ```sql
    UPDATE student SET score = 100 WHERE name = "mulberryleaf" AND class = 3
    ```
    
    Description: Set the value of `score` to `100` which `name` is `mulberryleaf` and `class` is `3` in `student` table.

15. Update without conditions
    
    ```sql
    UPDATE student SET score = 0
    ```
    
    Description: Set the value of `score` to `0` of all records in `student` table.

16. Delete all data
    
    ```sql
    DELETE FROM student
    ```
    
    Description: Delete all records of `student` table.

17. Delete data with conditions
    
    ```sql
    DELETE FROM student WHERE class < 0 OR class > 5
    ```
    
    Description: Delete records which `class` is lower than `0` or greater than `5` of `student` table.

18. Special characters in strings
    
    ```sql
    UPDATE student SET introduce = "Hi, my name is \"mulberryleaf\"." WHERE id = 1351633
    ```
    
    Description: Set the value of `introduce` to `Hi, my name is "mulberryleaf".` which `id` is `1351633` in `student` table.

## Interactive mode

You can input and execute SQL statements in interactive mode. And the result of the SQL statement will be printed directly on the screen. Interactive mode gives you a simpler way to execute SQL statements.

To enter interactive mode, input `it`.

To exit interactive mode, input `exit`.

```
Breezewish's Tiny SQL Database
Version: 0.0.1.20131221.200800
-h
c       file           Create table from file
i       table file     Import data to table
s       file           Select data by file
u       file           Update data by file
d       file           Delete data by file
it                     Enter interactive mode
q                      Quit
h                      Show this screen
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
-q
bye
```

## License

The MIT License (MIT)

Copyright (c) 2013 Breezewish

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.