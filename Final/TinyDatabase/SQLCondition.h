//
//  SQLCondition.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLCondition_h
#define _352978_SQLCondition_h

#include <stack>
#include <vector>
#include "SQLConstants.h"
#include "SQLTableRow.h"

class CompiledSQLConditionStatementObject
{
public:
    int rowIndex;
    int rowType;
    MyString value;
    int op;
};

class CompiledSQLConditionComponentObject
{
public:
    int type;
    CompiledSQLConditionStatementObject statement;
};

class CompiledSQLConditionObject
{
private:
    /*
     给定数据行和单一条件元素，计算数据行是否符合条件
     */
    bool testStatement(const SQLTableRow& row, const CompiledSQLConditionStatementObject& statement) const
    {
        auto &col = row.cols[statement.rowIndex];
        
        if (statement.rowType == SQLConstants::COLUMN_TYPE_CHAR) {
            
            MyString _v = statement.value;
            
            switch (statement.op) {
                case SQLConstants::WHERE_STATEMENT_EQUAL:
                    return col._v_s == _v;
                case SQLConstants::WHERE_STATEMENT_GT:
                    return col._v_s > _v;
                case SQLConstants::WHERE_STATEMENT_GTE:
                    return col._v_s >= _v;
                case SQLConstants::WHERE_STATEMENT_LIKE:
                    return (col._v_s.indexOf(_v) > -1);
                case SQLConstants::WHERE_STATEMENT_LT:
                    return col._v_s < _v;
                case SQLConstants::WHERE_STATEMENT_LTE:
                    return col._v_s <= _v;
                case SQLConstants::WHERE_STATEMENT_NE:
                    return col._v_s != _v;
            }
            
        } else {
            
            float _v = statement.value.toFloat();
            
            switch (statement.op) {
                case SQLConstants::WHERE_STATEMENT_EQUAL:
                    return col._v_f == _v;
                case SQLConstants::WHERE_STATEMENT_GT:
                    return col._v_f > _v;
                case SQLConstants::WHERE_STATEMENT_GTE:
                    return col._v_f >= _v;
                case SQLConstants::WHERE_STATEMENT_LIKE:
                    return (MyString(col._v_f).indexOf(MyString(_v)) > -1);
                case SQLConstants::WHERE_STATEMENT_LT:
                    return col._v_f < _v;
                case SQLConstants::WHERE_STATEMENT_LTE:
                    return col._v_f <= _v;
                case SQLConstants::WHERE_STATEMENT_NE:
                    return col._v_f != _v;
            }
        }
        
        return false;
    }
    
public:
    std::vector<CompiledSQLConditionComponentObject> condition_components;
    
    /*
     给定数据行，计算该行是否满足条件
     */
    bool test(const SQLTableRow& row) const
    {
        std::stack<bool> result;
        
        if (condition_components.size() == 0) {
            return true;
        }
        
        for (auto it = condition_components.begin(); it != condition_components.end(); ++it) {
            switch ((*it).type) {
                case SQLConstants::WHERE_COMPONENT_AND:
                {
                    bool a2 = result.top(); result.pop();
                    bool a1 = result.top(); result.pop();
                    result.push(a1 && a2);
                    break;
                }
                case SQLConstants::WHERE_COMPONENT_OR:
                {
                    bool a2 = result.top(); result.pop();
                    bool a1 = result.top(); result.pop();
                    result.push(a1 || a2);
                    break;
                }
                default: //WHERE_COMPONENT_STATEMENT
                {
                    result.push(testStatement(row, (*it).statement));
                    break;
                }
            }
        }
        
        return result.top();
    }
};

#endif