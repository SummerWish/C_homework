//
//  SQLQueryObject.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQLQueryObject.h"
#include <stack>

SQLWhereComponentObject::SQLWhereComponentObject(int _type)
{
    type = _type;
}

SQLWhereComponentObject::SQLWhereComponentObject(const SQLWhereStatementObject _statement)
{
    type = SQLConstants::WHERE_COMPONENT_STATEMENT;
    statement = _statement;
}

SQLQueryObject::SQLQueryObject()
{
    hasError = false;
}

SQLQueryObject::SQLQueryObject(const MyString& query)
{
    hasError = false;
    _query = query;
}

int SQLQueryObject::OperationStringToToken(const MyString& s) const
{
    const MyString _s = s.toUpper();
    if (_s == "CREATE") {
        return SQLConstants::OPERATION_CREATE_TABLE;
    } else if (_s == "SELECT") {
        return SQLConstants::OPERATION_SELECT;
    } else if (_s == "UPDATE") {
        return SQLConstants::OPERATION_UPDATE;
    } else if (_s == "DELETE") {
        return SQLConstants::OPERATION_DELETE;
    } else {
        return SQLConstants::OPERATION_UNDEFINED;
    }
}

int SQLQueryObject::ColumnTypeToInt(const MyString& s) const
{
    const MyString _s = s.toUpper();
    if (_s == "FLOAT") {
        return SQLConstants::COLUMN_TYPE_FLOAT;
    } else if (_s == "CHAR") {
        return SQLConstants::COLUMN_TYPE_CHAR;
    } else {
        return SQLConstants::COLUMN_TYPE_UNDEFINED;
    }
}

/*
// DEBUG only
void SQLQueryObject::_print() const
{
    SQLConstants c;
    
    std::cout << ":: " << _query << std::endl;
    
    std::cout << ">> Properties:" << std::endl;
    std::cout << std::left;
    
    {
        if (_int.size() > 0) {
            for (auto it = _int.begin(); it != _int.end(); ++it) {
                if (it->first == SQLConstants::TOKEN_TOP_VALUE) {
                    std::cout << std::setw(20) << c._constToString(it->first) << it->second << std::endl;
                } else {
                    std::cout << std::setw(20) << c._constToString(it->first) << c._constToString(it->second) << std::endl;
                }
            }
        }
    }
    
    {
        if (_str.size() > 0) {
            for (auto it = _str.begin(); it != _str.end(); ++it) {
                std::cout << std::setw(20) << c._constToString(it->first) << it->second << std::endl;
            }
        }
    }
    
    {
        if (_select_columns.size() > 0) {
            std::cout << ">> Selected columns:" << std::endl;
            
            std::cout << std::left;
            std::cout << std::setw(5) << "idx" << "value" << std::endl;
            
            for (int i = 0; i < _select_columns.size(); ++i) {
                std::cout << std::setw(5) << i << _select_columns[i] << std::endl;
            }
        }
    }
    
    {
        if (_where_statements.size() > 0) {
            std::cout << ">> Condition:" << std::endl;
            
            for (int i = 0; i < _where_statements.size(); ++i) {
                if (_where_statements[i].type == SQLConstants::WHERE_COMPONENT_STATEMENT) {
                    std::cout
                    << _where_statements[i].statement.name << " "
                    << c._constToString(_where_statements[i].statement.op) << " "
                    << _where_statements[i].statement.value << " ";
                } else {
                    std::cout << c._constToString(_where_statements[i].type) << " ";
                }
            }
            
            std::cout << std::endl;
        }
    }
    
    {
        if (_create_table_columns.size() > 0) {
            std::cout << ">> Create columns:" << std::endl;
            
            std::cout << std::left;
            std::cout << std::setw(5) << "idx" << std::setw(10) << "NAME" << std::setw(10) << "TYPE" << std::setw(5) << "SIZE" << "CAN_NULL" << std::endl;
            
            for (int i = 0; i < _create_table_columns.size(); ++i) {
                std::cout << std::setw(5) << i << std::setw(10) << _create_table_columns[i].name << std::setw(10) << c._constToString(_create_table_columns[i].type) << std::setw(5) << _create_table_columns[i].size << _create_table_columns[i].can_null << std::endl;
            }
        }
    }
    
    std::cout << "==================================" << std::endl;
    
}
*/

