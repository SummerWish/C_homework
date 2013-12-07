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

const char *VERSION = "0.0.1";

int database_handler_create_table(std::vector<MyString> params);
int database_handler_import(std::vector<MyString> params);
int database_handler_select(std::vector<MyString> params);
int database_handler_update(std::vector<MyString> params);
int database_handler_delete(std::vector<MyString> params);
int database_handler_help(std::vector<MyString> params);
int database_handler_index(std::vector<MyString> params);
int database_handler_quit(std::vector<MyString> params);

char *_read(const char *filename);
MyString read(const MyString& filename);
MyString read(const char *filename);

char *_read(const char *filename)
{
    char *fcontent = NULL;
    int fsize = 0;
    FILE *fp;
    
    fp = fopen(filename, "r");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        fsize = (int)ftell(fp);
        rewind(fp);
        
        fcontent = (char*) malloc(fsize + 1);
        fread(fcontent, 1, fsize, fp);
        
        fclose(fp);
    }
    
    fcontent[fsize] = '\0';
    
    return fcontent;
}

MyString read(const MyString& filename)
{
    char *fp = filename.toCString();
    return read(fp);
}

MyString read(const char *filename)
{
    char *content;
    content = _read(filename);
    
    return MyString(content);
}

#endif
