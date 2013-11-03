//
//  main.cpp
//  P4
//
//  Created by Breezewish on 13-10-9.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <iostream>
#include <algorithm>

int main(int argc, const char * argv[])
{

    int a, b, c;
    
    std::cin >> a >> b >> c;
    std::cout << std::max(a, std::max(b, c)) << std::endl;
    
    return 0;
    
}