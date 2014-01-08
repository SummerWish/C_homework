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
    std::vector<SQLTableHeader> head;
    std::list<SQLTableRow> rows;

    // for indexes
    //std::map<int, std::list<SQLTableRow>::iterator> _id2it;
    std::vector<SQLTableIndex> indexes; //index by column
    
    SQLTable()
    {
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
     给定一个条件，返回一个包含了所有符合条件的行枚举器的列表
     该函数会尝试使用索引
     */
    std::list<std::list<SQLTableRow>::iterator> getTargetRowIterators(CompiledSQLConditionObject& condition)
    {
        std::list<std::list<SQLTableRow>::iterator> desired_rows;

        auto index_stat = condition.statIndex();

        if (!index_stat.can_optimize) {
            // no index can be used, enum all rows and test conditions
            for (auto it = rows.begin(); it != rows.end(); ++it) {
                if (condition.test(*it)) {
                    desired_rows.push_back(it);
                }
            }
        } else {
            // can use index
            if (head[index_stat.row_index].type == SQLConstants::COLUMN_TYPE_FLOAT) {
                auto& idx = indexes[index_stat.row_index]._m_f;
                for (auto _it = idx.find(index_stat._v_f); _it != idx.end(); ++_it) {
                    auto& it = (*_it).second;
                    if (condition.test(*it)) {
                        desired_rows.push_back(it);
                    }
                }
            } else if (head[index_stat.row_index].type == SQLConstants::COLUMN_TYPE_CHAR) {
                auto& idx = indexes[index_stat.row_index]._m_s;
                for (auto _it = idx.find(index_stat._v_s); _it != idx.end(); ++_it) {
                    auto& it = (*_it).second;
                    if (condition.test(*it)) {
                        desired_rows.push_back(it);
                    }
                }
            }
        }
        
        return desired_rows;
    }

    /*
     删除一行，同时删除索引
     */
    std::list<SQLTableRow>::iterator removeRow(std::list<SQLTableRow>::iterator& it)
    {
        auto& row = *it;
        
        // remove index
        for (int i = 0; i < head.size(); ++i) {
        
            int& type = head[i].type;
            
            if (type == SQLConstants::COLUMN_TYPE_FLOAT) {
                
                auto& idx = indexes[i]._m_f;
                
                for (auto _it = idx.find(row.cols[i]._v_f); _it != idx.end(); ++_it) {
                    if ((*_it).second == it) {
                        idx.erase(_it);
                        break;
                    }
                }
                
            } else if (type == SQLConstants::COLUMN_TYPE_CHAR) {
                
                auto& idx = indexes[i]._m_s;
                
                for (auto _it = idx.find(row.cols[i]._v_s); _it != idx.end(); ++_it) {
                    if ((*_it).second == it) {
                        idx.erase(_it);
                        break;
                    }
                }
                
            }
            
        }
        
        // remove row
        return rows.erase(it);
    }

    /*
     删除所有行，并删除所有索引
     */
    void removeAllRows()
    {
        rows.clear();

        for (auto it = indexes.begin(); it != indexes.end(); ++it) {
            (*it)._m_f.clear();
            (*it)._m_s.clear();
        }
    }

    /*
     更新行，并更新索引
     */
    void updateRow(std::list<SQLTableRow>::iterator& _it, int column, float new_value)
    {
        auto& row = *_it;
        
        // update value
        float old_value = row.cols[column]._v_f;
        row.cols[column]._v_f = new_value;

        // update index
        auto& index = indexes[column]._m_f;
        for (auto it = index.find(old_value); it != index.end(); ++it) {
            if ((*it).second == _it) {
                index.erase(it);
                break;
            }
        }
    }

    void updateRow(std::list<SQLTableRow>::iterator& _it, int column, MyString& new_value)
    {
        auto& row = *_it;

        // update value
        MyString old_value = row.cols[column]._v_s;
        row.cols[column]._v_s = new_value;

        // update index
        auto& index = indexes[column]._m_s;
        for (auto it = index.find(old_value); it != index.end(); ++it) {
            if ((*it).second == _it) {
                index.erase(it);
                break;
            }
        }
    }

    /*
     重建一个列的索引为某个值
     */
    void regenerateIndex(int column, float new_value)
    {
        indexes[column]._m_f.clear();

        for (auto it = rows.begin(); it != rows.end(); ++it) {
            indexes[column]._m_f.insert(std::make_pair(new_value, it));
        }
    }

    void regenerateIndex(int column, MyString& new_value)
    {
        indexes[column]._m_s.clear();

        for (auto it = rows.begin(); it != rows.end(); ++it) {
            indexes[column]._m_s.insert(std::make_pair(new_value, it));
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
     */
    int import(std::istream& s)
    {
        char tmp[4096];
        int affected_rows = 0;

        int colSize = (int)head.size();
        
        while (s.getline(tmp, 4096)) {
            auto cols = my_split(tmp, (char*)",");
            
            if (cols.size() != head.size()) {
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
