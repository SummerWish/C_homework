//
//  main.c
//  P2
//
//  Created by Breezewish on 13-11-27.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define RECORD_SIZE 10

typedef struct {
    char name[50];
    int id;
} employee;

int find_by_id(const employee rec[], int length, int id);
int sort_comparer(const void *a, const void *b);

int main(int argc, const char * argv[])
{
    
    employee records[RECORD_SIZE];
    
    // 1. input
    printf("Input %d records (id name):\n", RECORD_SIZE);
    for (int i = 0; i < RECORD_SIZE; ++i) {
        printf("#%d: ", (i + 1));
        scanf("%d %s", &records[i].id, records[i].name);
    }
    
    // 2. sort
    qsort(records, RECORD_SIZE, sizeof(employee), sort_comparer);
    
    // output
    printf("\nRecords: (Count = %2d)\n", RECORD_SIZE);
    printf("=====================\n");
    for (int i = 0; i < RECORD_SIZE; ++i) {
        printf("%8d %s\n", records[i].id, records[i].name);
    }
    
    // 3. find
    int id;
    printf("\nFind name by id: ");
    scanf("%d", &id);
    int index = find_by_id(records, RECORD_SIZE, id);
    if (index == -1) {
        printf("record not found.\n");
    } else {
        printf("record found, id: %d, name: %s\n", records[index].id, records[index].name);
    }
    
    return 0;
}

int sort_comparer(const void *a, const void *b)
{
    
    const employee *_a = a;
    const employee *_b = b;
    
    return (_a->id - _b->id);
    
}

int find_by_id(const employee rec[], int length, int id)
{
    
    int l = 0, r = length - 1, mid;
    
    while (l <= r) {
        
        mid = (l + r) >> 1;
        
        if (id == rec[mid].id) {
            return mid;
        } else if (id < rec[mid].id) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
        
    }
    
    return -1;
    
}