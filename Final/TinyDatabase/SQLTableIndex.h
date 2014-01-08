//
//  SQLTableIndex.h
//  1352978
//
//  Created by Breezewish on 14-1-8.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLTableIndex_h
#define _352978_SQLTableIndex_h

#include <map>
#include "MyString.h"

class SQLTableIndex
{
public:
    std::multimap<float, int> _m_f;
    std::multimap<MyString, int> _m_s;

    SQLTableIndex()
    {
    }
};

#endif
