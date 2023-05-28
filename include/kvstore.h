#ifndef KVSTORE_H
#define KVSTORE_H

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

//metadata file is loaded into a hashmap in memory
//the hashmap contains the offset (recordPos) of the val in the
//datafile
//  TODO: make it so hashmap can expand 
//  TODO: make  hashmap independent of the value it is storing (use a dirty flag)
//open active datafile
//to search, look the hashmap to get offset in datafile
//to insert, insert in the hashmap, append to metafile and append to datafile
//to delete, delete in the datafile, delete in the metafile and delete in the map 

typedef struct{
    uint32_t checksum; 
    uint32_t timestamp; 
    uint32_t expiry; 
    uint32_t Ksize; 
    uint32_t Vsize; 
}RecHeader;

typedef struct{
    RecHeader header;
    char *key; 
    uint8_t value[];
}Record;

//wraps header in little endian and stores in buf
void Header_encode(RecHeader *header, uint8_t *buf);
//reads bytes from buf and writes to header
void Header_decode(uint8_t *buf, RecHeader *header);

////////////////////////////////////////////////////////////////////////////////

typedef struct{
    //pthread_rwlock_t rwlock;
    FILE* writer;
    FILE* reader;
    int id;
    int offset;
}Datafile;

void Datafile_insert(void);
void Datafile_delete(void);
void Datafile_search(void);

////////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint32_t  Timestamp; 
    uint32_t RecordSize;  
    uint32_t RecordPos;    
    uint32_t FileID;        
}Meta;

//loads database "name" map into memory
void Metadata_load(char *name);

uint32_t Metadata_getOffset(char *key);


#endif
