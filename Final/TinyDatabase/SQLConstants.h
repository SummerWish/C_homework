//
//  SQLConstants.h
//  1352978
//
//  Created by Breezewish on 13-12-7.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_SQLConstants_h
#define _352978_SQLConstants_h

class SQLConstants
{
public:
    enum
    {
        TOKEN_NULL                  = 0,
        TOKEN_OPERATION             = 1, // SELECT / UPDATE / ...
        TOKEN_TABLE_NAME            = 2, // the table name
        TOKEN_COLUMN_NAME           = 3, // the column name
        TOKEN_COLUMN_VALUE          = 4, // the column value (in UPDATE)
        // since this project is simple, we only need one (col_name, col_value) pair
        TOKEN_TOP_VALUE             = 5,// TOP N
        TOKEN_ORDER_COLUMN          = 6,// ORDER BY X (in SELECT)
        TOKEN_ORDER_ORDER           = 7,// ORDER BY X Y (in SELECT)
        
        
        TOKEN_FROM                  = 51,// FROM
        TOKEN_TOP                   = 52,// TOP
        TOKEN_ORDER                 = 53,// ORDER
        TOKEN_ORDER_BY              = 54,// BY
        TOKEN_WHERE                 = 55,// WHERE
        TOKEN_SET                   = 56,// SET
        TOKEN_BEGIN_ORDER           = 57,// (helper)
        TOKEN_BEGIN_WHERE_ORDER     = 58,// (helper)
        TOKEN_BEGIN_WHERE           = 59,// (helper)
        TOKEN_END_WHERE             = 60,// (helper)
        TOKEN_TABLE_BEGIN           = 61,
        TOKEN_TABLE_COLUMN_NAME     = 62,
        TOKEN_TABLE_COLUMN_TYPE     = 63,
        TOKEN_TABLE_COLUMN_SIZE     = 64,
        TOKEN_TABLE_COLUMN_NOT      = 65,
        TOKEN_TABLE_COLUMN_NULL     = 66,
        TOKEN_TABLE_LINE_END        = 67,
        TOKEN_TABLE_END             = 68,
        TOKEN_CREATE_TABLE          = 69,
        TOKEN_COLUMN_NAME_MULTI     = 70, // multiple allowed column name
        
        OPERATION_UNDEFINED         = 100,
        OPERATION_CREATE_TABLE      = 101,
        OPERATION_SELECT            = 102,
        OPERATION_UPDATE            = 103,
        OPERATION_DELETE            = 104,
        
        ORDER_ASC                   = 200,
        ORDER_DESC                  = 201,
        
        COLUMN_TYPE_UNDEFINED       = 300,
        COLUMN_TYPE_FLOAT           = 301,
        COLUMN_TYPE_CHAR            = 302,
        
        WHERE_COMPONENT_STATEMENT   = 400, // a = b, a like b, a < b, etc
        WHERE_COMPONENT_BEGIN       = 401, // (
        WHERE_COMPONENT_END         = 402, // )
        WHERE_COMPONENT_AND         = 403, // and
        WHERE_COMPONENT_OR          = 404, // or
        
        WHERE_STATEMENT_EQUAL       = 500, // a = b
        WHERE_STATEMENT_LIKE        = 501, // a like b
        WHERE_STATEMENT_GT          = 502, // a > b
        WHERE_STATEMENT_GTE         = 503, // a >= b
        WHERE_STATEMENT_LT          = 504, // a < b
        WHERE_STATEMENT_LTE         = 505, // a <= b
        WHERE_STATEMENT_NE          = 506, // a != b
    };
    
    /*
     常量转换为字符串，DEBUG用途
     */
    char *_constToString(int constant) const
    {
        switch (constant)
        {
            case TOKEN_NULL:                return (char*)"NULL";break;
            case TOKEN_OPERATION:           return (char*)"OPERATION";break;
            case TOKEN_TABLE_NAME:          return (char*)"TABLE_NAME";break;
            case TOKEN_COLUMN_NAME:         return (char*)"COLUMN_NAME";break;
            case TOKEN_COLUMN_VALUE:        return (char*)"COLUMN_VALUE";break;
            case TOKEN_TOP_VALUE:           return (char*)"TOP_VALUE";break;
            case TOKEN_ORDER_COLUMN:        return (char*)"ORDER_COLUMN";break;
            case TOKEN_ORDER_ORDER:         return (char*)"ORDER_ORDER";break;
            case OPERATION_UNDEFINED:       return (char*)"UNDEFINED";break;
            case OPERATION_CREATE_TABLE:    return (char*)"CREATE_TABLE";break;
            case OPERATION_SELECT:          return (char*)"SELECT";break;
            case OPERATION_UPDATE:          return (char*)"UPDATE";break;
            case OPERATION_DELETE:          return (char*)"DELETE";break;
            case ORDER_ASC:                 return (char*)"ASC";break;
            case ORDER_DESC:                return (char*)"DESC";break;
            case COLUMN_TYPE_UNDEFINED:     return (char*)"UNDEFINED";break;
            case COLUMN_TYPE_FLOAT:         return (char*)"FLOAT";break;
            case COLUMN_TYPE_CHAR:          return (char*)"CHAR";break;
            case WHERE_COMPONENT_BEGIN:     return (char*)"(";break;
            case WHERE_COMPONENT_END:       return (char*)")";break;
            case WHERE_COMPONENT_AND:       return (char*)"AND";break;
            case WHERE_COMPONENT_OR:        return (char*)"OR";break;
            case WHERE_STATEMENT_EQUAL:     return (char*)"=";break;
            case WHERE_STATEMENT_LIKE:      return (char*)"LIKE";break;
            case WHERE_STATEMENT_GT:        return (char*)">";break;
            case WHERE_STATEMENT_GTE:       return (char*)">=";break;
            case WHERE_STATEMENT_LT:        return (char*)"<";break;
            case WHERE_STATEMENT_LTE:       return (char*)"<=";break;
            case WHERE_STATEMENT_NE:        return (char*)"!=";break;
            default:
                char *tmp = new char[12];
                sprintf(tmp, "%d", constant);
                return tmp;
                break;
        }
    }
};

#endif
