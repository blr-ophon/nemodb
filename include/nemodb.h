#ifndef NEMODB_h
#define NEMODB_h

#include <string.h>
#include <stdlib.h>

struct db{
    char *name;
    char *path;
};

struct db *DB_create(char *name);

void DB_free(struct db *db);

void DB_setKeyValue(char *key, char *val);

char *DB_getKeyValue(char *key);

#endif
