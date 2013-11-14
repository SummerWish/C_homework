//
//  main.c
//  P5
//
//  Created by Breezewish on 13-11-14.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>

#define SIZE_STUDENT 3
#define SIZE_SUBJECT 4

int main(int argc, const char * argv[])
{
    
    int score[SIZE_STUDENT+1][SIZE_SUBJECT+1];
    
    for (int i = 0; i < SIZE_STUDENT; ++i) {
        printf("Student%d: ", i+1);
        for (int j = 0; j < SIZE_SUBJECT; ++j) {
            scanf("%d", &score[i][j]);
        }
    }
    
    // Calculate average score per student
    for (int i = 0; i < SIZE_STUDENT; ++i) {
        int sum = 0;
        for (int j = 0; j < SIZE_SUBJECT; ++j) {
            sum += score[i][j];
        }
        score[i][SIZE_SUBJECT] = sum / SIZE_SUBJECT;
    }
    
    // Calculate average score per subject
    for (int j = 0; j <= SIZE_SUBJECT; ++j) {
        int sum = 0;
        for (int i = 0; i < SIZE_STUDENT; ++i) {
            sum += score[i][j];
        }
        score[SIZE_STUDENT][j] = sum / SIZE_STUDENT;
    }
    
    // Output table
    for (int i = -1; i <= SIZE_STUDENT; ++i) {
        
        if (i == SIZE_STUDENT) {
            printf("average   ");
        } else if (i == -1) {
            printf("          ");
        } else {
            printf("student%d  ", i+1);
        }
        
        for (int j = 0; j <= SIZE_SUBJECT; ++j) {
            if (i == -1) {
                if (j == SIZE_SUBJECT) {
                    printf(" average");
                } else {
                    printf("subject%d  ", j+1);
                }
            } else {
                printf("%8d  ", score[i][j]);
            }
        }
        
        printf("\n");
    }
    
    return 0;
}

