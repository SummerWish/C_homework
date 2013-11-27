//
//  main.c
//  P1
//
//  Created by Breezewish on 13-11-27.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

typedef int BOOL;

#define TRUE 1
#define FALSE 0

BOOL isLunarYear(int y);
int dayOfYear(int y, int m, int d);

int main(int argc, const char * argv[])
{
    
    int y, m ,d;
    
    printf("Please input: Y/m/d\n");
    scanf("%d/%d/%d", &y, &m, &d);
    
    printf("Nth day of the year: %d\n", dayOfYear(y, m, d));
    
    return 0;
}

int dayOfYear(int y, int m, int d)
{
    
    int daysOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Invalid month
    if (m < 1 || m > 12) {
        return -1;
    }
    
    // 1. Is Lunar year?
    if (isLunarYear(y) == TRUE) {
        daysOfMonth[2] = 29;
    }
    
    // Invalid day
    if (d > daysOfMonth[m] || d < 1) {
        return -1;
    }
    
    // 2. Is year 1752?
    // http://unix.stackexchange.com/questions/17903/is-cal-broken-what-happened-in-september-1752
    if (y == 1752) {
        
        daysOfMonth[9] = 19;
        
        if (m == 9) {
            if (d > 2 && d < 14) {    // 1752/9/3 ~ 1752/9/13 doesn't exist
                return -1;
            } else if (d > 30) {       // Invalid day
                return -1;
            } else if (d > 2) {
                d = d - 11;
            }
        }
    }
    
    // Calculate
    
    int days = 0;
    
    for (int i = 1; i < m; ++i) {
        days += daysOfMonth[i];
    }
    
    days += d;
    
    return days;
    
}

BOOL isLunarYear(int y)
{
    
    if (y % 400 == 0) {
        return TRUE;
    } else if (y % 4 == 0 && y % 100 != 0) {
        return TRUE;
    } else {
        return FALSE;
    }
    
}