//
//  SQLExecuter.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__SQLExecuter__
#define ___352978__SQLExecuter__

#include <iostream>
#include <vector>
#include <list>
#include "MyString.h"
#include "MyTimer.h"
#include "SQLConstants.h"
#include "SQLTableHeader.h"
#include "SQLTableCell.h"
#include "SQLTableRow.h"
#include "SQLTable.h"
#include "SQLStorage.h"
#include "SQLResultObject.h"
#include "SQLQueryObject.h"

class SQLExecuter
{
private:
    SQLStorage _storage;
    
public:
    
    /*
     执行单一SQL语句
     */
    SQLResultObject& execute(const SQLQueryObject& query);
    
    /*
     从文件导入数据
     */
    SQLResultObject& import(const char *table, const char *filepath);
    SQLResultObject& import(const MyString& table, const char *filepath);
    
    /*
     导出数据到文件
     */
    void xport(const char *table, const char *filepath);
    void xport(const MyString& table, const MyString& filepath);
    void xport(const MyString& table, const char *filepath);
};


#endif /* defined(___352978__SQLExecuter__) */
