#ifndef CSL_INT_REP_LIST_H
#define CSL_INT_REP_LIST_H

#include "csl_mutator.h"

#include <stdlib.h>

struct cslMutatorIntRepListNode
{
    struct cslMutatorIntRep* nodeContent;
    struct cslMutatorIntRepListNode* nextNode;
};

struct cslMutatorIntRepListNode* cslMutatorIntRepList_init();
void cslMutatorIntRepList_add(struct cslMutatorIntRepListNode** list, struct cslMutatorIntRep* contentToAdd);
void cslMutatorIntRepList_free(struct cslMutatorIntRepListNode** list);
struct cslMutatorIntRep* cslMutatorIntRepList_get(struct cslMutatorIntRepListNode* list, int i);

#endif