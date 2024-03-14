#include "csl_int_rep_list.h"

struct cslMutatorIntRepListNode* cslMutatorIntRepList_init()
{
    return (struct cslMutatorIntRepListNode*)NULL;
}

void cslMutatorIntRepList_add(struct cslMutatorIntRepListNode** cslMutatorIntRepList, struct cslMutatorIntRep* contentToAdd)
{
    struct cslMutatorIntRepListNode* currentNode = *cslMutatorIntRepList;

    if (currentNode == (struct cslMutatorIntRepListNode*)NULL)
    {
        currentNode = malloc(sizeof(struct cslMutatorIntRepListNode));

        assert(currentNode != (struct cslMutatorIntRepListNode*)NULL);

        currentNode->nodeContent = contentToAdd;
        currentNode->nextNode = (struct cslMutatorIntRepListNode*)NULL;

        *cslMutatorIntRepList = currentNode;
    }
    else
    {
        while (currentNode->nextNode != (struct cslMutatorIntRepListNode*)NULL)
        {
            currentNode = currentNode->nextNode;
        }

        currentNode->nextNode = malloc(sizeof(struct cslMutatorIntRepListNode));

        assert(currentNode->nextNode != (struct cslMutatorIntRepListNode*)NULL);

        currentNode->nextNode->nodeContent = contentToAdd;
        currentNode->nextNode->nextNode = (struct cslMutatorIntRepListNode*)NULL;
    }
}

void cslMutatorIntRepList_free(struct cslMutatorIntRepListNode** cslMutatorIntRepList)
{
    if ((*cslMutatorIntRepList) != (struct cslMutatorIntRepListNode*)NULL)
    {
        cslMutatorIntRepList_free(&((*cslMutatorIntRepList)->nextNode));
    }

    free(*cslMutatorIntRepList);
    *cslMutatorIntRepList = (struct cslMutatorIntRepListNode*)NULL;
}