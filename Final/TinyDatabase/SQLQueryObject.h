//
//  SQLQueryObject.h
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLQueryObject_h
#define _352978_SQLQueryObject_h

#include <set>
#include <map>
#include <iostream>
#include <iomanip>
#include "SQLConstants.h"
#include "SQLTable.h"
#include "SQLFilter.h"
#include "SQLCondition.h"
#include "MyString.h"

class SQLCreateTableColumnObject
{
public:
    MyString name;
    int type;
    int size;
    bool can_null;
    bool end;
};

class SQLWhereStatementObject
{
public:
    MyString name;
    MyString value;
    int op;
};

class SQLWhereComponentObject
{
public:
    int type;
    SQLWhereStatementObject statement;
    
    SQLWhereComponentObject(int _type);
    SQLWhereComponentObject(const SQLWhereStatementObject _statement);
};

class SQLQueryObject
{
public:
    
    MyString _query;
    std::map<int, int> _int;
    std::map<int, MyString> _str;
    std::vector<MyString> _select_columns;
    std::vector<SQLCreateTableColumnObject> _create_table_columns;
    std::vector<SQLWhereComponentObject> _where_statements;
    
    SQLQueryObject();
    SQLQueryObject(const MyString& query);
    int OperationStringToToken(const MyString& s) const;
    int ColumnTypeToInt(const MyString& s) const;
    //void _print() const;
    
    /*
     检查条件是否有效
     */
    bool checkCondition(const SQLTable& table) const;
    /*
     将 COLUMN_NAME 转换为 index，并转换为后缀表达式
     */
    CompiledSQLConditionObject& compileCondition(const SQLTable& table) const;
    
    /*
     检查过滤列是否有效
     */
    bool checkFilter(const SQLTable& table) const;
    /*
     去除重复的 COLUMN_NAME，并将 COLUMN_NAME 转换为 index
     */
    CompiledSQLFilterObject& compileFilter(const SQLTable& table) const;
    
};

#endif
