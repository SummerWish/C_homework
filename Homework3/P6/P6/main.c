//
//  main.c
//  P6
//
//  Created by Breezewish on 13-11-14.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>
#include <string.h>

int my_strcmp(const char *p1, const char *p2);

int main(int argc, const char * argv[])
{
    char s1[255], s2[255];
    
    gets(s1);
    gets(s2);
    
    printf("my_strcmp = %d\n", my_strcmp(s1, s2));
    printf("   strcmp = %d\n", strcmp(s1, s2));
    
    return 0;
}

int my_strcmp(const char *p1, const char *p2)
{
    unsigned char *pc1 = (unsigned char *)p1;
    unsigned char *pc2 = (unsigned char *)p2;
    unsigned char c1, c2;
    
    do {
        c1 = *pc1++;
        c2 = *pc2++;
    } while (c1 == c2 && c1 != '\0');
    
    return (c1 - c2);
}