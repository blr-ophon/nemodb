#include "kvstore.h"
#include "hashtable.h"
#include "nemodb.h"

Record *Record_create(char *key, uint8_t *val, size_t n){
    Record *rec = (Record*) malloc(sizeof(Record));
    rec->key = strdup(key);
    rec->value = malloc(n);
    memcpy(rec->value, val, n);
    
    //HEADER
    rec->header.Ksize = strlen(key) + 1;
    rec->header.Vsize = n;
    //TODO
    rec->header.checksum = 0; 
    rec->header.timestamp = 0; 
    rec->header.expiry = 0; 

    return rec;
}

void Record_destroy(Record *rec){
    free(rec->key);
    free(rec->value);
    free(rec);
}

//store record in datafile
void Record_store(Database *db, Record *rec, Meta *meta){
    size_t size = sizeof(Record) + rec->header.Ksize + rec->header.Vsize;
    fseek(db->datafile.reader, 0, SEEK_END);
    int fpos = ftell(db->datafile.reader);
    fwrite(rec, size, 1, db->datafile.writer);

    //create and store metadata
    meta->FileID = db->datafile.id;
    meta->RecordSize = size;
    meta->RecordPos = fpos;
    meta->Timestamp = rec->header.timestamp;        

    Metadata_append(db->datafile.writer, rec, meta);
}

//load record from datafile
Record *Record_load(Database *db, char *key){
    //TODO replace HT_entry with Meta struct
    HT_entry *metadata = ht_search(db->keyDir, key);

    uint32_t offset = metadata->val.RecordPos;
    uint32_t length = metadata->val.RecordSize;

    Record *rec = (Record*) malloc(sizeof(Record));
    fseek(db->datafile.reader, offset, SEEK_SET);
    fread(rec, length, 1, db->datafile.reader);

    return rec;
}
