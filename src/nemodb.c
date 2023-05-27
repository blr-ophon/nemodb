#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "nemodb.h"
//TODO: check errno for system calls


int main(void){
    struct db *db = DB_create("teste");
    DB_setKeyValue("key", "val");
    char *val = DB_getKeyValue("key");
    printf("%s\n", val);
    free(val);
    DB_free(db);
    return 0;
}

//create database directory and struct
struct db *DB_create(char *name){
    struct stat st = {0};

    char *basedir = "./nemodb";
    if(stat(basedir, &st) < 0){ 
        mkdir(basedir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }
    
    //basedir + / + name
    char dbfolder[64];
    strcat(dbfolder, basedir);
    strcat(dbfolder, "/");
    strncat(dbfolder, name, 64 - strlen(basedir) + 1);

    if(stat(dbfolder, &st) < 0){ 
        mkdir(dbfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }

    struct db *database = malloc(sizeof(struct db));
    database->name = strdup(name);
    database->path = strdup(dbfolder);  //TODO: absolute path
                                        
    return database;
}

void DB_free(struct db *db){
    free(db->name);
    free(db->path);
    free(db);
}

//Open nemodb/dbname/key.kv and store val
void DB_setKeyValue(char *key, char *val){
    char fname[64] = {0};
    strncat(fname, key, 60);
    strcat(fname, ".kv");

    FILE *f = fopen(fname, "w");
    fprintf(f, "%s", val);
    fclose(f);
}

//Read all chars(in this case where the values are strings) of
//nemodb/dbname/key_string.kv and return the complete string
char *DB_getKeyValue(char *key){
    char fname[64] = {0};
    strncat(fname, key, 60);
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
