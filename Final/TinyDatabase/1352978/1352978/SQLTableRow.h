//
//  SQLTableRow.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLTableRow_h
#define _352978_SQLTableRow_h

#include <vector>
#include "MyString.h"
#include "SQLTableColumn.h"

class SQLTableRow
{
public:
    std::vector<SQLTableColumn> cols;
};

#endif
