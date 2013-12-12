//
//  SQLFilter.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLFilter_h
#define _352978_SQLFilter_h

#include <set>
#include "SQLTableColumn.h"
#include "SQLTableRow.h"

class CompiledSQLFilterObject
{
public:
    bool wild;
    std::set<int> columns;
    
    SQLTableRow& filter(SQLTableRow& row) const
    {
        if (wild) {
            return row;
        }
        
        SQLTableRow *ret = new SQLTableRow();
        
        for (std::set<int>::iterator it = columns.begin(); it != columns.end(); ++it) {
            ret->cols.push_back(row.cols[(*it)]);
        }
        
        return *ret;
    }
};


#endif
