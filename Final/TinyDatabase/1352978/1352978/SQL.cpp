//
//  SQL.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQL.h"

SQL::SQL()
{
}

SQLResultObject& SQL::execute(const char *sql)
{
    return execute(MyString(sql));
}

SQLResultObject& SQL::execute(const MyString& sql)
{
    return _executer.execute(_parser.parseLine(sql));
}

SQLResultObject& SQL::import(const char *table, const char *filepath)
{
    return _executer.import(table, filepath);
}

SQLResultObject& SQL::import(const MyString& table, const char *filepath)
{
    return _executer.import(table, filepath);
}

void SQL::xport(const char *table, const char *filepath)
{
    return _executer.xport(table, filepath);
}

void SQL::xport(const MyString& table, const MyString& filepath)
{
    return _executer.xport(table, filepath);
}

void SQL::xport(const MyString& table, const char *filepath)
{
    return _executer.xport(table, filepath);
}