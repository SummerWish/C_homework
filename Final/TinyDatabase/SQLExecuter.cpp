//
//  SQLExecuter.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQLExecuter.h"

/*
 执行单一SQL语句
 */
SQLResultObject& SQLExecuter::execute(SQLStorage& _storage, const SQLQueryObject& query)
{
    MyTimer timer;
    
    switch (query._int.find(SQLConstants::TOKEN_OPERATION)->second)
    {
        case SQLConstants::OPERATION_CREATE_TABLE:
        {
            MyString tableName = query._str.find(SQLConstants::TOKEN_TABLE_NAME)->second;
            bool existance = _storage.tableExists(tableName);
            
            if (existance) {
                // TABLE already exists
                throw MyString("Table [").concat(tableName).concat("] already exists");
            }
            
            // create table
            _storage.createTable(tableName);
            
            // create columns
            for (auto it = query._create_table_columns.begin(); it != query._create_table_columns.end(); ++it) {
                auto &_col = *it;
                bool result;
                result = _storage[tableName].createColumn(_col.name, _col.type/*, _col.size, _col.can_null*/);
                
                if (!result) {
                    throw MyString("Column [").concat(_col.name).concat("] duplicate in table [").concat(tableName).concat("]");
                }
            }
            
            return *new SQLResultObject(timer.elapsed(), tableName);
            
            break;
        }
        case SQLConstants::OPERATION_DELETE:
        {
            MyString tableName = query._str.find(SQLConstants::TOKEN_TABLE_NAME)->second;
            bool existance = _storage.tableExists(tableName);
            
            if (!existance) {
                throw MyString("Table [").concat(tableName).concat("] does not exist");
            }
            
            SQLTable &table = _storage[tableName];
            
            // 简易优化WHERE条件
            bool has_condition = (query._where_statements.size() > 0);
            CompiledSQLConditionObject condition;
            if (has_condition) {
                query.checkCondition(table);
                condition = query.compileCondition(table);
            }
            
            int affected_rows = 0;
            
            // delete rows
            if (has_condition) {
                for (auto it = table.rows.begin(); it != table.rows.end(); ) {
                    if (condition.test((*it))) {
                        it = table.rows.erase(it);
                        affected_rows++;
                    } else {
                        it++;
                    }
                }
            } else {
                affected_rows = (int)table.rows.size();
                table.rows.clear();
            }
            
            return *new SQLResultObject(timer.elapsed(), tableName, affected_rows);
            
            break;
        }
        case SQLConstants::OPERATION_SELECT:
        {
            MyString tableName = query._str.find(SQLConstants::TOKEN_TABLE_NAME)->second;
            bool existance = _storage.tableExists(tableName);
            
            if (!existance) {
                throw MyString("Table [").concat(tableName).concat("] does not exist");
            }
            
            SQLTable &table = _storage[tableName];
            
            // 优化WHERE
            bool has_condition = (query._where_statements.size() > 0);
            CompiledSQLConditionObject condition;
            if (has_condition) {
                query.checkCondition(table);
                condition = query.compileCondition(table);
            }
            
            // 优化SELECT COLUMN
            CompiledSQLFilterObject filter;
            query.checkFilter(table);
            filter = query.compileFilter(table);
            
            // 初始化结果集
            SQLTable *result_table = new SQLTable();
            if (filter.wild) {
                for (auto it = table.head.begin(); it != table.head.end(); ++it) {
                    (*result_table).createColumn(*it);
                }
            } else {
                for (auto it = filter.columns.begin(); it != filter.columns.end(); ++it) {
                    (*result_table).createColumn(table.head[(*it)]);
                }
            }
            
            // 检查ORDER BY
            int order_col = -1, order_col_type = -1;
            int order_order = -1;
            if (query._str.find(SQLConstants::TOKEN_COLUMN_NAME) != query._str.end()) {
                order_col = table.getColumnIndexByNameWithException(query._str.find(SQLConstants::TOKEN_COLUMN_NAME)->second);
                order_col_type = table.head[order_col].type;
                
                if (query._int.find(SQLConstants::TOKEN_ORDER_ORDER) == query._int.end()) {
                    order_order = SQLConstants::ORDER_ASC;
                } else {
                    order_order = query._int.find(SQLConstants::TOKEN_ORDER_ORDER)->second;
                }
            }
            
            // 初始化TOP
            int top_limit = -1;     // 设置为负数即可不受TOP数量限制
            if (query._int.find(SQLConstants::TOKEN_TOP_VALUE) != query._int.end()) {
                top_limit = query._int.find(SQLConstants::TOKEN_TOP_VALUE)->second;
            }
            
            // TOP = 0: 返回空的列表
            if (top_limit == 0) {
                return *new SQLResultObject(timer.elapsed(), tableName);
            }
            
            // begin selecting rows
            std::list<SQLTableRow> result;
            
            if (has_condition) {
                for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                    auto &_row = *it;
                    if (condition.test(_row)) {
                        result.push_back(_row);
                    }
                }
            } else {
                result = std::list<SQLTableRow>(table.rows.begin(), table.rows.end());
            }
            
            // begin sorting
            if (order_col > -1) {
                if (order_order == SQLConstants::ORDER_ASC) {
                    if (order_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                        result.sort([order_col](const SQLTableRow& r1, const SQLTableRow& r2) -> bool
                        {
                            return (r1.cols[order_col]._v_f < r2.cols[order_col]._v_f);
                        });
                    } else if (order_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                        result.sort([order_col](const SQLTableRow& r1, const SQLTableRow& r2) -> bool
                        {
                            return (r1.cols[order_col]._v_s < r2.cols[order_col]._v_s);
                        });
                    }
                } else if (order_order == SQLConstants::ORDER_DESC) {
                    if (order_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                        result.sort([order_col](const SQLTableRow& r1, const SQLTableRow& r2) -> bool
                        {
                            return (r1.cols[order_col]._v_f > r2.cols[order_col]._v_f);
                        });
                    } else if (order_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                        result.sort([order_col](const SQLTableRow& r1, const SQLTableRow& r2) -> bool
                        {
                            return (r1.cols[order_col]._v_s > r2.cols[order_col]._v_s);
                        });
                    }
                }
            }
            
            // filter and top, push data
            for (auto it = result.begin(); it != result.end(); ++it) {
                (*result_table).rows.push_back(filter.filter(*it));
                if (--top_limit == 0) {
                    break;
                }
            }
            
            result.clear();
            
            // return final result
            return *new SQLResultObject(timer.elapsed(), tableName, result_table);
            
            break;
        }
        case SQLConstants::OPERATION_UPDATE:
        {
            MyString tableName = query._str.find(SQLConstants::TOKEN_TABLE_NAME)->second;
            bool existance = _storage.tableExists(tableName);
            
            if (!existance) {
                throw MyString("Table [").concat(tableName).concat("] does not exist");
            }
            
            SQLTable &table = _storage[tableName];
            
            // 简易优化WHERE条件
            bool has_condition = (query._where_statements.size() > 0);
            CompiledSQLConditionObject condition;
            if (has_condition) {
                query.checkCondition(table);
                condition = query.compileCondition(table);
            }
            
            // 优化 SET COLUMN
            int set_col = -1, set_col_type = -1;
            float set_col_v_f;
            MyString set_col_v_s;
            
            set_col = table.getColumnIndexByNameWithException(query._str.find(SQLConstants::TOKEN_COLUMN_NAME)->second);
            set_col_type = table.head[set_col].type;
            
            if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                set_col_v_f = query._str.find(SQLConstants::TOKEN_COLUMN_VALUE)->second.toFloat();
            } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                set_col_v_s = query._str.find(SQLConstants::TOKEN_COLUMN_VALUE)->second;
            }
            
            // 初始化结果集
            SQLTable *result_table = new SQLTable();
            for (auto it = table.head.begin(); it != table.head.end(); ++it) {
                (*result_table).createColumn(*it);
            }
            
            // begin updating
            // optimize: test condition outside loops
            int affected_rows = 0;
            
            if (has_condition) {
                if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        auto &_row = *it;
                        if (condition.test(_row)) {
                            _row.cols[set_col]._v_f = set_col_v_f;
                            (*result_table).rows.push_back(_row);
                            affected_rows++;
                        }
                    }
                } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        auto &_row = *it;
                        if (condition.test(_row)) {
                            _row.cols[set_col]._v_s = set_col_v_s;
                            (*result_table).rows.push_back(_row);
                            affected_rows++;
                        }
                    }
                }
            } else {
                if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        (*it).cols[set_col]._v_f = set_col_v_f;
                        (*result_table).rows.push_back((*it));
                        affected_rows++;
                    }
                } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        (*it).cols[set_col]._v_s = set_col_v_s;
                        (*result_table).rows.push_back((*it));
                        affected_rows++;
                    }
                }
            }
            
            return *new SQLResultObject(timer.elapsed(), tableName, result_table);
            
            break;
        }
    }
    
    throw MyString("Unknown operation");
}