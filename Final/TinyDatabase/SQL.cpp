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
    return _executer.execute(_storage, _parser.parseLine(sql));
}

/*
 从文件导入数据到数据表
 */
SQLResultObject& SQL::import(const char *table, const char *filepath)
{
    return import(MyString(table), filepath);
}

SQLResultObject& SQL::import(const MyString& table, const char *filepath)
{
    MyTimer timer;
    
    MyString table_ = table.toUpper();
    bool exists = _storage.tableExists(table_);
    if (!exists) {
        return *new SQLResultObject(MyString("Table [").concat(table_).concat("] not exists"));
    }
    
    int n = _storage[table_].import(filepath);
    
    return *new SQLResultObject(timer.elapsed(), table_, n);
}

/*
 导出数据表数据到文件
 */
SQLResultObject& SQL::xport(const char *table, const char *filepath)
{
    return xport(MyString(table), filepath);
}

SQLResultObject& SQL::xport(const MyString& table, const MyString& filepath)
{
    MyTimer timer;
    
    MyString table_ = table.toUpper();
    bool exists = _storage.tableExists(table_);
    if (!exists) {
        return *new SQLResultObject(MyString("Table [").concat(table_).concat("] not exists"));
    }
    
    int n = _storage[table_].xport(filepath);
    
    return *new SQLResultObject(timer.elapsed(), table_, n);
}

SQLResultObject& SQL::xport(const MyString& table, const char *filepath)
{
    MyTimer timer;
    
    MyString table_ = table.toUpper();
    bool exists = _storage.tableExists(table_);
    if (!exists) {
        return *new SQLResultObject(MyString("Table [").concat(table_).concat("] not exists"));
    }
    
    int n = _storage[table_].xport(filepath);
    
    return *new SQLResultObject(timer.elapsed(), table_, n);
}