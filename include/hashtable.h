#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct ht_entry{
    char *key;
    char *val;
}HT_entry;

typedef struct htable{
    size_t size;
    int count;
    HT_entry **entries; //array of pointers to entries
}HTable;



char *ht_search(char **table, char *key);

int ht_getID(char *key);

HTable *ht_createTable(size_t size);

HT_entry *ht_createEntry(char *key, char *val);

void ht_freeEntry(HT_entry *entry);

void ht_freeTable(HTable *table);

#endif
