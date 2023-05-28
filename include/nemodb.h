#ifndef NEMODB_h
#define NEMODB_h

#include "kvstore.h"
#include "hashtable.h"
#include <string.h>
#include <stdlib.h>

typedef struct database{
    char *name;
    char *path;

    HTable keyDir; 
    Datafile datafile;
}Database;

void DB_create(char *name);

void DB_destroy(Database *db);

Database *DB_load(char *dbname);

//void DB_setKeyValue(Database *db, char *key, char *val);
//
//char *DB_getKeyValue(Database *db, char *key);

#endif
