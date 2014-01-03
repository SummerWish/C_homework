//
//  SQLTableHeader.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLTableHeader_h
#define _352978_SQLTableHeader_h

#include "SQLConstants.h"
#include "MyString.h"

class SQLTableHeader
{
public:
    MyString name;
    int type;
    int size;
    bool can_null;
    
    SQLTableHeader(const MyString& _name, int _type)
    {
        name = _name.toUpper();
        type = _type;
    }
    
    SQLTableHeader(const MyString& _name, int _type, int _size, bool _can_null)
    {
        name = _name.toUpper();
        type = _type;
        size = _size;
        can_null = _can_null;
    }
};

#endif
