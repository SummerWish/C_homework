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
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include "MyString.h"
#include "SQLTableHeaderColumn.h"
#include "SQLTableRow.h"

class SQLTable
{
public:
    std::vector<SQLTableHeaderColumn> head;
    std::map<MyString, int> _colIndex;
    std::list<SQLTableRow> rows;
    
    bool createColumn(const MyString& name, int type, int size, bool can_null)
    {
        //_colIndex = std::map<MyString, int>();
        
        MyString _name = name.toUpper();
        
        if (_colIndex.find(_name) != _colIndex.end()) {
            // column exists
            
            return false;
        } else {
            // std::cout << _name << " " << (int)head.size() << std::endl;
            _colIndex[_name] = (int)head.size();
            
            head.push_back(SQLTableHeaderColumn(_name, type, size, can_null));
            
            return true;
        }
    }
    
    void print(std::ostream& s)
    {
        s << std::left;
        
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
            
            for (int i = 0; i < cols.size(); ++i) {
                SQLTableColumn col;
                
                col._type = head[i].type;
                
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
