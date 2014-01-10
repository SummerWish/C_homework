//
//  SQLExecuter.cpp
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQLExecuter.h"

/*
 给定一个条件，返回一个包含了所有符合条件的行枚举器的列表
 该函数会尝试使用索引
 
 如果给定了排序主键，则会对结果进行排序
 注，在可以被索引优化情况下，不会对结果进行排序
 */
SQLSelectResultObject& getTargetRowIterators(SQLTable& table, CompiledSQLConditionObject& condition, int order_col, int order)
{
    MyTimer timer;
    int scanned_rows = 0;
    auto *desired_rows = new std::list<std::list<SQLTableRow>::iterator>();
    
    auto index_stat = condition.statIndex();
    
    if (!index_stat.can_optimize) {
        
        // 查询无法进行索引优化
        
        if (order_col == -1) {
            // no index can be used, enum all rows and test conditions
            for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                scanned_rows++;
                if (condition.test(*it)) {
                    desired_rows->push_back(it);
                }
            }
        } else {
            // order optimize
            if (order == SQLConstants::ORDER_ASC) {
                //asc
                if (table.head[order_col].type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    auto& idx = table.indexes[order_col]._m_f;
                    for (auto it = idx.begin(); it != idx.end(); ++it) {
                        scanned_rows++;
                        if (condition.test(*it->second)) {
                            desired_rows->push_back(it->second);
                        }
                    }
                } else {
                    auto& idx = table.indexes[order_col]._m_s;
                    for (auto it = idx.begin(); it != idx.end(); ++it) {
                        scanned_rows++;
                        if (condition.test(*it->second)) {
                            desired_rows->push_back(it->second);
                        }
                    }
                }
            } else {
                //desc
                if (table.head[order_col].type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    auto& idx = table.indexes[order_col]._m_f;
                    for (auto it = idx.begin(); it != idx.end(); ++it) {
                        scanned_rows++;
                        if (condition.test(*it->second)) {
                            desired_rows->push_front(it->second);
                        }
                    }
                } else {
                    auto& idx = table.indexes[order_col]._m_s;
                    for (auto it = idx.begin(); it != idx.end(); ++it) {
                        scanned_rows++;
                        if (condition.test(*it->second)) {
                            desired_rows->push_front(it->second);
                        }
                    }
                }
            }
        }
        
    } else {
        
        // 可以使用索引优化，则使用索引
        
        if (table.head[index_stat.row_index].type == SQLConstants::COLUMN_TYPE_FLOAT) {
            
            auto range = table.indexes[index_stat.row_index]._m_f.equal_range(index_stat._v_f);
            
            if (index_stat.only_one) {
                for (auto _it = range.first; _it != range.second; ++_it) {
                    scanned_rows++;
                    desired_rows->push_back(_it->second);
                }
            } else {
                for (auto _it = range.first; _it != range.second; ++_it) {
                    scanned_rows++;
                    auto& it = _it->second;
                    if (condition.test(*it)) {
                        desired_rows->push_back(it);
                    }
                }
            }
            
        } else if (table.head[index_stat.row_index].type == SQLConstants::COLUMN_TYPE_CHAR) {
            
            auto range = table.indexes[index_stat.row_index]._m_s.equal_range(index_stat._v_s);
            
            if (index_stat.only_one) {
                for (auto _it = range.first; _it != range.second; ++_it) {
                    scanned_rows++;
                    desired_rows->push_back(_it->second);
                }
            } else {
                for (auto _it = range.first; _it != range.second; ++_it) {
                    scanned_rows++;
                    auto& it = _it->second;
                    if (condition.test(*it)) {
                        desired_rows->push_back(it);
                    }
                }
            }
            
        }
        
        // 排序
        // TODO: 优化: 使用排序列进行搜索索引，可避免排序
        if (order_col > -1) {
            if (order == SQLConstants::ORDER_ASC) {
                if (table.head[order_col].type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    desired_rows->sort([order_col](const std::list<SQLTableRow>::iterator& r1, const std::list<SQLTableRow>::iterator& r2) -> bool
                                      {
                                          return (r1->cols[order_col]._v_f < r2->cols[order_col]._v_f);
                                      });
                } else if (table.head[order_col].type == SQLConstants::COLUMN_TYPE_CHAR) {
                    desired_rows->sort([order_col](const std::list<SQLTableRow>::iterator& r1, const std::list<SQLTableRow>::iterator& r2) -> bool
                                      {
                                          return (r1->cols[order_col]._v_s < r2->cols[order_col]._v_s);
                                      });
                }
            } else if (order == SQLConstants::ORDER_DESC) {
                if (table.head[order_col].type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    desired_rows->sort([order_col](const std::list<SQLTableRow>::iterator& r1, const std::list<SQLTableRow>::iterator& r2) -> bool
                                      {
                                          return (r1->cols[order_col]._v_f > r2->cols[order_col]._v_f);
                                      });
                } else if (table.head[order_col].type == SQLConstants::COLUMN_TYPE_CHAR) {
                    desired_rows->sort([order_col](const std::list<SQLTableRow>::iterator& r1, const std::list<SQLTableRow>::iterator& r2) -> bool
                                      {
                                          return (r1->cols[order_col]._v_s > r2->cols[order_col]._v_s);
                                      });
                }
            }
        }
        
    }
    
    return *(new SQLSelectResultObject(timer.elapsed(), scanned_rows, desired_rows));
}

