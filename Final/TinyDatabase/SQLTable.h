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
#include "SQLTableIndex.h"
#include "SQLCondition.h"

class SQLTable
{
private:
    std::map<MyString, int> _headMapping;
    int increase_counter;
    
public:
    MyString name;
    std::vector<SQLTableHeader> head;
    std::list<SQLTableRow> rows;

    std::vector<SQLTableIndex> indexes; //index by column
    
    SQLTable()
    {
        increase_counter = 0;
    }
    
    SQLTable(const MyString& _name) {
        name = _name;
        increase_counter = 0;
    }
    
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
     给定列名，返回列下标
     若没有这个列，则抛出异常
     */
    int getColumnIndexByNameWithException(const MyString& name) const
    {
        int ret = getColumnIndexByName(name);
        if (ret != -1) {
            return ret;
        } else {
            throw MyString("Column [").concat(name).concat("] does not exist");
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
            indexes.push_back(SQLTableIndex());
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
    int print(std::ostream& s) const
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
        
        return (int)rows.size();
    }
    
    /*
     输出结果到文件
     */
    int xport(const MyString& filepath) const
    {
        char *fp = filepath.toCString();
        int n = xport(fp);
        delete[] fp;
        return n;
    }
    
    int xport(const char *filepath) const
    {
        std::ofstream fout;
        fout.open(filepath);
        
        if (!fout.good()) {
            throw MyString("Failed to write file");
        }
        
        int n = print(fout);
        fout.close();
        return n;
    }
    
    /*
     从流中导入数据
     单行最大不超过100KB
     */
    int import(std::istream& s)
    {
        char *tmp = new char[102400];
        int affected_rows = 0, ignored_rows = 0;

        int colSize = (int)head.size();
        
        while (s.getline(tmp, 102400)) {
            auto cols = my_split(tmp, (char*)",");
            
            if (cols.size() != head.size()) {
                ignored_rows++;
                continue;
            }
            
            affected_rows++;
            
            // create row
            int _id = increase_counter++;
            SQLTableRow row(_id, colSize);
            
            // insert other data
            for (int i = 0; i < colSize; ++i) {
                if (head[i].type == SQLConstants::COLUMN_TYPE_CHAR) {
                    row.cols.push_back(SQLTableCell(cols[i]));
                } else {
                    row.cols.push_back(SQLTableCell(cols[i].toFloat()));
                }
            }
            
            rows.push_back(row);
            
            // reference to index collection
            auto it = rows.end();
            it--;
            
            // update index
            for (int i = 0; i < colSize; ++i) {
                if (head[i].type == SQLConstants::COLUMN_TYPE_CHAR) {
                    indexes[i]._m_s.insert(std::make_pair(cols[i], it));
                } else {
                    indexes[i]._m_f.insert(std::make_pair(cols[i].toFloat(), it));
                }
            }
        }
        
        if (ignored_rows > 0) {
            std::cout << "[Warning] Ignored " << ignored_rows << " rows." << std::endl;
        }
        
        return affected_rows;
    }
    
    /*
     从文件导入数据
     */
    int import(const char *filepath)
    {
        std::ifstream fin;
        fin.open(filepath);
        
        if (!fin.good()) {
            throw MyString("Cannot open file");
        }
        
        int n = import(fin);
        fin.close();
        
        return n;
    }

};

#endif /* defined(___352978__SQLTable__) */
