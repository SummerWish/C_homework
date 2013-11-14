//
//  main.c
//  P2
//
//  Created by Breezewish on 13-11-14.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    
    unsigned int n;
    // MAX_UNSIGNED_SHORT is 65535, doesn't meet our needs.
    
    scanf("%d", &n);
    
    // 1. Calculate length
    unsigned int n2 = n, len = 0;
    while (n2 > 0) {
        n2 = n2 / 10;
        len++;
    }
    
    if (len > 5) {
        printf("Input number is too large.\n");
        return 1;
    }
    
    printf("length=%d\n", len);
    
    
    // 2. Output per digit BY ASC
    int digits[5] = {}, p = 0, n3 = n;
    while (n3 > 0) {
        digits[p++] = n3 % 10;
        n3 = n3 / 10;
    }
    
    printf("ASC digit=");
    
    for (int i = len - 1; i >= 0; --i)
        printf("%d ", digits[i]);
    
    printf("\n");
    
    
    // 3. Output per digit BY DEC
    printf("DEC digit=");
    
    for (int i = 0; i < len; ++i)
        printf("%d ", digits[i]);
    
    printf("\n");
    
    return 0;
}

