//
//  main.c
//  P5
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

#define SIZE 5

int main(int argc, const char * argv[])
{
    
    for (int row = 0; row < SIZE; ++row) {
        for (int col = row; col > 0; --col) {
            printf(" ");
        }
        for (int col = 0; col < SIZE; ++col) {
            printf("*");
        }
        printf("\n");
    }
    
    return 0;
}

