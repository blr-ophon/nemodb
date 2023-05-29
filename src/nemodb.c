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
    if(!db){
        printf("database not found");
    }

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
    char dbfolder[MAX_PATHNAME] = {0};
    snprintf(dbfolder, MAX_PATHNAME, "%s/%s", basedir, name);

    if(stat(dbfolder, &st) < 0){ 
        mkdir(dbfolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //or 0700
    }

    //create database file
    char dbfile[2*MAX_PATHNAME] = {0};
    char metafile[2*MAX_PATHNAME] = {0};
    snprintf(dbfile, MAX_PATHNAME, "%s/%s%s", dbfolder, name, ".dat");
    snprintf(metafile, MAX_PATHNAME, "%s/%s%s", dbfolder, name, ".kv");

    //TODO: ask if user wishes to overwrite if it's the same name
    FILE *dbf = fopen(dbfile, "w");
    FILE *metaf = fopen(metafile, "w");
    fclose(dbf);
    fclose(metaf);
}

Database *DB_load(char *dbname){
    struct stat st = {0};

    //basedir + / + name + / + name.dat
    char *basedir = DB_BASEDIR;

    char dbfolder[MAX_PATHNAME] = {0};
    snprintf(dbfolder, MAX_PATHNAME, "%s/%s", basedir, dbname);

    char dbfile[2*MAX_PATHNAME] = {0};
    char metafile[2*MAX_PATHNAME] = {0};
    snprintf(dbfile, MAX_PATHNAME, "%s/%s%s", dbfolder, dbname, ".dat");
    snprintf(metafile, MAX_PATHNAME, "%s/%s%s", dbfolder, dbname, ".kv");

    if(stat(dbfolder, &st) < 0){ 
        //database not found
        return NULL;
    }

    Database *database = malloc(sizeof(Database));
    database->name = strdup(dbname);
    database->path = strdup(dbfolder);  

    //LOAD HASHTABLE
    
    database->indexfile.id = 0;  //TODO
    database->indexfile.offset = 0;  //TODO
    database->indexfile.reader = fopen(metafile, "rb");
    database->indexfile.writer = fopen(metafile, "ab");
    //TODO: check fopen errno
    database->keyDir = Metadata_load(database->indexfile.reader);

    //LOAD DATAFILE
    
    database->datafile.id = 0;  //TODO
    database->datafile.offset = 0;  //TODO
    database->datafile.reader = fopen(dbfile, "rb");
    database->datafile.writer = fopen(dbfile, "ab");
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
    if(ht_search(db->keyDir, key)){
        //TODO: delete previous entry in hashmap and indexfile
        return;
    }
    
    //create and store record file
    Record *rec = Record_create(key, data, size);         
    Meta metadata;  //to be filled by Record_store
    Record_store(db, rec, &metadata);

    //append to indexfile 
    Metadata_append(db->indexfile.writer, rec, &metadata);

    //store in hashmap
    ht_insert(db->keyDir, key, &metadata);
    fflush(db->datafile.writer);
    fflush(db->indexfile.writer);

    Record_free(rec);
}

Record *DB_search(Database *db, char *key){
    //fseek(db->datafile.reader, metadata->RecordPos, SEEK_SET);
    //fread(rec, metadata->RecordSize, 1, db->datafile.reader);
    fflush(db->datafile.reader);
    return Record_load(db, key);
}
