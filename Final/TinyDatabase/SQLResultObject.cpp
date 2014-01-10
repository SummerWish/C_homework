//
//  SQLResultObject.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQLResultObject.h"

SQLResultObject::SQLResultObject(long time)
{
    table = nullptr;
    execute_time = time;
}

SQLResultObject::SQLResultObject(long time, int affected_rows, int scanned_rows)
{
    table = nullptr;
    execute_time = time;
    n = affected_rows;
    scanned = scanned_rows;
}

SQLResultObject::SQLResultObject(long time, const SQLTable *records, int scanned_rows)
{
    execute_time = time;
    
    table = records;
    n = (int)records->rows.size();
    scanned = scanned_rows;
}

SQLResultObject::~SQLResultObject()
{
    if (table != nullptr) {
        delete table;
    }
}

/*
 输出结果到流
 */
void SQLResultObject::print(std::ostream& s) const
{
    if (table != nullptr) {
        table->print(s);
    }
}

/*
 输出结果到文件
 */
void SQLResultObject::xport(const MyString& filepath) const
{
    if (table != nullptr) {
        table->xport(filepath);
    }
}

void SQLResultObject::xport(const char *filepath) const
{
    if (table != nullptr) {
        table->xport(filepath);
    }
}