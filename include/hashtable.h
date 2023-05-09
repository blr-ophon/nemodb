#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct ht_entry{
    char *key;
    char *val;
}HT_entry;

typedef struct htable{
    size_t size;
    int count;
    HT_entry **entries; //array of pointers to entries
}HTable;


uint32_t ht_hash(char *key);

HT_entry *ht_search(HTable *table, char *key);

HT_entry *ht_createEntry(char *key, char *val);

HTable *ht_createTable(size_t size);

void ht_freeEntry(HT_entry *entry);

void ht_freeTable(HTable *table);

#endif
