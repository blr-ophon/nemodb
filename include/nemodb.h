#ifndef NEMODB_h
#define NEMODB_h

#include <string.h>
#include <stdlib.h>

typedef struct database{
    char *name;
    char *path;
}Database;

Database *DB_create(char *name);

void DB_destroy(Database *db);

void DB_setKeyValue(Database *db, char *key, char *val);

char *DB_getKeyValue(Database *db, char *key);

#endif
