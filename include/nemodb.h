#ifndef NEMODB_h
#define NEMODB_h

struct db{
    char *name;
    char *path;
};

struct db *DB_create(char *name);

#endif
