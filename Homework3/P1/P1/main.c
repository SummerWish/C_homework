//
//  main.c
//  P1
//
//  Created by Breezewish on 13-11-14.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    
    int c1, c2;
    c1 = getchar();
    c2 = getchar();
    
    printf("Output by putchar:\n");
    putchar(c1);
    putchar(c2);
    printf("\n");
    
    printf("Output by printf:\n");
    printf("%c%c\n", c1, c2);
    
    // Question1:
    //
    // Should be defined as an integer.
    //
    // The prototype of getchar() is `int getchar ( void );`
    // so definitely c1 and c2 should be `int`
    //
    // However, in many cases, `char` will work but it
    // is not a best practice.
    
    
    // Question2:
    printf("Output ascii:\n");
    printf("%d %d\n", c1, c2);
    
    // Question3:
    //
    // Of course not. Their range-limit is different.
    
    return 0;
}

