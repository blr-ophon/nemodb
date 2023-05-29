#include "kvstore.h"
#include "hashtable.h"
#include "nemodb.h"

static void ferror_check(FILE *f, int rv){
    if (rv ==  0) {
        if (feof(f)) {
            fprintf(stderr, "Error: Reached end of file.\n");
        } else if (ferror(f)) {
            perror("Error reading from file");
        } else {
            fprintf(stderr, "Unknown error occurred during file read.\n");
        }
        fclose(f);
        exit(1);
    }
}

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

void Record_free(Record *rec){
    free(rec->key);
    free(rec->value);
    free(rec);
}

//store record in datafile, meta receives info on how the record was stored
void Record_store(Database *db, Record *rec, Meta *meta){
    //APPEND RECORD TO DATAFILE
    
    //set cursor
    size_t size = sizeof(Record) + rec->header.Ksize + rec->header.Vsize;
    fseek(db->datafile.writer, 0, SEEK_END);
    uint32_t fpos = ftell(db->datafile.writer);

    //write header, key and value in order
    fwrite(&rec->header, sizeof(RecHeader), 1, db->datafile.writer);
    for(uint32_t i = 0; i < rec->header.Ksize; i++){
        fwrite(&rec->key[i], 1, 1, db->datafile.writer);
    }
    for(uint32_t i = 0; i < rec->header.Vsize; i++){
        fwrite(&rec->value[i], 1, 1, db->datafile.writer);
    }

    //create metadata
    meta->FileID = db->datafile.id;
    meta->RecordSize = size;
    meta->RecordPos = fpos;
    meta->Timestamp = rec->header.timestamp;        
}


//load record from datafile
Record *Record_load(Database *db, char *key){
    Record *rec = (Record*) malloc(sizeof(Record));

    Meta *metadata = Metadata_retrieve(db->keyDir, key);
    printf("Metadata(%u(id) | %u(pos) | %u(size) | %u(ts))\n",
            metadata->FileID,
            metadata->RecordPos,
            metadata->RecordSize,
            metadata->Timestamp
          );

    //set cursor
    fseek(db->datafile.reader, metadata->RecordPos, SEEK_SET);
    printf("FTELL: %li\n", ftell(db->datafile.reader));

    //read header, key and value in order
    int rv = fread(&rec->header, sizeof(RecHeader), 1, db->datafile.reader);
    ferror_check(db->datafile.reader, rv);
    printf("Header(%u(cs) | %u(ex) | %u(ts) | %u(ksize) | %u(vsize))\n",
            rec->header.checksum,
            rec->header.expiry,
            rec->header.timestamp,
            rec->header.Ksize,
            rec->header.Vsize
          );
    for(uint32_t i = 0; i < rec->header.Ksize; i++){
        fread(&rec->key[i], 1, 1, db->datafile.reader);
    }
    for(uint32_t i = 0; i < rec->header.Vsize; i++){
        fread(&rec->value[i], 1, 1, db->datafile.reader);
    }
    return rec;
}