bool SQLQueryObject::checkCondition(const SQLTable& table) const
{
    for (int i = 0; i < _where_statements.size(); ++i) {
        if (_where_statements[i].type == SQLConstants::WHERE_COMPONENT_STATEMENT) {
            if (table.getColumnIndexByName(_where_statements[i].statement.name) == -1) {
                return false;
            }
        }
    }
    
    return true;
}

bool isOperator(CompiledSQLConditionComponentObject t)
{
    if (t.type == SQLConstants::WHERE_COMPONENT_AND || t.type == SQLConstants::WHERE_COMPONENT_OR) {
        return true;
    } else {
        return false;
    }
}

int getPriority(CompiledSQLConditionComponentObject t)
{
    if (t.type == SQLConstants::WHERE_COMPONENT_AND) {
        return 2;
    } else if (t.type == SQLConstants::WHERE_COMPONENT_OR) {
        return 1;
    } else {
        return 0;
    }
}

/*
 将 COLUMN_NAME 转换为 index，并转换为后缀表达式
 */
CompiledSQLConditionObject& SQLQueryObject::compileCondition(const SQLTable& table) const
{
    // Convert to column index
    std::vector<CompiledSQLConditionComponentObject> infix;
    
    for (auto it = _where_statements.begin(); it != _where_statements.end(); ++it) {
        auto &component = *it;
        
        CompiledSQLConditionComponentObject t;
        
        t.type = component.type;
        if (component.type == SQLConstants::WHERE_COMPONENT_STATEMENT) {
            t.statement.rowIndex = table.getColumnIndexByName(component.statement.name);
            t.statement.rowType = table.head[t.statement.rowIndex].type;
            t.statement.op = component.statement.op;
            t.statement.value = component.statement.value;
        }
        
        infix.push_back(t);
    }
    
    // Calculate postfix
    std::vector<CompiledSQLConditionComponentObject> postfix;
    std::stack<CompiledSQLConditionComponentObject> operator_stack;
    
    for (auto it = infix.begin(); it != infix.end(); ++it) {
        auto &p = *it;
        
        if (isOperator(p)) {
            while (!operator_stack.empty() && isOperator(operator_stack.top()) && getPriority(operator_stack.top()) >= getPriority(p)) {
                postfix.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(p);
        } else if (p.type == SQLConstants::WHERE_COMPONENT_BEGIN) {
            operator_stack.push(p);
        } else if (p.type == SQLConstants::WHERE_COMPONENT_END) {
            while (operator_stack.top().type != SQLConstants::WHERE_COMPONENT_BEGIN)
            {
                postfix.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.pop();
        } else {
            postfix.push_back(p);
        }
    }
    
    while (!operator_stack.empty()) {
        postfix.push_back(operator_stack.top());
        operator_stack.pop();
    }
    
    //postfix.pop_back();
    
    auto *ret = new CompiledSQLConditionObject;
    ret->condition_components = postfix;
    
    return *ret;
}

bool SQLQueryObject::checkFilter(const SQLTable& table) const
{
    bool has_wildcard = false;
    
    if (_select_columns.size() == 0) {
        return false;
    }
    
    for (int i = 0; i < _select_columns.size(); ++i) {
        if (_select_columns[i] == "*") {
            has_wildcard = true;
            break;
        }
    }
    
    if (has_wildcard && _select_columns.size() > 1) {
        return false;
    }
    
    if (has_wildcard) {
        return true;
    }
    
    for (int i = 0; i < _select_columns.size(); ++i) {
        if (table.getColumnIndexByName(_select_columns[i]) == -1) {
            return false;
        }
    }
    
    return true;
}

/*
 去除重复的 COLUMN_NAME，并将 COLUMN_NAME 转换为 index
 */
CompiledSQLFilterObject& SQLQueryObject::compileFilter(const SQLTable& table) const
{
    std::set<int> filter;
    auto *ret = new CompiledSQLFilterObject;
    
    if (_select_columns[0] == "*") {
        ret->wild = true;
        return *ret;
    }
    
    ret->wild = false;
    
    for (int i = 0; i < _select_columns.size(); ++i) {
        filter.insert(table.getColumnIndexByName(_select_columns[i]));
    }
    
    ret->columns = filter;
    
    return *ret;
}
