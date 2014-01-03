//
//  SQLResultObject.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQLResultObject.h"

SQLResultObject::SQLResultObject(const MyString& error)
{
    ok = false;
    err = error;
}

SQLResultObject::SQLResultObject(long time,
                                 const MyString& affect_table)
{
    tableName = affect_table;
    execute_time = time;
    ok = true;
}

SQLResultObject::SQLResultObject(long time,
                                 const MyString& affect_table, int affected_rows)
{
    tableName = affect_table;
    execute_time = time;
    ok = true;
    n = affected_rows;
}

SQLResultObject::SQLResultObject(long time,
                                 const MyString& affect_table, const SQLTable& records)
{
    tableName = affect_table;
    execute_time = time;
    ok = true;
    
    table = records;    //TODO: avoid copying
    n = (int)table.rows.size();
}

/*
 输出结果到流
 */
void SQLResultObject::print(std::ostream& s) const
{
    table.print(s);
}

/*
 输出结果到文件
 */
void SQLResultObject::xport(const MyString& filepath) const
{
    table.xport(filepath);
}

void SQLResultObject::xport(const char *filepath) const
{
    table.xport(filepath);
}