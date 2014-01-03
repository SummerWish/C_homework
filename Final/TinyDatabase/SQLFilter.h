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
#include "SQLTableCell.h"
#include "SQLTableRow.h"

class CompiledSQLFilterObject
{
public:
    bool wild;
    std::set<int> columns;
    
    /*
     给定一个数据行，返回其中某些列
     */
    SQLTableRow& filter(SQLTableRow& row) const
    {
        if (wild) {
            return row;
        }
        
        auto *ret = new SQLTableRow();
        ret->cols.reserve(columns.size());
        
        for (auto it = columns.begin(); it != columns.end(); ++it) {
            ret->cols.push_back(row.cols[(*it)]);
        }
        
        return *ret;
    }
};


#endif
