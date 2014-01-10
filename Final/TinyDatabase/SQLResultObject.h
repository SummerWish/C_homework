//
//  SQLResultObject.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__SQLResultObject__
#define ___352978__SQLResultObject__

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <fstream>
#include "MyString.h"
#include "SQLConstants.h"
#include "SQLTable.h"

class SQLSelectResultObject
{
public:
    int n;
    long execute_time;
    std::list<std::list<SQLTableRow>::iterator> *desired_rows;
    
    SQLSelectResultObject(long time)
    {
        n = 0;
        execute_time = time;
    }
    
    SQLSelectResultObject(long time, int scanned_objects, std::list<std::list<SQLTableRow>::iterator> *rows)
    {
        n = scanned_objects;
        execute_time = time;
        desired_rows = rows;
    }
    
    ~SQLSelectResultObject()
    {
        if (desired_rows != nullptr) {
            delete desired_rows;
        }
    }
};

class SQLResultObject
{
public:
    int n;
    int scanned;
    long execute_time;
    const SQLTable *table;
    
    /*
     空结果构造函数
     */
    SQLResultObject(long time);
    
    /*
     无返回构造函数
     */
    SQLResultObject(long time, int affected_rows, int scanned_rows);
    
    /*
     返回结果集的构造函数
     */
    SQLResultObject(long time, const SQLTable *records, int scanned_rows);
    
    /*
     析构函数
     */
    ~SQLResultObject();
    
    /*
     输出结果到流
     */
    void print(std::ostream& s) const;
    
    /*
     输出结果到文件
     */
    void xport(const MyString& filepath) const;
    void xport(const char *filepath) const;
};

#endif /* defined(___352978__SQLResultObject__) */
