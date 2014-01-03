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
SQLResultObject& SQLExecuter::execute(const SQLQueryObject& query)
{
    MyTimer timer;
    
    //std::cout << query._query << std::endl;
    
    if (query.hasError) {
        return *new SQLResultObject(MyString("Syntax error"));
    }
    
    switch (query._int.find(SQLConstants::TOKEN_OPERATION)->second)
    {
        case SQLConstants::OPERATION_CREATE_TABLE:
        {
            MyString tableName = query._str.find(SQLConstants::TOKEN_TABLE_NAME)->second;
            bool existance = _storage.tableExists(tableName);
            
            if (existance) {
                // TABLE already exists
                return *new SQLResultObject(MyString("Table [").concat(tableName).concat("] already exists"));
            }
            
            // create table
            _storage.createTable(tableName);
            
            // create columns
            for (auto it = query._create_table_columns.begin(); it != query._create_table_columns.end(); ++it) {
                auto &_col = *it;
                bool result;
                result = _storage[tableName].createColumn(_col.name, _col.type/*, _col.size, _col.can_null*/);
                
                if (!result) {
                    return *new SQLResultObject(MyString("Column [").concat(_col.name).concat("] duplicate in table [").concat(tableName).concat("]"));
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
                return *new SQLResultObject(MyString("Table [").concat(tableName).concat("] not exists"));
            }
            
            SQLTable &table = _storage[tableName];
            
            // 简易优化WHERE条件
            bool has_condition = (query._where_statements.size() > 0);
            CompiledSQLConditionObject condition;
            if (has_condition) {
                if (!query.checkCondition(table)) {
                    return *new SQLResultObject(MyString("Condition column not found"));
                }
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
                return *new SQLResultObject(MyString("Table [").concat(tableName).concat("] not exists"));
            }
            
            SQLTable &table = _storage[tableName];
            
            // 优化WHERE
            bool has_condition = (query._where_statements.size() > 0);
            CompiledSQLConditionObject condition;
            if (has_condition) {
                if (!query.checkCondition(table)) {
                    return *new SQLResultObject(MyString("Condition column not found"));
                }
                condition = query.compileCondition(table);
            }
            
            // 优化SELECT COLUMN
            CompiledSQLFilterObject filter;
            if (!query.checkFilter(table)) {
                return *new SQLResultObject(MyString("Invalid filtering column"));
            }
            filter = query.compileFilter(table);
            // 补充COLUMN TYPES
            std::vector<int> filter_types;
            if (filter.wild) {
                for (auto it = table.head.begin(); it != table.head.end(); ++it) {
                    filter_types.push_back((*it).type);
                }
            } else {
                for (auto it = filter.columns.begin(); it != filter.columns.end(); ++it) {
                    filter_types.push_back(table.head[(*it)].type);
                }
            }
            
            // 检查ORDER BY
            int order_col = -1, order_col_type = -1;
            int order_order = -1;
            if (query._str.find(SQLConstants::TOKEN_COLUMN_NAME) != query._str.end()) {
                order_col = table.getColumnIndexByName(query._str.find(SQLConstants::TOKEN_COLUMN_NAME)->second);
                if (order_col == -1) {
                    return *new SQLResultObject(MyString("Invalid ORDER column"));
                }
                
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
            
            // filter and top
            std::list<SQLTableRow> result_final;
            
            for (auto it = result.begin(); it != result.end(); ++it) {
                result_final.push_back(filter.filter((*it)));
                if (--top_limit == 0) {
                    break;
                }
            }
            
            result.clear();
            
            // return final result
            return *new SQLResultObject(timer.elapsed(), tableName, result_final, filter_types);
            
            break;
        }
        case SQLConstants::OPERATION_UPDATE:
        {
            MyString tableName = query._str.find(SQLConstants::TOKEN_TABLE_NAME)->second;
            bool existance = _storage.tableExists(tableName);
            
            if (!existance) {
                return *new SQLResultObject(MyString("Table [").concat(tableName).concat("] not exists"));
            }
            
            SQLTable &table = _storage[tableName];
            
            // 简易优化WHERE条件
            bool has_condition = (query._where_statements.size() > 0);
            CompiledSQLConditionObject condition;
            if (has_condition) {
                if (!query.checkCondition(table)) {
                    return *new SQLResultObject(MyString("Condition column not found"));
                }
                condition = query.compileCondition(table);
            }
            
            // 优化 SET COLUMN
            int set_col = -1, set_col_type = -1;
            float set_col_v_f;
            MyString set_col_v_s;
            
            set_col = table.getColumnIndexByName(query._str.find(SQLConstants::TOKEN_COLUMN_NAME)->second);
            if (set_col == -1) {
                return *new SQLResultObject(MyString("Invalid ORDER column"));
            }
            set_col_type = table.head[set_col].type;
            
            if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                set_col_v_f = query._str.find(SQLConstants::TOKEN_COLUMN_VALUE)->second.toFloat();
            } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                set_col_v_s = query._str.find(SQLConstants::TOKEN_COLUMN_VALUE)->second;
            }
            
            std::list<SQLTableRow> result;
            std::vector<int> filter_types;
            
            for (auto it = table.head.begin(); it != table.head.end(); ++it) {
                filter_types.push_back((*it).type);
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
                            result.push_back(_row);
                            affected_rows++;
                        }
                    }
                } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        auto &_row = *it;
                        if (condition.test(_row)) {
                            _row.cols[set_col]._v_s = set_col_v_s;
                            result.push_back(_row);
                            affected_rows++;
                        }
                    }
                }
            } else {
                if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        (*it).cols[set_col]._v_f = set_col_v_f;
                        result.push_back((*it));
                        affected_rows++;
                    }
                } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        (*it).cols[set_col]._v_s = set_col_v_s;
                        result.push_back((*it));
                        affected_rows++;
                    }
                }
            }
            
            return *new SQLResultObject(timer.elapsed(), tableName, result, filter_types);
            
            break;
        }
    }
    
    return *new SQLResultObject(MyString("Unknown operation"));
}

/*
 从文件导入数据
 */
SQLResultObject& SQLExecuter::import(const char *table, const char *filepath)
{
    return import(MyString(table), filepath);
}

SQLResultObject& SQLExecuter::import(const MyString& table, const char *filepath)
{
    MyTimer timer;
    
    MyString table_ = table.toUpper();
    
    bool exists = _storage.tableExists(table_);
    if (!exists) {
        return *new SQLResultObject(MyString("Table [").concat(table_).concat("] not exists"));
    }
    
    std::ifstream fin;
    fin.open(filepath);
    int n = _storage[table_].import(fin);
    fin.close();
    
    return *new SQLResultObject(timer.elapsed(), table_, n);
}

/*
 导出数据到文件
 */
void SQLExecuter::xport(const char *table, const char *filepath)
{
    xport(MyString(table), filepath);
}

void SQLExecuter::xport(const MyString& table, const MyString& filepath)
{
    char *fp = filepath.toCString();
    xport(table, fp);
    delete[] fp;
}

void SQLExecuter::xport(const MyString& table, const char *filepath)
{
    std::ofstream fout;
    fout.open(filepath);
    _storage[table].print(fout);
    fout.close();
}