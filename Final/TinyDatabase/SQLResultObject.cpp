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
                                 const MyString& affect_table, const std::list<SQLTableRow>& _rows, const std::vector<int>& _colTypes)
{
    tableName = affect_table;
    execute_time = time;
    ok = true;
    n = (int)_rows.size();
    colTypes = _colTypes;
    rows = _rows;
}

/*
 输出结果到流
 */
void SQLResultObject::print(std::ostream& s) const
{
    s << std::left;
    
    for (auto it = rows.begin(); it != rows.end(); ++it) {
        auto &row = *it;
        for (int i = 0; i < row.cols.size(); ++i) {
            if (colTypes[i] == SQLConstants::COLUMN_TYPE_CHAR) {
                s << std::setw(15) << row.cols[i]._v_s;
            } else {
                s << std::setw(15) << row.cols[i]._v_f;
            }
        }
        
        s << std::endl;
    }
}

/*
 输出结果到文件
 */
void SQLResultObject::xport(const MyString& filepath) const
{
    char *fp = filepath.toCString();
    xport(fp);
    delete[] fp;
}

void SQLResultObject::xport(const char *filepath) const
{
    std::ofstream fout;
    fout.open(filepath);
    print(fout);
    fout.close();
}