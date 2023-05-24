#include "btree.h"

int main(void){
    return 0;
}

static void split(struct BtreeNode **node, uint8_t m){
    //splits an m-node into 2 (m/2)-nodes 
}

////////////////////////////////////////////////////////////////////////////////
/// TREE
////////////////////////////////////////////////////////////////////////////////

Btree *Btree_create(uint8_t degree){
    Btree *btree = (Btree*) calloc(1, sizeof(Btree)); 
    btree->degree = degree;
    return btree;
}

void Btree_insert(Btree *btree, uint32_t key, void *val){
    BtreeNode *node = BtreeNode_create(btree->degree);
    node->keys[0] = key;
    node->record = val;
    node->size = 1;


}

static void Btree_free_rec(BtreeNode *node){
    if(node == NULL) return;

    for(int i = 0; i < node->size; i++){
        Btree_free_rec(node->children[i]);
    }
    BtreeNode_free(node);
}

void Btree_free(Btree *btree){
    Btree_free_rec(btree->root);
    free(btree);
}

////////////////////////////////////////////////////////////////////////////////
/// NODE
////////////////////////////////////////////////////////////////////////////////

BtreeNode *BtreeNode_create(uint8_t n){
    BtreeNode *btnode = (BtreeNode*) calloc(1, sizeof(BtreeNode));
    btnode->children = calloc(n, sizeof(void*));
    btnode->keys = calloc(n, sizeof(uint32_t));
    return btnode;
}



void BtreeNode_free(BtreeNode *node){
    free(node->children);
    free(node->keys);
    //TODO free record
    free(node);
}

