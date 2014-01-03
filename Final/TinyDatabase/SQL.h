//
//  SQL.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__SQL__
#define ___352978__SQL__

#include <ctime>
#include "MyString.h"
#include "SQLConstants.h"
#include "SQLStorage.h"
#include "SQLParser.h"
#include "SQLExecuter.h"
#include "SQLQueryObject.h"
#include "SQLResultObject.h"

class SQL
{
private:
    SQLExecuter _executer;
    SQLParser   _parser;
public:
    
    /*
     执行一行SQL语句
     */
    SQLResultObject& execute(const char *sql);
    SQLResultObject& execute(const MyString& sql);
    
    /*
     从文件导入数据到数据表
     */
    SQLResultObject& import(const char *table, const char *filepath);
    SQLResultObject& import(const MyString& table, const char *filepath);
    
    /*
     导出数据表数据到文件
     */
    void xport(const char *table, const char *filepath);
    void xport(const MyString& table, const MyString& filepath);
    void xport(const MyString& table, const char *filepath);
};

#endif /* defined(___352978__SQL__) */
