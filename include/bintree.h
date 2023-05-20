#ifndef BINTREE_H
#define BINTREE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct bintNode{
    uint32_t val;
    struct bintNode *Lnode;
    struct bintNode *Rnode;
};

void bint_insert(struct bintNode **head, uint32_t val);

void bint_delete(struct bintNode **head, uint32_t val);

bool bint_search(struct bintNode *head, uint32_t val);

#endif
