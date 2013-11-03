//
//  main.cpp
//  P6
//
//  Created by Breezewish on 13-10-9.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[])
{
    
    wchar_t s[] = L"Tongji";
    
    for (int i = 0; s[i] != '\0'; s[i++] += 3) /* empty */;
    
    std::wcout << s << std::endl;
    
    return 0;
    
}