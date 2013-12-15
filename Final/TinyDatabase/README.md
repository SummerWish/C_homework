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

```sql
CREATE TABLE table_name (
    column_name datatype(length) [NOT] NULL; 
    ...
) 
```

```sql
SELECT [TOP n] {*|column1, column2, ...} FROM table_name 
[WHERE where_statement]
[ORDER BY column_name [ASC|DESC] ] 
```

```sql
UPDATE table_name SET colume_name = column_value 
[WHERE where_statement]
```

```sql
DELETE FROM table_name 
[WHERE where_statement]
```

where_statement:

//TODO

### Example

//TODO