SQLSelectResultObject& getTargetRowIterators(SQLTable& table, CompiledSQLConditionObject& condition)
{
    return getTargetRowIterators(table, condition, -1, -1);
}

/*
 删除一行，同时删除索引
 */
std::list<SQLTableRow>::iterator removeRow(SQLTable& table, std::list<SQLTableRow>::iterator& it)
{
    auto& row = *it;
    
    // remove index
    for (int i = 0; i < table.head.size(); ++i) {
        
        int& type = table.head[i].type;
        
        if (type == SQLConstants::COLUMN_TYPE_FLOAT) {
            
            auto& idx = table.indexes[i]._m_f;
            auto range = idx.equal_range(row.cols[i]._v_f);
            
            for (auto _it = range.first; _it != range.second; ++_it) {
                if (_it->second == it) {
                    idx.erase(_it);
                    break;
                }
            }
            
        } else if (type == SQLConstants::COLUMN_TYPE_CHAR) {
            
            auto& idx = table.indexes[i]._m_s;
            auto range = idx.equal_range(row.cols[i]._v_s);
            
            for (auto _it = range.first; _it != range.second; ++_it) {
                if (_it->second == it) {
                    idx.erase(_it);
                    break;
                }
            }
            
            
        }
        
    }
    
    // remove row
    return table.rows.erase(it);
}

/*
 删除所有行，并删除所有索引
 */
void removeAllRows(SQLTable& table)
{
    table.rows.clear();
    
    for (auto it = table.indexes.begin(); it != table.indexes.end(); ++it) {
        it->_m_f.clear();
        it->_m_s.clear();
    }
}

/*
 更新行，并更新索引
 */
void updateRow(SQLTable& table, std::list<SQLTableRow>::iterator& _it, int column, float new_value)
{
    auto& row = *_it;
    
    // update value
    float old_value = row.cols[column]._v_f;
    row.cols[column]._v_f = new_value;
    
    // update index
    auto& idx = table.indexes[column]._m_f;
    auto range = idx.equal_range(old_value);
    
    // remove old index
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == _it) {
            idx.erase(it);
            break;
        }
    }
    
    // insert new index
    idx.insert(std::make_pair(new_value, _it));
}

void updateRow(SQLTable& table, std::list<SQLTableRow>::iterator& _it, int column, MyString& new_value)
{
    auto& row = *_it;
    
    // update value
    MyString old_value = row.cols[column]._v_s;
    row.cols[column]._v_s = new_value;
    
    // update index
    auto& idx = table.indexes[column]._m_s;
    auto range = idx.equal_range(old_value);
    
    // remove old index
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == _it) {
            idx.erase(it);
            break;
        }
    }
    
    // insert new index
    idx.insert(std::make_pair(new_value, _it));
}

/*
 重建一个列的索引为某个值
 */
void regenerateIndex(SQLTable& table, int column, float new_value)
{
    table.indexes[column]._m_f.clear();
    
    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
        table.indexes[column]._m_f.insert(std::make_pair(new_value, it));
    }
}

