//
//  MyConsole.cpp
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "MyConsole.h"

const char *MyConsole::FLAG_CREATE_TABLE   = "c";
const char *MyConsole::FLAG_IMPORT         = "i";
const char *MyConsole::FLAG_SELECT         = "s";
const char *MyConsole::FLAG_UPDATE         = "u";
const char *MyConsole::FLAG_DELETE         = "d";
const char *MyConsole::FLAG_INTERACTIVE    = "it";
const char *MyConsole::FLAG_INDEX          = "index";
const char *MyConsole::FLAG_QUIT           = "q";
const char *MyConsole::FLAG_HELP           = "h";

/*
 输出欢迎信息
 */
void MyConsole::hello(const char *version)
{
    std::cout << "Breezewish's Tiny SQL Database" << std::endl;
    std::cout << "Version: " << version << std::endl;
}

/*
 输出帮助信息
 */
int MyConsole::help()
{
    for (int i = 0; i < _desc.size(); ++i) {
        std::cout << std::left;
        std::cout << std::setw(8) << _desc[i].flag;
        std::cout << std::setw(15) << _desc[i].params;
        std::cout << _desc[i].desc << std::endl;
    }
    
    return MyConsole::STATUS_OK;
}

/*
 绑定对不同的flag的处理函数
 */
MyConsole& MyConsole::bind(const MyConsoleHandler handler,
                           const char *flag, const char *desc_params, const char *desc)
{
    // 转换为MyString插入std::map（免去比较器）
    MyString _flag = flag;
    
    _handlers[_flag] = handler;
    
    ConsoleDescItem it = {flag, desc_params, desc};
    _desc.push_back(it);
    
    return *this;
}

/*
 从控制台读取一行输入，调用并返回相应处理函数
 */
int MyConsole::read()
{
    std::cout << "-";
    
    // 解析输入的参数
    char input[1024];
    std::cin.getline(input, 1024);
    std::vector<MyString> params = MyString(input).split((char*)" ");
    
    // 函数回调是否存在？
    std::map<MyString, MyConsoleHandler>::iterator flag = _handlers.find(params[0]);
    if (flag == _handlers.end()) {
        return MyConsole::STATUS_OK;
    }
    
    // 执行相关函数
    params.erase(params.begin());
    return flag->second(params);
}