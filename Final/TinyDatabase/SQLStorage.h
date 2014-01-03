//
//  SQLStorage.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLStorage_h
#define _352978_SQLStorage_h

#include <vector>
#include <map>
#include "MyString.h"
#include "SQLTable.h"

class SQLStorage
{
private:
    std::map<MyString, SQLTable> _tables;

public:
    
    bool tableExists(const char *table) const
    {
        return tableExists(MyString(table));
    }
    
    bool tableExists(const MyString& table) const
    {
        if (_tables.find(table.toUpper()) == _tables.end()) {
            return false;
        } else {
            return true;
        }
    }
    
    SQLTable& selectTable(const char *table)
    {
        return selectTable(MyString(table));
    }
    
    SQLTable& selectTable(const MyString& table)
    {
        return _tables.find(table.toUpper())->second;
    }
    
    SQLTable& createTable(const char *table)
    {
        return createTable(MyString(table));
    }
    
    SQLTable& createTable(const MyString& table)
    {
        auto *table_ = new SQLTable();
        _tables[table.toUpper()] = *table_;
        return *table_;
    }
    
    SQLTable& operator[](const char *table)
    {
        return selectTable(MyString(table));
    }
    
    SQLTable& operator[](const MyString& table)
    {
        return selectTable(table);
    }
    
};

#endif
