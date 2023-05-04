#include "hashtable.h"

#define TABLE_SIZE 64

int ht_getID(char *key){
    return 0;
}

char *ht_search(char **table, char *key){
    return table[ht_getID(key)];
}

HT_entry *ht_createEntry(char *key, char *val){
    HT_entry *entry = malloc(sizeof(HT_entry));
    entry->key = strdup(key);
    entry->val = strdup(val);
    return entry;
}


HTable *ht_createTable(size_t size){
    HTable *table = malloc(sizeof(HTable)); 

    table->entries = calloc(size, sizeof(HT_entry*));
    table->size = size;
    table->count = 0;

    return table;
}

void ht_freeEntry(HT_entry *entry){
    free(entry->key);
    free(entry->val);
}

void ht_freeTable(HTable *table){
    //free all entries
    for(size_t i = 0; i < table->size; i++){
        if(table->entries[i]){
            ht_freeEntry(table->entries[i]);
        }
    }
    free(table->entries);
    free(table);
}

int main(void){
    HTable *table = ht_createTable(64);
    ht_freeTable(table);
    return 0;
}
