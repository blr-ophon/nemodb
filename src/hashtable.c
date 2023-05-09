#include "hashtable.h"

#define MAX_NAME 128

//int main(void){
//    HTable *table = ht_createTable(64);
//    printf("joao => %u\n", ht_djb2("joao", table->size));
//    printf("corno => %u\n", ht_djb2("corno", table->size));
//    printf("maria => %u\n", ht_djb2("maria", table->size));
//    printf("gorda => %u\n", ht_djb2("gorda", table->size));
//    printf("Jacob => %u\n", ht_djb2("Jacob", table->size));
//
//    ht_insert(table, "joao", "1");
//    ht_insert(table, "corno", "2");
//    ht_insert(table, "maria", "3");
//    ht_insert(table, "gorda", "4");
//    ht_insert(table, "Jacob", "5");
//
//    ht_printTable(table);
//    ht_freeTable(table);
//    return 0;
//}

void ht_printTable(HTable *table){
    for(size_t i = 0; i < table->size; i++){
        if(table->entries[i]){
            printf("%s : %s\n", 
                    table->entries[i]->key,
                    table->entries[i]->val

                  );
        }
    }
}

uint32_t ht_djb2(char *key, uint32_t table_size){
    int len = strnlen(key, MAX_NAME);

    uint32_t hash = 5381;
    for(int i = 0; i < len; i++){
        hash = ((hash << 5) + hash) + key[i];
    }

    return hash % table_size;
}


uint32_t ht_hash(char *key, int attempts, size_t table_size){
    uint32_t h1 = ht_djb2(key, table_size);
    if(attempts == 0){
        return h1;
    }

    //double hashing
    uint32_t h2 = 1 + (ht_djb2(key, table_size- 2) % (table_size- 2));
    return (h1 + attempts * h2) % table_size;
}


void ht_insert(HTable *table, char *key, char *val){
    uint32_t hash = ht_hash(key, 0, table->size);
    HT_entry **entries = table->entries;

    if(entries[hash] == NULL){
        entries[hash] = ht_createEntry(key, val);

    //Collision
    }else{
        //hash while entry is filled
        int i = 0;
        do{
            if(strncmp(entries[hash]->key, key, MAX_NAME)){
                //ignore if trying to insert repeated entry
                return;
            }
            hash = ht_hash(key, ++i, table->size);
        }while(entries[hash]);

        entries[hash] = ht_createEntry(key, val);
    }
}

void ht_delete(HTable *table, char *key){
    uint32_t hash = ht_hash(key, 0, table->size);
    HT_entry **entries = table->entries;

    if(entries[hash]){
        //entry is found
        if(strncmp(entries[hash]->key, key, MAX_NAME) == 0){
            //if key matches
            ht_freeEntry(entries[hash]);
            entries[hash]->key = NULL;
            entries[hash]->val = NULL;

            
        //Collision
        }else{
            //hash while key doesn't match
            int i = 0;
            do{
                hash = ht_hash(key, ++i, table->size);
                if(entries[hash] == NULL){
                    //no entry found
                    return;
                }
            }while(strncmp(entries[hash]->key, key, MAX_NAME));

            ht_freeEntry(entries[hash]);
            entries[hash]->key = NULL;
            entries[hash]->val = NULL;
        }
    }
}

HT_entry *ht_search(HTable *table, char *key){
    uint32_t hash = ht_hash(key, 0, table->size);
    HT_entry **entries = table->entries;
    

    if(entries[hash]){
        //entry is found
        if(strncmp(entries[hash]->key, key, MAX_NAME) == 0){
            //if key matches
            return entries[hash];
            
        //Collision
        }else{
            //hash while key doesn't match
            int i = 0;
            do{
                hash = ht_hash(key, ++i, table->size);
                if(entries[hash] == NULL){
                    //no entry found
                    return NULL;
                }
            }while(strncmp(entries[hash]->key, key, MAX_NAME));

            return entries[hash];
        }
    }

    return NULL;
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

