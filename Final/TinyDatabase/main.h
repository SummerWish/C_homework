//
//  main.h
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_main_h
#define _352978_main_h

#include <iostream>
#include <vector>

#include "MyConsole.h"
#include "MyString.h"
#include "SQL.h"

#define APP_VERSION "0.0.1.20140103.183700"
#define OUTPUT_PREFIX "1352978_"

int database_handler_create_table(const std::vector<MyString>& params);
int database_handler_import(const std::vector<MyString>& params);
int database_handler_select(const std::vector<MyString>& params);
int database_handler_update(const std::vector<MyString>& params);
int database_handler_delete(const std::vector<MyString>& params);
int database_handler_interactive_mode(const std::vector<MyString>& params);
int database_handler_help(const std::vector<MyString>& params);
int database_handler_quit(const std::vector<MyString>& params);

char *_read(const char *filename);
MyString& read(const MyString& filename);
MyString& read(const char *filename);

/*
 从文件中读取所有内容
 */
char *_read(const char *filename)
{
    char *content = new char[1];
    int fsize = 0;
    FILE *f;
    
    f = fopen(filename, "rb");
    
    if (f) {
        
        // get FILE_SIZE
        fseek(f, 0, SEEK_END);
        fsize = (int)ftell(f);
        rewind(f);
        
        // read content
        delete[] content;
        content = new char[fsize + 1];
        fread(content, sizeof(char), fsize, f);
        fclose(f);
        
        content[fsize] = '\0';
        
        return content;
        
    } else {
        
        fclose(f);
        
        delete[] content;
        throw MyString("Cannot open file");
        
    }
}

MyString& read(const MyString& filename)
{
    char *fp = filename.toCString();
    
    MyString &val = read(fp);
    delete[] fp;
    
    return val;
}

MyString& read(const char *filename)
{
    char *content;
    content = _read(filename);
    
    MyString *val = new MyString(content);
    delete[] content;
    
    return *val;
}

#endif
