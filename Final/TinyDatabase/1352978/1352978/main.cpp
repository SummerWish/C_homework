//
//  main.cpp
//  1352978
//
//  Created by Breezewish on 13-12-5.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include "main.h"

MyConsole console;
SQL sql;

int execute_n = 0;

int main(int argc, const char * argv[])
{
    // Attach handlers
    console.bind(database_handler_create_table, MyConsole::FLAG_CREATE_TABLE, "file", "Create table from file");
    console.bind(database_handler_import, MyConsole::FLAG_IMPORT, "table file", "Import data to table");
    console.bind(database_handler_select, MyConsole::FLAG_SELECT, "file", "Select data by file");
    console.bind(database_handler_update, MyConsole::FLAG_UPDATE, "file", "Update data by file");
    console.bind(database_handler_delete, MyConsole::FLAG_DELETE, "file", "Delete data by file");
    console.bind(database_handler_interactive_mode, MyConsole::FLAG_INTERACTIVE, "", "Enter interactive mode");
    console.bind(database_handler_index, MyConsole::FLAG_INDEX, "0|1", "Switch off/on index");
    console.bind(database_handler_quit, MyConsole::FLAG_QUIT, "", "Quit");
    console.bind(database_handler_help, MyConsole::FLAG_HELP, "", "Show this screen");
    
    // loop
    console.hello();
    while (console.read() != MyConsole::STATUS_EXIT) execute_n++;
    
    return 0;
}

/*
 -it
 */
int database_handler_interactive_mode(std::vector<MyString> params)
{
    MyString line;
    
    do {
        std::cout << "interactive> ";
        
        char input[1024];
        std::cin.getline(input, 1024);
        line = MyString(input);
        
        if (line != "exit") {
            SQLResultObject result = sql.execute(line);
            if (!result.ok) {
                std::cout << "Error: " << result.err << std::endl;
            } else {
                std::cout << "Completed without errors in " << result.execute_time << "ms." << std::endl;
                std::cout << "Affected " << result.n << " rows." << std::endl;
                if (result.rows.size() > 0) {
                    result.print(std::cout);
                }
            }
        }
    } while (line != "exit");
    
    return MyConsole::STATUS_OK;
}

/*
 -c file
 */
int database_handler_create_table(std::vector<MyString> params)
{
    SQLResultObject result = sql.execute(read(params[0]));
    if (!result.ok) {
        std::cout << "Error: " << result.err << std::endl;
        return MyConsole::STATUS_FAIL;
    } else {
        std::cout << "Completed without errors in " << result.execute_time << "ms." << std::endl;
        return MyConsole::STATUS_OK;
    }
}

/*
 -i table file
 */
int database_handler_import(std::vector<MyString> params)
{
    SQLResultObject result = sql.import(params[0], params[1].toCString());
    if (!result.ok) {
        std::cout << "Error: " << result.err << std::endl;
        return MyConsole::STATUS_FAIL;
    } else {
        std::cout << "Completed without errors in " << result.execute_time << "ms." << std::endl;
        std::cout << "Imported " << result.n << " rows." << std::endl;
        
        return MyConsole::STATUS_OK;
    }
}

/*
 -s file
 */
int database_handler_select(std::vector<MyString> params)
{
    SQLResultObject result = sql.execute(read(params[0]));
    if (!result.ok) {
        std::cout << "Error: " << result.err << std::endl;
        return MyConsole::STATUS_FAIL;
    } else {
        MyString output = MyString("1352978_select_").concat(MyString(execute_n)).concat(".txt");
        std::cout << "Completed without errors in " << result.execute_time << "ms." << std::endl;
        std::cout << "Selected " << result.n << " rows." << std::endl;
        std::cout << "Data outputed to: " << output << std::endl;
        result.xport(output);
        
        return MyConsole::STATUS_OK;
    }
}

/*
 -u file
 */
int database_handler_update(std::vector<MyString> params)
{
    SQLResultObject result = sql.execute(read(params[0]));
    if (!result.ok) {
        std::cout << "Error: " << result.err << std::endl;
        return MyConsole::STATUS_FAIL;
    } else {
        MyString output = MyString("1352978_update_").concat(MyString(execute_n)).concat(".txt");
        std::cout << "Completed without errors in " << result.execute_time << "ms." << std::endl;
        std::cout << "Updated " << result.n << " rows." << std::endl;
        std::cout << "Table data outputed to: " << output << std::endl;
        result.xport(output);
        
        return MyConsole::STATUS_OK;
    }
}

/*
 -d file
 */
int database_handler_delete(std::vector<MyString> params)
{
    SQLResultObject result = sql.execute(read(params[0]));
    if (!result.ok) {
        std::cout << "Error: " << result.err << std::endl;
        return MyConsole::STATUS_FAIL;
    } else {
        MyString output = MyString("1352978_delete_").concat(MyString(execute_n)).concat(".txt");
        std::cout << "Completed without errors in " << result.execute_time << "ms." << std::endl;
        std::cout << "Affected " << result.n << " rows." << std::endl;
        std::cout << "Table data outputed to: " << output << std::endl;
        sql.xport(result.tableName, output);
        
        return MyConsole::STATUS_OK;
    }
}

/*
 -index 0|1
 */
int database_handler_index(std::vector<MyString> params)
{
    console << "WTF creating index without giving me a column name!!??" << std::endl;
    return MyConsole::STATUS_OK;
}

/*
 -q
 */
int database_handler_quit(std::vector<MyString> params)
{
    console << "bye" << std::endl;
    return MyConsole::STATUS_EXIT;
}

/*
 -h
 */
int database_handler_help(std::vector<MyString> params)
{
    return console.help();
}
