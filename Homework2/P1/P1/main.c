//
//  main.c
//  P1
//
//  Created by Breezewish on 13-10-31.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

#define PI 3.1415927

int main(int argc, const char * argv[])
{
    float r = 0.0, h = 0.0;
    
    printf("Please input R and H:\n");
    printf("r=");
    scanf("%f", &r);
    printf("h=");
    scanf("%f", &h);
    
    printf("\nCalculate result:\n");
    printf("C(circle)=%.2f\n", 2*PI*r);
    printf("S(circle)=%.2f\n", PI*r*r);
    printf("S(sphere)=%.2f\n", 4*PI*r*r);
    printf("V(sphere)=%.2f\n", PI*r*r*r/3.0);
    printf("V(cylinder)=%.2f\n", PI*r*r*h);
    
    return 0;
}

