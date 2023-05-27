#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "nemodb.h"
//TODO: check errno for system calls

#define MAX_PATHNAME 64
#define MAX_KEYNAME 64

int main(void){
    Database *db = DB_create("teste");
    DB_setKeyValue(db, "key", "val");
    char *val = DB_getKeyValue(db, "key");
    printf("%s\n", val);
    free(val);
    DB_free(db);
    return 0;
}

//create database directory and struct
Database *DB_create(char *name){
    struct stat st = {0};

    char *basedir = "./nemodb";
    if(stat(basedir, &st) < 0){ 
        mkdir(basedir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }
    
    //basedir + / + name
    char dbfolder[MAX_PATHNAME];
    strcat(dbfolder, basedir);
    strcat(dbfolder, "/");
    strncat(dbfolder, name, MAX_PATHNAME - strlen(basedir) + 1);

    if(stat(dbfolder, &st) < 0){ 
        mkdir(dbfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }

    Database *database = malloc(sizeof(Database));
    database->name = strdup(name);
    database->path = strdup(dbfolder);  //TODO: absolute path
                                        
    return database;
}

void DB_free(Database *db){
    free(db->name);
    free(db->path);
    free(db);
}

//Open nemodb/dbname/key.kv and store val
void DB_setKeyValue(Database *db, char *key, char *val){
    char fname[MAX_PATHNAME + MAX_KEYNAME] = {0};
    strncat(fname, db->path, MAX_PATHNAME);
    strcat(fname, "/");
    strncat(fname, key, MAX_KEYNAME - 4);
    strcat(fname, ".kv");

    FILE *f = fopen(fname, "w");
    fprintf(f, "%s", val);
    fclose(f);
}

//Read all chars(in this case where the values are strings) of
//nemodb/dbname/key_string.kv and return the complete string
char *DB_getKeyValue(Database *db, char *key){
    char fname[MAX_PATHNAME + MAX_KEYNAME] = {0};
    strncat(fname, db->path, MAX_PATHNAME);
    strcat(fname, "/");
    strncat(fname, key, MAX_KEYNAME - 3);
    strcat(fname, ".kv");

    FILE *f = fopen(fname, "r");
    if(f == 0){
        printf("\nFile not found\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    int flength = ftell(f);
    rewind(f);

    char *val = malloc((flength+1) *sizeof(char));
    char tmp;
    int i;
    for(i = 0; (tmp = fgetc(f)) != EOF; i++) {
        val[i] = tmp;
    }
    val[i] = '\0';

    fclose(f);
    return val;
}
