//
//  SQL.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQL.h"

/*
 执行一行SQL语句
 */
SQLResultObject& SQL::execute(const char *sql)
{
    return execute(MyString(sql));
}

SQLResultObject& SQL::execute(const MyString& sql)
{
    return _executer.execute(_parser.parseLine(sql));
}

/*
 从文件导入数据到数据表
 */
SQLResultObject& SQL::import(const char *table, const char *filepath)
{
    return _executer.import(table, filepath);
}

SQLResultObject& SQL::import(const MyString& table, const char *filepath)
{
    return _executer.import(table, filepath);
}

/*
 导出数据表数据到文件
 */
SQLResultObject& SQL::xport(const char *table, const char *filepath)
{
    return _executer.xport(table, filepath);
}

SQLResultObject& SQL::xport(const MyString& table, const MyString& filepath)
{
    return _executer.xport(table, filepath);
}

SQLResultObject& SQL::xport(const MyString& table, const char *filepath)
{
    return _executer.xport(table, filepath);
}