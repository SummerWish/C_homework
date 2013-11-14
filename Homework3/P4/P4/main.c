//
//  main.c
//  P4
//
//  Created by Breezewish on 13-11-14.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int a[100], i, j, p, t, n=20;
    
    for (j = 0; j<n ; j++)
        scanf("%d", &a[j]);
    
    for (j = 0; j<n-1 ; j++) {
        p = j;
        
        // find the minimal number behind a[j]
        for (i=j+1; i<n ; i++)
            if (a[p]>a[i])
                p = i;
        
        // then swap it to a[j]
        if (p!=j) {
            t = a[j]; a[j] = a[p]; a[p]=t;
        }
    }
    
    for ( j = 0; j<n ; j++ )
        printf("%d ", a[j]);
    
    return 0;
}

