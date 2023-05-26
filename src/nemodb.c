#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "nemodb.h"
#include <string.h>
#include <stdlib.h>
//TODO: check errno for system calls


int main(void){
    DB_create("teste");
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
    strncat(dbfolder, name, 48);

    if(stat(dbfolder, &st) < 0){ 
        mkdir(dbfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }

    struct db *database = malloc(sizeof(struct db));
    database->name = strdup(name);
    database->path = strdup(dbfolder);  //TODO: absolute path
                                        
    return database;
}
