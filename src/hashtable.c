#include "hashtable.h"

#define TABLE_SIZE 64
#define MAX_NAME 128

int main(void){
    HTable *table = ht_createTable(64);
    ht_freeTable(table);
    printf("joao => %u\n", ht_hash("joao"));
    printf("corno => %u\n", ht_hash("corno"));
    printf("maria => %u\n", ht_hash("maria"));
    printf("gorda => %u\n", ht_hash("gorda"));
    printf("Jacob => %u\n", ht_hash("Jacob"));
    return 0;
}

void ht_insert(HTable *table, char *key, char *val){
    table->entries[ht_hash(key)] = ht_createEntry(key,val);
}

void ht_delete(HTable *table, char *key){
    HT_entry *entry = ht_search(table, key);
    ht_freeEntry(entry);
    table->entries[ht_hash(key)]->key = NULL;
    table->entries[ht_hash(key)]->val = NULL;
}

uint32_t ht_hash(char *key){
    int len = strnlen(key, MAX_NAME);
    uint32_t hash = 0;
    for(int i = 0; i < len; i++){
        //hash_value += key[i];
        hash = (64*hash + key[i]) % TABLE_SIZE;
    }
    return hash;
}

HT_entry *ht_search(HTable *table, char *key){
    return table->entries[ht_hash(key)];
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

