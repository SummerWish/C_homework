//
//  SQLParser.h
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__SQLParser__
#define ___352978__SQLParser__

#include <map>
#include <vector>
#include "MyString.h"
#include "SQLConstants.h"
#include "SQLQueryObject.h"
#include "SQLResultObject.h"

class SQLParser
{
public:
    enum
    {
        PARSE_STATUS_TOKEN          = 0,
        PARSE_STATUS_DELIM          = 1
    };
    
    /*
     解析一行SQL语句，转换为SQLQueryObject
     */
    SQLQueryObject& parseLine(const char *line);
    SQLQueryObject& parseLine(const MyString& line);
};

#endif /* defined(___352978__SQLParser__) */
