//
//  MyConsole.h
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef ___352978__MyConsole__
#define ___352978__MyConsole__

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include "MyString.h"

extern const char *VERSION;

typedef std::ostream& (*manip) (std::ostream&);
typedef int (*MyConsoleHandler)(std::vector<MyString>);

typedef struct
{
    const char *flag;
    const char *params;
    const char *desc;
} ConsoleDescItem;

class MyConsole
{
private:
    std::map<MyString, MyConsoleHandler> _handlers;
    std::vector<ConsoleDescItem> _desc;
    
public:
    
    static const char *FLAG_CREATE_TABLE;
    static const char *FLAG_IMPORT;
    static const char *FLAG_SELECT;
    static const char *FLAG_UPDATE;
    static const char *FLAG_DELETE;
    static const char *FLAG_INDEX;
    static const char *FLAG_INTERACTIVE;
    static const char *FLAG_QUIT;
    static const char *FLAG_HELP;
    
    enum
    {
        STATUS_OK = 0,
        STATUS_FAIL = 1,
        STATUS_EXIT = -1
    };
    
    /*
     绑定对不同的flag的处理函数
     */
    MyConsole& bind(const MyConsoleHandler handler,
                    const char *flag, const char *desc_param, const char *desc);
    
    /*
     输出欢迎信息
     */
    void hello();
    
    /*
     输出帮助信息
     */
    int help();
    
    /*
     从控制台读取一行输入，调用并返回相应处理函数
     */
    int read();
    
    /*
     对std::cout包装，实现 MyConsole << x
     */
    template <class T> MyConsole& operator<<(const T &t)
    {
        std::cout << t;
        return *this;
    }
    MyConsole& operator<<(manip manipulator)
    {
        std::cout << manipulator;
        return *this;
    }
    
};

#endif /* defined(___352978__MyConsole__) */
