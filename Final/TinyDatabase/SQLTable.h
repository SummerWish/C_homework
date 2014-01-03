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
#include "SQLTableHeader.h"
#include "SQLTableRow.h"

class SQLTable
{
private:
    std::map<MyString, int> _headMapping;
    
public:
    std::vector<SQLTableHeader> head;
    std::list<SQLTableRow> rows;
    
    int getColumnIndexByName(const MyString& name) const
    {
        auto it = _headMapping.find(name.toUpper());
        
        if (it != _headMapping.end()) {
            return it->second;
        } else {
            return -1;
        }
    }
    
    bool createColumn(const MyString& name, int type)
    {
        MyString _name = name.toUpper();
        
        if (getColumnIndexByName(_name) != -1) {
            // column exists
            return false;
        } else {
            _headMapping[_name] = (int)head.size();
            head.push_back(SQLTableHeader(_name, type));
            
            return true;
        }
    }
    
    void print(std::ostream& s) const
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
