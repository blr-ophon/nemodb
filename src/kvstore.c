#include "kvstore.h"
#include "hashtable.h"

void Header_encode(RecHeader *header, uint8_t *buf){
    memcpy(buf, header, sizeof(RecHeader)); 
}

void Header_decode(uint8_t *buf, RecHeader *header){
    memcpy(header, buf, sizeof(RecHeader)); 
}

void Record_store(Record *rec){
    //insert in hashmap
    
    //get hash for rec->key, encode header and store
    //value byte array
    uint8_t buf[256];
    Header_encode(&rec->header, buf);
}

void Record_load(Record *rec){
    //read record for rec->key
    uint8_t buf[256];
    Header_decode(buf, &rec->header);
    //buf to rec->val
}
