#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "nemodb.h"
//TODO: check errno for system calls

#define MAX_PATHNAME 64
#define MAX_KEYNAME 64
#define DB_BASEDIR "./databases"

void DB_insert(char *key, uint8_t data, size_t size);
void DB_delete(char *key);
Record *DB_search(char *key);

int main(void){
    DB_create("test_db");
    Database *db = DB_load("test_db");

    //DB_setKeyValue(db, "test_key", "test_val");
    //char *val = DB_getKeyValue(db, "test_key");
    //if(val){
    //    printf("%s\n", val);
    //    free(val);
    //}
    DB_destroy(db);
    return 0;
}

//create database directory and struct
void DB_create(char *name){
    struct stat st = {0};

    char *basedir = DB_BASEDIR;
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
}

void DB_destroy(Database *db){
    // Delete database file
    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s", db->path);
    if(system(command) == 0){
        printf("'%s' database deleted\n", db->name);
    }else{
        printf("Failed to delete '%s' database\n", db->name);
    }

    free(db->name);
    free(db->path);
    free(db);
}

Database *DB_load(char *dbname){
    struct stat st = {0};

    //basedir + / + name
    char *basedir = DB_BASEDIR;
    char dbfolder[MAX_PATHNAME];
    strcat(dbfolder, basedir);
    strcat(dbfolder, "/");
    strncat(dbfolder, dbname, MAX_PATHNAME - strlen(basedir) + 1);

    if(stat(dbfolder, &st) < 0){ 
        //database not found
        return NULL;
    }

    Database *database = malloc(sizeof(Database));
    database->name = strdup(dbname);
    database->path = strdup(dbfolder);  

    //TODO load hashmap
    char metadata[MAX_PATHNAME];
    strncat(metadata, "/", 1);
    strncat(metadata, dbname, strlen(dbname));
    strncat(metadata, ".kv", 3);

    FILE *metafile = fopen(metadata, "ab+"); 
    if(!metafile){
        //TODO: file opening failed
    }

    //TODO fill datafile 


    return database;
}











//Open nemodb/dbname/key.kv and store val
//void DB_setKeyValue(Database *db, char *key, char *val){
//    char fname[MAX_PATHNAME + MAX_KEYNAME] = {0};
//    strncat(fname, db->path, MAX_PATHNAME);
//    strcat(fname, "/");
//    strncat(fname, key, MAX_KEYNAME - 4);
//    strcat(fname, ".kv");
//
//    FILE *f = fopen(fname, "w");
//    fprintf(f, "%s", val);
//    fclose(f);
//}
//
////Read all chars(in this case where the values are strings) of
////nemodb/dbname/key_string.kv and return the complete string
//char *DB_getKeyValue(Database *db, char *key){
//    char fname[MAX_PATHNAME + MAX_KEYNAME] = {0};
//    strncat(fname, db->path, MAX_PATHNAME);
//    strcat(fname, "/");
//    strncat(fname, key, MAX_KEYNAME - 3);
//    strcat(fname, ".kv");
//
//    FILE *f = fopen(fname, "r");
//    if(f == 0){
//        printf("\nFile not found\n");
//        return NULL;
//    }
//
//    fseek(f, 0, SEEK_END);
//    int flength = ftell(f);
//    rewind(f);
//
//    char *val = malloc((flength+1) *sizeof(char));
//    char tmp;
//    int i;
//    for(i = 0; (tmp = fgetc(f)) != EOF; i++) {
//        val[i] = tmp;
//    }
//    val[i] = '\0';
//
//    fclose(f);
//    return val;
//}
