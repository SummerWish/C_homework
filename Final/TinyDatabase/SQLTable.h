//
//  SQLTable.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__SQLTable__
#define ___352978__SQLTable__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include "MyString.h"
#include "SQLTableHeader.h"
#include "SQLTableRow.h"

class SQLTable
{
private:
    std::map<MyString, int> _headMapping;
    
public:
    std::vector<SQLTableHeader> head;
    std::list<SQLTableRow> rows;
    
    /*
     给定列名，返回列下标
     若没有这个列，则返回-1
     */
    int getColumnIndexByName(const MyString& name) const
    {
        auto it = _headMapping.find(name.toUpper());
        
        if (it != _headMapping.end()) {
            return it->second;
        } else {
            return -1;
        }
    }
    
    /*
     添加一列
     */
    bool createColumn(const SQLTableHeader column)
    {
        if (getColumnIndexByName(column.name) != -1) {
            // column exists
            return false;
        } else {
            _headMapping[column.name.toUpper()] = (int)head.size();
            head.push_back(column);
            return true;
        }
    }
    
    bool createColumn(const MyString& name, int type)
    {
        if (getColumnIndexByName(name) != -1) {
            return false;
        } else {
            return createColumn(SQLTableHeader(name, type));
        }
    }
    
    /*
     输出结果到流
     */
    void print(std::ostream& s) const
    {
        s << std::left;
        
        // output columns
        for (auto it = head.begin(); it != head.end(); ++it) {
            auto &head = *it;
            s << std::setw(15) << head.name;
        }
        s << std::endl;
        
        for (auto it = head.begin(); it != head.end(); ++it) {
            s << "===============";
        }
        s << std::endl;
        
        // output content
        for (auto it = rows.begin(); it != rows.end(); ++it) {
            auto &row = *it;
            for (int i = 0; i < row.cols.size(); ++i) {
                if (head[i].type == SQLConstants::COLUMN_TYPE_CHAR) {
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
    void xport(const MyString& filepath) const
    {
        char *fp = filepath.toCString();
        xport(fp);
        delete[] fp;
    }
    
    void xport(const char *filepath) const
    {
        std::ofstream fout;
        fout.open(filepath);
        print(fout);
        fout.close();
    }
    
    /*
     从文件导入数据
     */
    int import(std::istream& s)
    {
        char tmp[4096];
        int affected_rows = 0;
        
        while (s.getline(tmp, 4096)) {
            auto cols = my_split(tmp, (char*)",");
            
            if (cols.size() > head.size()) {
                continue;
            }
            
            affected_rows++;
            SQLTableRow row;
            row.cols.reserve(head.size());
            
            for (int i = 0; i < cols.size(); ++i) {
                SQLTableCell col;
                
                if (head[i].type == SQLConstants::COLUMN_TYPE_CHAR) {
                    col._v_s = cols[i];
                } else {
                    col._v_f = cols[i].toFloat();
                }
                
                row.cols.push_back(col);
            }
            
            rows.push_back(row);
        }
        
        return affected_rows;
    }
};

#endif /* defined(___352978__SQLTable__) */
