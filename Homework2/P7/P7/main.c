//
//  main.c
//  P7
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

#define SIZE 30

int main(int argc, const char * argv[])
{
    
    int fib[SIZE+2] = {1, 1}; /* 1, 1, 0, 0, .... */
    
    for (int i = 2; i < SIZE+2; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    
    double result = 0.0;
    
    for (int i = 1; i <= SIZE; ++i) {
        result += (double)fib[i+1] / fib[i];
    }
    
    printf("result=%lf\n", result);
    
    return 0;
}

