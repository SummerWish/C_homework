//
//  main.c
//  P8
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    //code 1
    //break: calculate SUM=1+2+.. until SUM >= 100
    int sum = 0;
    for (int i = 1; ; ++i) {
        sum += i;
        if (sum >= 100) {
            break;
        }
    }
    printf("sum1=%d\n", sum);
    
    //code 2:
    //continue: calculate the sum and the count of N (N % 3 = 0, 0 < N <= 100)
    int sum2 = 0, count2 = 0;
    for (int i = 1; i <= 100; ++i) {
        if (i % 3 != 0) {
            continue;
        }
        sum2 += i;
        count2 ++;
    }
    printf("sum2=%d, count2=%d\n", sum2, count2);
    
    return 0;
}

