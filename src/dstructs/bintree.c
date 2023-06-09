#include <stdio.h>
#include "bintree.h"

//int main(void){
//    struct bintNode *root= bint_create();
//    for(int i = 0; i < 10; i++){
//        bint_insert(&root, i);
//    }
//
//    bint_delete(&root, 8);
//    bint_delete(&root, 15);
//    bint_delete(&root, 3);
//    bint_delete(&root, 2);
//    bint_delete(&root, 5);
//
//    for(int i = 0; i < 20; i++){
//        if(bint_search(root, i)){
//            printf("%d found\n", i);
//        }else{
//            printf("%d Not found\n", i);
//        }
//    }
//    return 0;
//}

struct bintNode *bint_create(void){ 
    return calloc(1, sizeof(struct bintNode));
}


bool bint_search(struct bintNode *head, uint32_t val){
    struct bintNode *crnt;
    crnt = head;

    while(crnt->val != val){
        crnt = val < crnt->val? crnt->Lnode : crnt->Rnode;        
        if(!crnt) return false;
    }

    return true;
}


void bint_insert(struct bintNode **head, uint32_t val){
    //current and previous node
    struct bintNode *crnt = *head;
    struct bintNode *prev = NULL;

    while(crnt != NULL){
        prev = crnt;    
        crnt = val < crnt->val? crnt->Lnode : crnt->Rnode;        
    }
    
    crnt = calloc(1, sizeof(struct bintNode));
    crnt->val = val;

    if(prev == NULL){
        //empty tree
        *head = crnt;
        return;
    }

    //link to parent node
    if(crnt->val < prev->val){
        prev->Lnode = crnt;
    }else{
        prev->Rnode = crnt;
    }
}


void bint_delete(struct bintNode **head, uint32_t val){
    //current and previous node
    struct bintNode *crnt = *head;
    struct bintNode *prev = NULL;

    while(crnt->val != val){
        prev = crnt;
        crnt = val < crnt->val? crnt->Lnode : crnt->Rnode;        
        if(!crnt) return;  //val not in tree
    }
    if(prev == NULL){
        //TODO: delete root node
        return;
    }
    
    if(crnt->Rnode == NULL){ //crnt has no right subtree
        if(crnt->Lnode == NULL){
            //crnt is a leaf: Delete and unlink from parent
            if(val < prev->val){
                prev->Lnode = NULL;
            }else{
                prev->Rnode = NULL;
            }
            free(crnt);
        }else{
            //crnt has left node that will take its place
            if(val < prev->val){
                prev->Lnode = crnt->Lnode;
            }else{
                prev->Rnode = crnt->Lnode;
            }
            free(crnt);
        }

    }else{ //crnt swaps with the leftmost node from its right subtree
           
        //find leftmost node
        struct bintNode *tmpCrnt = crnt->Rnode;
        struct bintNode *tmpPrev = NULL;
        while(tmpCrnt->Lnode != NULL){
            tmpPrev = tmpCrnt;
            tmpCrnt = tmpCrnt->Lnode;
        }

        //rearrange links if necessary
        if(tmpPrev){
            tmpPrev->Lnode = tmpCrnt->Rnode;
            tmpCrnt->Rnode = tmpPrev;
        }

        if(val < prev->val){
            prev->Lnode = tmpCrnt;
        }else{
            prev->Rnode = tmpCrnt;
        }
        free(crnt);
    }
}

