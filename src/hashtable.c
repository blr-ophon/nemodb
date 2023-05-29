#include "hashtable.h"
#include "kvstore.h"

#define MAX_NAME 128

void Metadata_append(FILE *f, Record *rec, Meta *metadata){
    //generate metadata from record


    //4 bytes of key length / K_len bytes of key / sizeof(Meta) bytes of Meta
    uint32_t K_len = rec->header.Ksize;
    fwrite(&K_len, sizeof(uint32_t), 1, f);
    fwrite(&rec->key, K_len, 1, f);
    fwrite(metadata, sizeof(Meta), 1, f);   
}

HTable *Metadata_load(FILE *f){
    //get number of entries
    fseek(f, 0, SEEK_END);
    int flength = ftell(f)/sizeof(Meta);
    rewind(f);

    //load into Hashtable
    HTable *table = ht_createTable(flength*3);
    for(int i = 0; i < flength; i++){
        Meta *metadata = malloc(sizeof(Meta));

        //read key length
        uint32_t K_len;
        fread(&K_len, sizeof(uint32_t), 1, f);

        //read key 
        char *key = malloc(K_len);
        fread(key, K_len, 1, f);
        fread(metadata, sizeof(Meta), 1, f);

        //insert in hash table
        ht_insert(table, key, metadata);
        free(key);
    }
    return table;

}

void ht_printTable(HTable *table){
    for(size_t i = 0; i < table->size; i++){
        if(table->entries[i]){
            printf("%s : %u\n", 
                    table->entries[i]->key,
                    table->entries[i]->val.RecordPos

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


void ht_insert(HTable *table, char *key, Meta *val){
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
            //entries[hash]->val = NULL;

            
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
            //entries[hash]->val = NULL;
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

HT_entry *ht_createEntry(char *key, Meta *val){
    HT_entry *entry = malloc(sizeof(HT_entry));
    entry->key = strdup(key);
    memcpy(&entry->val, val, sizeof(Meta));
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
    //free(entry->val);
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


