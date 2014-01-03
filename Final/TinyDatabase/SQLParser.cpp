//
//  SQLParser.cpp
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "SQLParser.h"

/*
 解析一行SQL语句，转换为SQLQueryObject
 */
SQLQueryObject& SQLParser::parseLine(const char *line)
{
    return parseLine(MyString(line));
}

SQLQueryObject& SQLParser::parseLine(const MyString& line)
{
    // maybe better or worse method: RegExp
    
    auto *query = new SQLQueryObject(line);
    
    int pos = 0, pos_max = line.length();
    char ch;
    int status = SQLParser::PARSE_STATUS_TOKEN;
    bool expect_parenthesis_group = false, in_parenthesis = false;
    bool expect_string = false, in_string = false, in_escape = false;
    bool syntax_error = false;
    bool parse_end = false;
    int parenthesis = 0;
    int token = SQLConstants::TOKEN_NULL, subtoken = SQLConstants::TOKEN_NULL;
    int _opToken = SQLConstants::OPERATION_UNDEFINED;
    SQLCreateTableColumnObject _create_table_column;
    SQLWhereStatementObject _where_statement;
    
    std::vector<char> _value;
    std::vector<char> t;
    
    //TODO: select * from student where id > 1 and name xx b
    
    while (pos <= pos_max) {
        
        if (syntax_error) {
            // 有语法错误则中断解析
            break;
        }
        
        if (parse_end) {
            // 预期内容解析完毕了，则简单粗暴地中断后面所有内容的解析
            break;
        }
        
        ch = line[pos];
        
        // 暂时只支持双引号包裹的字符串
        // 对于字符串特殊处理
        if (in_string && ch != '\"' && ch != '\\' && ch != '\0') {
            if (in_escape) {
                // \a，\b，\c等处理为a，b，c
                // 只需要撤销转义标记
                in_escape = false;
            }
            
            _value.push_back(ch);
            pos++;
            
            continue;
        }
        
        switch (ch) {
            case '\\':
                
                if (in_string) {
                    // 处理转义符
                    if (in_escape) {
                        in_escape = false;
                        _value.push_back('\\');
                        break;
                    } else {
                        in_escape = true;
                        break;
                    }
                } else {
                    // 不在字符串中，那这是神马符号?
                    syntax_error = true;
                    throw MyString("[!E01] Syntax error at ").concat(pos);
                    break;
                }
                
                break;
                
            case '"':
                
                if (expect_string) {
                    
                    expect_string = false;
                    if (
                        token == SQLConstants::TOKEN_COLUMN_NAME ||
                        (token == SQLConstants::TOKEN_WHERE && subtoken == SQLConstants::TOKEN_COLUMN_NAME)
                        ) {
                        // UPDATE TABLE_NAME SET COLUMN_NAME = COLUMN_VALUE
                        // UPDATE TABLE_NAME SET COLUMN_NAME = _"_COLUMN_VALUE" // we are here
                        // WHERE COLUMN_NAME ? _"_COLUMN_VALUE"                 // or we are here
                        in_string = true;
                        _value.clear();
                        break;
                    }
                } else if (in_string) {
                    if (in_escape) {
                        // 在转义符号内
                        in_escape = false;
                        _value.push_back('\"');
                        break;
                    } else {
                        // 不在转义符号内：字符串结束
                        in_string = false;
                        
                        if (token == SQLConstants::TOKEN_WHERE) {
                            // WHERE COLUMN_NAME ? "COLUMN_VALUE_"_ // we are here
                            subtoken = SQLConstants::TOKEN_COLUMN_VALUE;
                            _where_statement.value = MyString(_value);
                            query->_where_statements.push_back(SQLWhereComponentObject(_where_statement));
                            break;
                        } else if (_opToken == SQLConstants::OPERATION_UPDATE) {
                            // UPDATE TABLE_NAME SET COLUMN_NAME = "COLUMN_VALUE_"_ // we are here
                            token = SQLConstants::TOKEN_BEGIN_WHERE;
                            query->_str[SQLConstants::TOKEN_COLUMN_VALUE] = MyString(_value);
                            break;
                        } else {
                            syntax_error = true;
                            throw MyString("[!E01.1] Syntax error at ").concat(pos).concat(", unexpected \'\"\'");
                            break;
                        }
                    }
                }
                
                break;
                
            case '(':
                
                if (
                    token == SQLConstants::TOKEN_WHERE &&
                     (
                     subtoken == SQLConstants::TOKEN_NULL ||
                     subtoken == SQLConstants::WHERE_COMPONENT_AND ||
                     subtoken == SQLConstants::WHERE_COMPONENT_OR
                     )
                    ) {
                    
                    query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_BEGIN));
                    status = SQLParser::PARSE_STATUS_DELIM;
                    parenthesis++;
                    break;
                } else if (
                           token == SQLConstants::TOKEN_WHERE &&
                           subtoken == SQLConstants::TOKEN_COLUMN_VALUE
                           ) {
                    
                    // 前面应该是 AND / OR
                    if (MyString(t).toUpper() == "AND") {
                        subtoken = SQLConstants::WHERE_COMPONENT_AND;
                        query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_AND));
                        
                        query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_BEGIN));
                        status = SQLParser::PARSE_STATUS_DELIM;
                        parenthesis++;
                        
                    } else if (MyString(t).toUpper() == "OR") {
                        subtoken = SQLConstants::WHERE_COMPONENT_OR;
                        query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_OR));
                    
                        query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_BEGIN));
                        status = SQLParser::PARSE_STATUS_DELIM;
                        parenthesis++;
                    } else {
                        syntax_error = true;
                        throw MyString("[!E02.1] Syntax error at ").concat(pos).concat(", unexpected \'(\'");
                        break;
                    }
                    
                } else if (token == SQLConstants::TOKEN_TABLE_COLUMN_NAME) {
                    _create_table_column.type = query->ColumnTypeToInt(MyString(t));
                    status = SQLParser::PARSE_STATUS_DELIM;
                    token = SQLConstants::TOKEN_TABLE_COLUMN_TYPE;
                    break;
                } else if (in_parenthesis) {
                    syntax_error = true;
                    throw MyString("[!E02.2] Syntax error at ").concat(pos).concat(", \')\' expected");
                    break;
                } else {
                    if (expect_parenthesis_group) {
                        expect_parenthesis_group = false;
                        in_parenthesis = true;
                        status = SQLParser::PARSE_STATUS_DELIM;
                        break;
                    } else {
                        syntax_error = true;
                        throw MyString("[!E02.3] Syntax error at ").concat(pos).concat(", unexpected \'(\'");
                        break;
                    }
                }
                
                break;
            
            case ')':
                
                if (
                    token == SQLConstants::TOKEN_WHERE &&
                    subtoken == SQLConstants::TOKEN_COLUMN_VALUE &&
                    parenthesis > 0
                    ) {
                    
                    query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_END));
                    
                    status = SQLParser::PARSE_STATUS_DELIM;
                    parenthesis--;
                    break;
                } else if (
                           token == SQLConstants::TOKEN_WHERE &&
                           subtoken == SQLConstants::TOKEN_COLUMN_NAME &&
                           parenthesis > 0
                           ) {
                    
                    // 没有空格或引号直接结束 则先把上一个没处理完毕的COLUMN_VALUE处理完
                    expect_string = false;
                    subtoken = SQLConstants::TOKEN_COLUMN_VALUE;
                    _where_statement.value = MyString(t);
                    query->_where_statements.push_back(SQLWhereComponentObject(_where_statement));
                    
                    // 然后再 push ')'
                    query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_END));
                    
                    status = SQLParser::PARSE_STATUS_DELIM;
                    parenthesis--;
                    break;
                } else if (token == SQLConstants::TOKEN_TABLE_COLUMN_TYPE) {
                    _create_table_column.size = MyString(t).toInteger();
                    status = SQLParser::PARSE_STATUS_DELIM;
                    token = SQLConstants::TOKEN_TABLE_COLUMN_SIZE;
                    break;
                } else if (
                           token == SQLConstants::TOKEN_TABLE_COLUMN_NULL ||
                           token == SQLConstants::TOKEN_TABLE_BEGIN ||
                           token == SQLConstants::TOKEN_TABLE_COLUMN_NOT ||
                           token == SQLConstants::TOKEN_TABLE_COLUMN_SIZE
                           ) {
                    if (!_create_table_column.end) {
                        // 不允许没有分号来结束 CREATE_TABLE
                        syntax_error = true;
                        throw MyString("[!E03.1] Syntax error at ").concat(pos).concat(", \';\' expected");
                        break;
                    }
                    token = SQLConstants::TOKEN_TABLE_END;
                    in_parenthesis = false;
                    parse_end = true;
                    break;
                } else {
                    syntax_error = true;
                    throw MyString("[!E03.2] Syntax error at ").concat(pos).concat(", unexpected \')\'");
                    break;
                }
                
                break;
            
            case ';':
                
                if (
                    token == SQLConstants::TOKEN_TABLE_COLUMN_SIZE ||
                    token == SQLConstants::TOKEN_TABLE_COLUMN_NOT
                    ) {
                    if (MyString(t).toUpper() == "NULL") {
                        _create_table_column.can_null = (token == SQLConstants::TOKEN_TABLE_COLUMN_SIZE);
                        token = SQLConstants::TOKEN_TABLE_COLUMN_NULL;
                        //此处没有break，给下面的处理
                    } else {
                        syntax_error = true;
                        throw MyString("[!E04.1] Syntax error at ").concat(pos).concat(", expected NULL");
                        break;
                    }
                }
                
                if (token == SQLConstants::TOKEN_TABLE_COLUMN_NULL) {
                    // CREATE TABLE 一行结束
                    _create_table_column.end = true;
                    query->_create_table_columns.push_back(_create_table_column);
                    status = SQLParser::PARSE_STATUS_DELIM;
                    token = SQLConstants::TOKEN_TABLE_BEGIN;
                    break;
                } else {
                    syntax_error = true;
                    throw MyString("[!E04.2] Syntax error at ").concat(pos).concat(", unexpected \';\'");
                    break;
                }
            
            case '!':
                
                if (
                    token == SQLConstants::TOKEN_WHERE &&
                    (
                     subtoken == SQLConstants::TOKEN_NULL ||
                     subtoken == SQLConstants::WHERE_COMPONENT_AND ||
                     subtoken == SQLConstants::WHERE_COMPONENT_OR
                     )
                    ) {
                    
                    // 只有 != 这种情况
                    if (line[pos+1] == '=') {
                        status = SQLParser::PARSE_STATUS_DELIM;
                        subtoken = SQLConstants::TOKEN_COLUMN_NAME;
                        _where_statement = SQLWhereStatementObject();
                        _where_statement.name = MyString(t).toUpper();
                        _where_statement.op = SQLConstants::WHERE_STATEMENT_NE;
                        pos++;
                        expect_string = true;
                        break;
                    } else {
                        syntax_error = true;
                        throw MyString("[!E05.1] Syntax error at ").concat(pos).concat(", \'=\' expected");
                        break;
                    }
                } else {
                    syntax_error = true;
                    throw MyString("[!E05.2] Syntax error at ").concat(pos).concat(", unexpected \'<\'");
                    break;
                }
                
            case '<':
                
                if (
                    token == SQLConstants::TOKEN_WHERE &&
                    (
                     subtoken == SQLConstants::TOKEN_NULL ||
                     subtoken == SQLConstants::WHERE_COMPONENT_AND ||
                     subtoken == SQLConstants::WHERE_COMPONENT_OR
                     )
                    ) {
                    
                    status = SQLParser::PARSE_STATUS_DELIM;
                    subtoken = SQLConstants::TOKEN_COLUMN_NAME;
                    _where_statement = SQLWhereStatementObject();
                    _where_statement.name = MyString(t).toUpper();
                    
                    if (line[pos+1] == '=') {
                        // <=
                        _where_statement.op = SQLConstants::WHERE_STATEMENT_LTE;
                        pos++;
                    } else {
                        // <
                        _where_statement.op = SQLConstants::WHERE_STATEMENT_LT;
                    }
                    
                    expect_string = true;
                    break;
                    
                } else {
                    syntax_error = true;
                    throw MyString("[!E06] Syntax error at ").concat(pos).concat(", unexpected \'<\'");
                    break;
                }
                
            case '>':
                
                if (
                    token == SQLConstants::TOKEN_WHERE &&
                    (
                     subtoken == SQLConstants::TOKEN_NULL ||
                     subtoken == SQLConstants::WHERE_COMPONENT_AND ||
                     subtoken == SQLConstants::WHERE_COMPONENT_OR
                     )
                    ) {
                    
                    status = SQLParser::PARSE_STATUS_DELIM;
                    subtoken = SQLConstants::TOKEN_COLUMN_NAME;
                    _where_statement = SQLWhereStatementObject();
                    _where_statement.name = MyString(t).toUpper();
                    
                    if (line[pos+1] == '=') {
                        // >=
                        _where_statement.op = SQLConstants::WHERE_STATEMENT_GTE;
                        pos++;
                    } else {
                        // >
                        _where_statement.op = SQLConstants::WHERE_STATEMENT_GT;
                    }
                    
                    expect_string = true;
                    break;
                    
                } else {
                    syntax_error = true;
                    throw MyString("[!E07] Syntax error at ").concat(pos).concat(", unexpected \'>\'");
                    break;
                }
                
            case '=':
                
                if (
                    token == SQLConstants::TOKEN_WHERE &&
                    (
                     subtoken == SQLConstants::TOKEN_NULL ||
                     subtoken == SQLConstants::WHERE_COMPONENT_AND ||
                     subtoken == SQLConstants::WHERE_COMPONENT_OR
                     )
                    ) {
                    
                    status = SQLParser::PARSE_STATUS_DELIM;
                    subtoken = SQLConstants::TOKEN_COLUMN_NAME;
                    _where_statement = SQLWhereStatementObject();
                    _where_statement.name = MyString(t).toUpper();  //列
                    _where_statement.op = SQLConstants::WHERE_STATEMENT_EQUAL;
                    expect_string = true;   //可接字符串
                    break;
                    
                } else if (token == SQLConstants::TOKEN_SET) {
                    status = SQLParser::PARSE_STATUS_DELIM;
                    token = SQLConstants::TOKEN_COLUMN_NAME;
                    expect_string = true;   //可接字符串
                    query->_str[SQLConstants::TOKEN_COLUMN_NAME] = MyString(t).toUpper();   //列名转大写
                    break;
                } else {
                    syntax_error = true;
                    throw MyString("[!E08] Syntax error at ").concat(pos).concat(", unexpected \'=\'");
                    break;
                }
                
            case '\0':
                
                if (in_string) {
                    // 字符串未闭合就结束了
                    syntax_error = true;
                    throw MyString("[!E09] Syntax error at ").concat(pos).concat(", unclosed string");
                    break;
                }
                
                // through
            case ',':
                // through
            case ' ':
                // through
            case '\r':
                // through
            case '\n':
                // through
            case '\t':
                
                // 上一个字符已经是空格分隔符，则忽略
                if (status == SQLParser::PARSE_STATUS_DELIM) {
                    break;
                }
                
                // 上一个字符是有效的字符
                if (status == SQLParser::PARSE_STATUS_TOKEN) {
                    status = SQLParser::PARSE_STATUS_DELIM;
                    MyString _t = MyString(t);
                    
                    if (token == SQLConstants::TOKEN_NULL) {
                        // 没有上一个token，则该token为OPCODE
                        token = SQLConstants::TOKEN_OPERATION;
                        _opToken = query->OperationStringToToken(_t);
                        
                        if (_opToken == SQLConstants::OPERATION_UNDEFINED) {
                            syntax_error = true;
                            throw MyString("[!E10] Syntax error, invalid OPCODE: ").concat(_t);
                            break;
                        } else {
                            query->_int[SQLConstants::TOKEN_OPERATION] = _opToken;
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_OPERATION) {
                        // 上一个token是OPCODE
                        
                        if (_opToken == SQLConstants::OPERATION_SELECT) {
                            
                            // SELECT _[TOP N] COLUMN_NAME1 , COLUMN_NAME2 , COLUMN_NAME3_ ...
                            if (_t.toUpper() == "TOP") {
                                // 是否是 _TOP_ N
                                token = SQLConstants::TOKEN_TOP;
                                break;
                            } else if (_t.toUpper() == "FROM") {
                                // 不能是 FROM
                                syntax_error = true;
                                throw MyString("[!E11] Syntax error at ").concat(pos).concat(", COLUMN_NAME expected");
                                break;
                            } else {
                                // 解析列名
                                query->_select_columns.push_back(_t.toUpper()); //列名转大写
                                token = SQLConstants::TOKEN_COLUMN_NAME_MULTI;
                                break;
                            }
                        } else if (_opToken == SQLConstants::OPERATION_UPDATE) {
                            // UPDATE _TABLE_NAME_ SET ...
                            token = SQLConstants::TOKEN_TABLE_NAME;
                            query->_str[SQLConstants::TOKEN_TABLE_NAME] = _t.toUpper();   //表名转大写
                            break;
                        } else if (_opToken == SQLConstants::OPERATION_DELETE) {
                            // DELETE _FROM_ TABLE_NAME ...
                            if (_t.toUpper() == "FROM") {
                                token = SQLConstants::TOKEN_FROM;
                                break;
                            } else {
                                syntax_error = true;
                                throw MyString("[!E12] Syntax error at ").concat(pos).concat(", \'FROM\' expected");
                                break;
                            }
                        } else if (_opToken == SQLConstants::OPERATION_CREATE_TABLE) {
                            // CREATE _TABLE_ TABLE_NAME ...
                            if (_t.toUpper() == "TABLE") {
                                token = SQLConstants::TOKEN_CREATE_TABLE;
                                break;
                            } else {
                                syntax_error = true;
                                throw MyString("[!E13] Syntax error at ").concat(pos).concat(", \'TABLE\' expected");
                                break;
                            }
                        } else {
                            // will never reach here
                            syntax_error = true;
                            throw MyString("[!E14] WTF?");
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_TOP) {
                        // SELECT TOP _N_ COLUMN_NAME1 ...
                        if (_opToken == SQLConstants::OPERATION_SELECT) {
                            if (_t.isDigit()) {
                                token = SQLConstants::TOKEN_TOP_VALUE;
                                query->_int[SQLConstants::TOKEN_TOP_VALUE] = _t.toInteger();
                                break;
                            } else {
                                syntax_error = true;
                                throw MyString("[!E15] Syntax error at ").concat(pos).concat(", integer expected");
                                break;
                            }
                        } else {
                            // Unreachable here
                            syntax_error = true;
                            throw MyString("[!E16] Syntax error at ").concat(pos);
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_TOP_VALUE) {
                        // 上一个token是TOP N
                        // 处理COLUMN_NAME
                        query->_select_columns.push_back(_t.toUpper()); //列名转大写
                        token = SQLConstants::TOKEN_COLUMN_NAME_MULTI;
                        break;
                    } else if (token == SQLConstants::TOKEN_COLUMN_NAME) {
                        if (_opToken == SQLConstants::OPERATION_UPDATE) {
                            // UPDATE TABLE_NAME SET COLUMN_NAME = _COLUMN_VALUE_ // we are here
                            // UPDATE TABLE_NAME SET COLUMN_NAME = "COLUMN_VALUE"
                            expect_string = false;
                            token = SQLConstants::TOKEN_BEGIN_WHERE;
                            query->_str[SQLConstants::TOKEN_COLUMN_VALUE] = _t;
                            break;
                        } else if (_opToken == SQLConstants::OPERATION_SELECT) {
                            // SELECT ... ORDER BY COLUMN_NAME _ASC|DESC_
                            if (_t.toUpper() == "ASC") {
                                parse_end = true;
                                query->_int[SQLConstants::TOKEN_ORDER_ORDER] = SQLConstants::ORDER_ASC;
                                break;
                            } else if (_t.toUpper() == "DESC") {
                                parse_end = true;
                                query->_int[SQLConstants::TOKEN_ORDER_ORDER] = SQLConstants::ORDER_DESC;
                                break;
                            } else {
                                syntax_error = true;
                                throw MyString("[!E17] Syntax error at ").concat(pos).concat(", \'ASC|DESC\' expected");
                                break;
                            }
                        } else {
                            // Unreachable here
                            syntax_error = true;
                            throw MyString("[!E18] Syntax error at ").concat(pos);
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_COLUMN_NAME_MULTI) {
                        if (_t.toUpper() == "FROM") {
                            token = SQLConstants::TOKEN_FROM;
                            break;
                        } else {
                            query->_select_columns.push_back(_t.toUpper()); //列名转大写
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_FROM) {
                        if (_opToken == SQLConstants::OPERATION_SELECT) {
                            // SELECT ... FROM _TABLE_NAME_ ...
                            token = SQLConstants::TOKEN_BEGIN_WHERE_ORDER;
                            query->_str[SQLConstants::TOKEN_TABLE_NAME] = _t.toUpper();   //表名转大写
                            break;
                        } else if (_opToken == SQLConstants::OPERATION_DELETE) {
                            // DELETE FROM _TABLE_NAME_ ...
                            token = SQLConstants::TOKEN_BEGIN_WHERE;
                            query->_str[SQLConstants::TOKEN_TABLE_NAME] = _t.toUpper();   //表名转大写
                            break;
                        } else {
                            // Unreachable here
                            syntax_error = true;
                            throw MyString("[!E19] Syntax error at ").concat(pos);
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_TABLE_NAME) {
                        if (_opToken == SQLConstants::OPERATION_UPDATE) {
                            // UPDATE TABLE_NAME _SET_ ...
                            if (_t.toUpper() == "SET") {
                                token = SQLConstants::TOKEN_SET;
                                break;
                            } else {
                                syntax_error = true;
                                throw MyString("[!E20] Syntax error at ").concat(pos).concat(", \'SET\' expected");
                                break;
                            }
                        } else {
                            // Unreachable here
                            syntax_error = true;
                            throw MyString("[!E21] Syntax error at ").concat(pos);
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_SET) {
                        // SET _COLUMN_NAME_ = COLUMN_VALUE
                        // 留给 = 处理
                        break;
                    } else if (token == SQLConstants::TOKEN_END_WHERE) {
                        // SELECT ... WHERE ... [ORDER BY ...]
                        
                        if (_t.toUpper() == "ORDER") {
                            token = SQLConstants::TOKEN_ORDER;
                            break;
                        } else {
                            syntax_error = true;
                            throw MyString("[!E22] Syntax error at ").concat(pos).concat(", \'ORDER\' expected");
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_WHERE) {
                        // WHERE _........_
                        if (
                            subtoken == SQLConstants::TOKEN_NULL ||
                            subtoken == SQLConstants::WHERE_COMPONENT_AND ||
                            subtoken == SQLConstants::WHERE_COMPONENT_OR
                            ) {
                            
                            if (_t.toUpper() == "LIKE") {
                                // WHERE aaa LIKE ...
                                
                                if (_where_statement.name.length() == 0) {
                                    syntax_error = true;
                                    throw MyString("[!E22.1] Syntax error at ").concat(pos).concat(", unexpected \'LIKE\'");
                                    break;
                                } else {
                                    subtoken = SQLConstants::TOKEN_COLUMN_NAME;
                                    _where_statement.op = SQLConstants::WHERE_STATEMENT_LIKE;
                                    expect_string = true;
                                }
                                break;
                            } else {
                                // 留给 =, >, < 处理
                                _where_statement = SQLWhereStatementObject();
                                _where_statement.name = _t.toUpper();
                                break;
                            }
                        } else if (subtoken == SQLConstants::TOKEN_COLUMN_NAME) {
                            // WHERE COLUMN_NAME = _COLUMN_VALUE_ // we are here
                            expect_string = false;
                            subtoken = SQLConstants::TOKEN_COLUMN_VALUE;
                            _where_statement.value = _t;
                            query->_where_statements.push_back(SQLWhereComponentObject(_where_statement));
                            break;
                        } else if (subtoken == SQLConstants::TOKEN_COLUMN_VALUE) {
                            if (_t.toUpper() == "AND") {
                                subtoken = SQLConstants::WHERE_COMPONENT_AND;
                                query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_AND));
                                break;
                            } else if (_t.toUpper() == "OR") {
                                subtoken = SQLConstants::WHERE_COMPONENT_OR;
                                query->_where_statements.push_back(SQLWhereComponentObject(SQLConstants::WHERE_COMPONENT_OR));
                                break;
                            } else if (_t.toUpper() == "ORDER") {
                                // SELECT 中 WHERE 后面可以是 ORDER BY
                                if (_opToken == SQLConstants::OPERATION_SELECT) {
                                    if (parenthesis > 0) {
                                        // 进入 ORDER 前判断括号是否已经平衡
                                        syntax_error = true;
                                        throw MyString("[!E22.2] Syntax error at ").concat(pos).concat(", \')\' expected");
                                        break;
                                    } else {
                                        token = SQLConstants::TOKEN_ORDER;
                                        break;
                                    }
                                } else {
                                    syntax_error = true;
                                    throw MyString("[!E22.3] Syntax error at ").concat(pos).concat(", unexpected \'ORDER\'");
                                    break;
                                }
                            } else {
                                syntax_error = true;
                                throw MyString("[!E22.4] Syntax error at ").concat(pos).concat(", \'AND|OR\' expected");
                                break;
                            }
                        }
                    } else if (
                               token == SQLConstants::TOKEN_BEGIN_WHERE_ORDER ||
                               token == SQLConstants::TOKEN_BEGIN_WHERE
                               ) {
                        // ORDER BY | WHERE
                        // ... WHERE _COLUMN_NAME_ LIKE|= COLUMN_VALUE
                        if (token == SQLConstants::TOKEN_BEGIN_WHERE_ORDER && _t.toUpper() == "ORDER") {
                            token = SQLConstants::TOKEN_ORDER;
                            break;
                        } else {
                            if (_t.toUpper() != "WHERE") {
                                syntax_error = true;
                                throw MyString("[!E23] Syntax error at ").concat(pos).concat(", \'WHERE\' expected");
                                break;
                            } else {
                                token = SQLConstants::TOKEN_WHERE;
                                break;
                            }
                        }
                    } else if (token == SQLConstants::TOKEN_ORDER) {
                        if (_t.toUpper() != "BY") {
                            syntax_error = true;
                            throw MyString("[!E25] Syntax error at ").concat(pos).concat(", \'BY\' expected");
                            break;
                        } else {
                            token = SQLConstants::TOKEN_ORDER_BY;
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_ORDER_BY) {
                        // SELECT ..... ORDER BY _COLUMN_NAME_ [ASC|DESC]
                        query->_str[SQLConstants::TOKEN_COLUMN_NAME] = _t.toUpper();  //列名转大写
                        token = SQLConstants::TOKEN_COLUMN_NAME;
                        break;
                    } else if (token == SQLConstants::TOKEN_CREATE_TABLE) {
                        // CREATE TABLE _TABLE_NAME_ (...)
                        query->_str[SQLConstants::TOKEN_TABLE_NAME] = _t.toUpper();   //表名转大写
                        expect_parenthesis_group = true;
                        token = SQLConstants::TOKEN_TABLE_BEGIN;
                        break;
                    } else if (token == SQLConstants::TOKEN_TABLE_BEGIN) {
                        // ( _COLUMN_NAME_ COLUMN_TYPE(SIZE) COLUMN_TYPE ; ... ; )
                        _create_table_column = SQLCreateTableColumnObject();
                        _create_table_column.name = _t.toUpper();
                        token = SQLConstants::TOKEN_TABLE_COLUMN_NAME;
                        break;
                    } else if (
                               token == SQLConstants::TOKEN_TABLE_COLUMN_NAME ||
                               token == SQLConstants::TOKEN_TABLE_COLUMN_TYPE
                               ) {
                        // COLUMN_NAME COLUMN_TYPE ( SIZE ) NOT NULL
                        // 留给括号来处理
                        break;
                    } else if (token == SQLConstants::TOKEN_TABLE_COLUMN_SIZE) {
                        // COLUMN_NAME COLUMN_TYPE ( SIZE ) _NOT_
                        if (t.size() == 0) {
                            //)_ _NOT
                            break;
                        } else {
                            // must be NOT / NULL
                            if (_t.toUpper() == "NOT") {
                                token = SQLConstants::TOKEN_TABLE_COLUMN_NOT;
                                break;
                            } else if (_t.toUpper() == "NULL") {
                                token = SQLConstants::TOKEN_TABLE_COLUMN_NULL;
                                break;
                            } else {
                                syntax_error = true;
                                throw MyString("[!E26] Syntax error at ").concat(pos).concat(", \'[NOT] NULL\' expected");
                                break;
                            }
                        }
                    } else if (token == SQLConstants::TOKEN_TABLE_COLUMN_NOT) {
                        // must be NULL
                        if (_t.toUpper() == "NULL") {
                            token = SQLConstants::TOKEN_TABLE_COLUMN_NULL;
                            _create_table_column.can_null = false;
                            break;
                        } else {
                            syntax_error = true;
                            throw MyString("[!E27] Syntax error at ").concat(pos).concat(", \'NULL\' expected");
                            break;
                        }
                    } else if (token == SQLConstants::TOKEN_TABLE_COLUMN_NULL) {
                        // 留给分号来处理
                        break;
                    }
                }
                
                break;
                
            default:
                
                // 允许a-zA-Z0-9\_\-\*
                if ((ch >= 'a' && ch <= 'z') ||
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9') ||
                    (ch == '_') ||
                    (ch == '-') ||
                    (ch == '*')
                    ) {
                    if (status == SQLParser::PARSE_STATUS_DELIM) {
                        // 上一个字符是空格分隔符：重新开始累加字符
                        t.clear();
                        status = SQLParser::PARSE_STATUS_TOKEN;
                    }
                    
                    // 累加字符
                    t.push_back(ch);
                } else {
                    // 其他特殊字符
                    syntax_error = true;
                    throw MyString("[!E28] Syntax error at ").concat(pos).concat(", unexpected char ").concat(ch);
                }
                
                break;
        }
        
        if (ch == '\0' && !syntax_error) {
            // 检查其他语句是否已结束
            
            if (token == SQLConstants::TOKEN_WHERE) {
                if (parenthesis > 0) {
                    // 结束前判断 WHERE 里的括号是否已经平衡
                    syntax_error = true;
                    throw MyString("[!E29.1] Syntax error at ").concat(pos).concat(", \')\' expected");
                    break;
                }
            }
            
            if (_opToken == SQLConstants::OPERATION_UNDEFINED) {
                syntax_error = true;
                throw MyString("[!E29.2] Syntax error at ").concat(pos).concat(", unknown OPCODE");
                break;
            } else if (_opToken == SQLConstants::OPERATION_SELECT) {
                if (query->_select_columns.size() == 0) {
                    syntax_error = true;
                    throw MyString("[!E29.3] Syntax error at ").concat(pos).concat(", COLUMN_NAME expected");
                    break;
                }
                
                if (query->_str.find(SQLConstants::TOKEN_TABLE_NAME) == query->_str.end()) {
                    syntax_error = true;
                    throw MyString("[!E29.4] Syntax error at ").concat(pos).concat(", TABLE_NAME expected");
                    break;
                }
            } else if (_opToken == SQLConstants::OPERATION_UPDATE) {
                if (query->_str.find(SQLConstants::TOKEN_COLUMN_NAME) == query->_str.end()) {
                    syntax_error = true;
                    throw MyString("[!E29.5] Syntax error at ").concat(pos).concat(", COLUMN_NAME expected");
                    break;
                }
                
                if (query->_str.find(SQLConstants::TOKEN_COLUMN_VALUE) == query->_str.end()) {
                    syntax_error = true;
                    throw MyString("[!E29.6] Syntax error at ").concat(pos).concat(", COLUMN_VALUE expected");
                    break;
                }
            } else if (_opToken == SQLConstants::OPERATION_CREATE_TABLE) {
                if (query->_str.find(SQLConstants::TOKEN_TABLE_NAME) == query->_str.end()) {
                    syntax_error = true;
                    throw MyString("[!E29.7] Syntax error at ").concat(pos).concat(", TABLE_NAME expected");
                    break;
                }
            }
            // TODO: other
        }
        
        pos++;
    }
    
    return *query;
    
}