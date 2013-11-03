//
//  main.cpp
//  P3
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <iostream>
#include <string>

int main(int argc, const char * argv[])
{
    std::wstring str;
    std::getline(std::wcin, str);
    
    int cLetter = 0, cDecimal = 0, cSymbol = 0, cSpace = 0;
    
    for (int i = (int)str.length() - 1; i >= 0; --i) {
        wchar_t ch = str.at(i);
        
        if ((ch >= 0x41 && ch <= 0x5a) || (ch >= 0x61 && ch <= 0x7a)) {
            cLetter++;
        } else if (ch == 0x20) {
            cSpace++;
        } else if (ch >= 0x30 && ch <= 0x39) {
            cDecimal++;
        } else {
            cSymbol++;
        }
    }
    
    std::wcout << "Count of letters = " << cLetter << std::endl;
    std::wcout << "Count of decimals = " << cDecimal << std::endl;
    std::wcout << "Count of spaces = " << cSpace << std::endl;
    std::wcout << "Count of symbols = " << cSymbol << std::endl;
    
    return 0;
}

