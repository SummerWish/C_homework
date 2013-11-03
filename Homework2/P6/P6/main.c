//
//  main.c
//  P6
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

#define R 10
#define r 9.5

int main(int argc, const char * argv[])
{
    for (int j = -R; j <= R; ++j) {
        for (int i = -R; i <= R; ++i) {
            if ((i*i+j*j <= R*R) && (i*i+j*j >= r*r)) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    
    return 0;
}

