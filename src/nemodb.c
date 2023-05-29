#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "nemodb.h"

//TODO: check errno for system calls

#define MAX_PATHNAME 64
#define MAX_KEYNAME 64
#define DB_BASEDIR "./databases"

int main(void){
    DB_create("test_db");
    Database *db = DB_load("test_db");

    uint8_t data[7] = {1,2,3,4,5,6,7};
    DB_insert(db, "testkey", data, 7);


    Record *rec = DB_search(db, "testkey");

    for(uint32_t i = 0; i < rec->header.Vsize; i++){
        printf("%d ", rec->value[i]);
    }
    printf("\n"); 


    DB_free(db);
    return 0;
}

//create database directory and struct
void DB_create(char *name){
    struct stat st = {0};

    //create base directory
    char *basedir = DB_BASEDIR;
    if(stat(basedir, &st) < 0){ 
        mkdir(basedir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }
    
    //create database directory (basedir + / + name)
    char dbfolder[MAX_PATHNAME];
    strncat(dbfolder, basedir, strlen(basedir));
    strncat(dbfolder, "/", 1);
    strncat(dbfolder, name, MAX_PATHNAME - strlen(basedir) + 1);

    if(stat(dbfolder, &st) < 0){ 
        mkdir(dbfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }

    //create database file
    char dbfile[2*MAX_PATHNAME];
    strncat(dbfile, dbfolder, strlen(dbfolder));
    strncat(dbfile, "/", 1);
    strncat(dbfile, name, MAX_PATHNAME - strlen(basedir) + 1);
    strncat(dbfile, ".dat", 4);

    //TODO: ask if user wishes to overwrite if it's the same name
    FILE *f = fopen(dbfile, "w");
    fclose(f);
}

Database *DB_load(char *dbname){
    struct stat st = {0};

    //basedir + / + name + / + name.dat
    char *basedir = DB_BASEDIR;
    char dbfolder[MAX_PATHNAME];
    strncat(dbfolder, basedir, strlen(basedir));
    strncat(dbfolder, "/", 1);
    strncat(dbfolder, dbname, MAX_PATHNAME - strlen(basedir) + 1);
    char dbfile[2*MAX_PATHNAME];
    strncat(dbfile, dbfolder, strlen(dbfolder));
    strncat(dbfile, "/", 1);
    strncat(dbfile, dbname, MAX_PATHNAME - strlen(basedir) + 1);
    strncat(dbfile, ".dat", 4);

    if(stat(dbfolder, &st) < 0){ 
        //database not found
        return NULL;
    }

    Database *database = malloc(sizeof(Database));
    database->name = strdup(dbname);
    database->path = strdup(dbfolder);  

    //LOAD HASHTABLE
    char metadata[MAX_PATHNAME];
    strncat(metadata, "/", 1);
    strncat(metadata, dbname, strlen(dbname));
    strncat(metadata, ".kv", 3);

    FILE *metafile = fopen(metadata, "ab+"); 
    if(!metafile){
        //TODO: file opening failed
    }
    database->keyDir = Metadata_load(metafile);

    //fill datafile 
    database->datafile.id = 0;  //TODO
    database->datafile.offset = 0;  //TODO
    database->datafile.reader = fopen(database->path, "rb");
    database->datafile.writer = fopen(database->path, "ab+");
    //TODO: check fopen errno

    return database;
}

void DB_free(Database *db){
    ht_freeTable(db->keyDir);
    fclose(db->datafile.reader);
    fclose(db->datafile.writer);

    free(db->name);
    free(db->path);
    free(db);
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

    DB_free(db);
}

void DB_insert(Database *db, char *key, uint8_t *data, size_t size){
    //create and store record file
    Record *rec = Record_create(key, data, size);         
    Meta *metadata = NULL;
    Record_store(db, rec, metadata);
    Record_free(rec);

    //store in hashmap
    ht_insert(db->keyDir, key, metadata);
}

Record *DB_search(Database *db, char *key){
    Meta *metadata = Metadata_retrieve(db->keyDir, key);
    fseek(db->datafile.reader, metadata->RecordPos, SEEK_SET);
    
    Record *rec = malloc(sizeof(Record));
    fread(rec, metadata->RecordSize, 1, db->datafile.reader);

    return rec;
}
