#ifndef NEMODB_h
#define NEMODB_h

#include "hashtable.h"
#include "kvstore.h"
#include <string.h>
#include <stdlib.h>

typedef struct{
    //pthread_rwlock_t rwlock;
    FILE* writer;
    FILE* reader;
    int id;
    int offset;
}Datafile;

typedef struct database{
    char *name;
    char *path;

    HTable *keyDir; 
    Datafile datafile;
}Database;


void DB_create(char *name);
Database *DB_load(char *dbname);

void DB_free(Database *db);
void DB_destroy(Database *db);

void DB_insert(Database *db, char *key, uint8_t *data, size_t size);
void DB_delete(char *key);
Record *DB_search(char *key);

#endif
