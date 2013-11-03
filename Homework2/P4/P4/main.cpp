//
//  main.cpp
//  P4
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>

int main(int argc, const char * argv[])
{
    std::vector<int> vec;
    
    std::cout << "Please input 8 numbers (separate with space)" << std::endl;
    std::cout << "Example:1 2 3 4 5 6 7 8" << std::endl;
    
    int n;
    
    for (int i = 0; i < 8; ++i) {
        std::cin >> n;
        vec.push_back(n);
    }
    
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    
    std::cout << "Sort result:" << std::endl;
    
    for (std::vector<int>::iterator it=vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    
    std::cout << std::endl;
    
    return 0;
}

