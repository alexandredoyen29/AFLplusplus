#ifndef WILDCARD_MUTATOR_INT_REP_LIST_H
#define WILDCARD_MUTATOR_INT_REP_LIST_H

#include "wildcard_mutator.h"

#include <stdlib.h>

struct wildcardMutatorIntRepListNode
{
    struct wildcardMutatorIntRep* nodeContent;
    struct wildcardMutatorIntRepListNode* nextNode;
};

struct wildcardMutatorIntRepListNode* wildcardMutatorIntRepList_init();
void wildcardMutatorIntRepList_add(struct wildcardMutatorIntRepListNode** listPtr, struct wildcardMutatorIntRep* contentToAdd);
void wildcardMutatorIntRepList_free(struct wildcardMutatorIntRepListNode** listPtr);
struct wildcardMutatorIntRep* wildcardMutatorIntRepList_get(struct wildcardMutatorIntRepListNode* list, int i);
void wildcardMutatorIntRepList_iter(struct wildcardMutatorIntRepListNode* list, void (*action)(struct wildcardMutatorIntRep* str));
int wildcardMutatorIntRepList_length(struct wildcardMutatorIntRepListNode* list);

#endif