void regenerateIndex(SQLTable& table, int column, MyString& new_value)
{
    table.indexes[column]._m_s.clear();
    
    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
        table.indexes[column]._m_s.insert(std::make_pair(new_value, it));
    }
}

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
            
            return *new SQLResultObject(timer.elapsed());
            
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
            int scanned_rows = 0;
            
            // delete rows
            if (has_condition) {

                auto _r = getTargetRowIterators(table, condition);
                auto rows_to_delete = _r.desired_rows;
                
                for (auto it = rows_to_delete->begin(); it != rows_to_delete->end(); ++it) {
                    removeRow(table, *it);
                    affected_rows++;
                }
                
                scanned_rows = _r.n;

            } else {

                affected_rows = (int)table.rows.size();
                removeAllRows(table);
                
                scanned_rows = affected_rows;
                
            }
            
            //TODO: not eligant here
            auto ret = new SQLResultObject(timer.elapsed(), affected_rows, scanned_rows);
            ret->table = new SQLTable(tableName);
            
            return *ret;
            
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
            
            int scanned_rows = 0;
            
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
            SQLTable *result_table = new SQLTable(tableName);
            if (filter.wild) {
                for (auto it = table.head.begin(); it != table.head.end(); ++it) {
                    result_table->createColumn(*it);
                }
            } else {
                for (auto it = filter.columns.begin(); it != filter.columns.end(); ++it) {
                    result_table->createColumn(table.head[(*it)]);
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
                return *new SQLResultObject(timer.elapsed());
            }
            
            // begin selecting rows
            std::list<SQLTableRow> result;
            
            if (has_condition) {

                auto _r = getTargetRowIterators(table, condition, order_col, order_order);
                auto rows_to_select = _r.desired_rows;
                
                for (auto it = rows_to_select->begin(); it != rows_to_select->end(); ++it) {
                    result.push_back(**it);
                }
                
                scanned_rows = _r.n;
                
            } else {

                if (order_col == -1) {
                    
                    result = std::list<SQLTableRow>(table.rows.begin(), table.rows.end());
                    scanned_rows = (int)result.size();
                    
                } else {
                    
                    //如果没有WHERE，则直接使用排序索引
                    auto _r = getTargetRowIterators(table, condition, order_col, order_order);
                    auto rows_to_select = _r.desired_rows;
                    
                    for (auto it = rows_to_select->begin(); it != rows_to_select->end(); ++it) {
                        result.push_back(**it);
                    }
                    
                    scanned_rows = _r.n;

                }
                
                
            }

            // filter and top, push data
            for (auto it = result.begin(); it != result.end(); ++it) {
                result_table->rows.push_back(filter.filter(*it));
                if (--top_limit == 0) {
                    break;
                }
            }
            
            result.clear();
            
            // return final result
            return *new SQLResultObject(timer.elapsed(), result_table, scanned_rows);
            
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
            
            int scanned_rows;
            
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
            SQLTable *result_table = new SQLTable(tableName);
            for (auto it = table.head.begin(); it != table.head.end(); ++it) {
                result_table->createColumn(*it);
            }
            
            // begin updating
            // optimize: test condition outside loops
            int affected_rows = 0;

            if (has_condition) {

                // update specific index when doing selective update
                
                auto _r = getTargetRowIterators(table, condition);
                auto rows_to_update = _r.desired_rows;
                
                // update value and index
                if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {
                    for (auto it = rows_to_update->begin(); it != rows_to_update->end(); ++it) {
                        updateRow(table, *it, set_col, set_col_v_f);
                        result_table->rows.push_back(**it);
                        affected_rows++;
                    }
                } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {
                    for (auto it = rows_to_update->begin(); it != rows_to_update->end(); ++it) {
                        updateRow(table, *it, set_col, set_col_v_s);
                        result_table->rows.push_back(**it);
                        affected_rows++;
                    }
                }
                
                scanned_rows = _r.n;

            } else {

                // optimize: regenerate index when doing whole update

                if (set_col_type == SQLConstants::COLUMN_TYPE_FLOAT) {

                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        it->cols[set_col]._v_f = set_col_v_f;
                        result_table->rows.push_back(*it);
                        affected_rows++;
                    }

                    regenerateIndex(table, set_col, set_col_v_f);

                } else if (set_col_type == SQLConstants::COLUMN_TYPE_CHAR) {

                    for (auto it = table.rows.begin(); it != table.rows.end(); ++it) {
                        it->cols[set_col]._v_s = set_col_v_s;
                        result_table->rows.push_back(*it);
                        affected_rows++;
                    }

                    regenerateIndex(table, set_col, set_col_v_s);
                }
                
                scanned_rows = affected_rows;

            }
            
            return *new SQLResultObject(timer.elapsed(), result_table, scanned_rows);
            
            break;
        }
    }
    
    throw MyString("Unknown operation");
}