//
//  main.c
//  P2
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{

    int score = 0;
    
    printf("Please input the score:");
    scanf("%d", &score);
    
    if (score > 100 || score < 0) {
        printf("Input invalid!\n");
        return 1;
    }
    
    printf("Grade:");
    
    //Using 'IF' statement
    /*
    if (score > 100 || score < 0) {
        printf("[Input invalid]");
    } else if (score >= 90) {
        printf("A");
    } else if (score >= 80) {
        printf("B");
    } else if (score >= 70) {
        printf("C");
    } else if (score >= 60) {
        printf("D");
    } else {
        printf("E");
    }
    */
    
    //Using 'SWITCH' statement
    switch (score / 10) {
        case 9: /* fall through */
        case 10:
            printf("A");
            break;
        case 8:
            printf("B");
            break;
        case 7:
            printf("C");
            break;
        case 6:
            printf("D");
            break;
        default:
            printf("E");
            break;
    }
    
    printf("\n");
    
    return 0;
}

