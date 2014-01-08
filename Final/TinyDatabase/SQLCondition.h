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
#include "MyString.h"

class SQLIndexStatObject
{
public:
    bool can_optimize;
    bool only_one;
    int row_index;
    float _v_f;
    MyString _v_s;

    SQLIndexStatObject()
    {
        can_optimize = false;
        only_one = false;
    }

    SQLIndexStatObject(int row, bool one, float value)
    {
        can_optimize = true;
        only_one = one;
        row_index = row;
        _v_f = value;
    }

    SQLIndexStatObject(int row, bool one, const MyString& value)
    {
        can_optimize = true;
        only_one = one;
        row_index = row;
        _v_s = value;
    }

};

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
     判断该条件是否是一个可以被索引加速的条件，并返回可以被使用索引的列下标
     如果无法被索引加速，则返回 -1

     注：目前暂时只支持单条件索引优化
     */
    SQLIndexStatObject statIndex() const
    {

        if (condition_components.size() == 0) {
            return SQLIndexStatObject();
        }
        
        int res_count = 0;
        int res_rowIndex;
        int res_rowType;
        float res_v_f;
        MyString res_v_s;

        for (auto it = condition_components.begin(); it != condition_components.end(); ++it) {
            auto& component = *it;
            if (component.type == SQLConstants::WHERE_COMPONENT_STATEMENT) {
                auto& statement = component.statement;
                if (statement.op == SQLConstants::WHERE_STATEMENT_EQUAL) {
                    res_count++;
                    if (res_count == 1) {
                        res_rowType = statement.rowType;
                        res_rowIndex = statement.rowIndex;
                        if (statement.rowType == SQLConstants::COLUMN_TYPE_CHAR) {
                            res_v_s = statement.value;
                        } else {
                            res_v_f = statement.value.toFloat();
                        }
                    }
                } else {
                    return SQLIndexStatObject();
                }
            }
        }
        
        if (res_count == 0) {
            return SQLIndexStatObject();
        } else {
            if (res_rowType == SQLConstants::COLUMN_TYPE_CHAR) {
                return SQLIndexStatObject(res_rowIndex, res_count == 1, res_v_s);
            } else {
                return SQLIndexStatObject(res_rowIndex, res_count == 1, res_v_f);
            }
        }
    }

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
            switch (it->type) {
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
                    result.push(testStatement(row, it->statement));
                    break;
                }
            }
        }
        
        return result.top();
    }
};

#endif