#include "csl_int_rep_list.h"

struct cslMutatorIntRepListNode* cslMutatorIntRepList_init()
{
    return (struct cslMutatorIntRepListNode*)NULL;
}

void cslMutatorIntRepList_add(struct cslMutatorIntRepListNode** listPtr, struct cslMutatorIntRep* contentToAdd)
{
    struct cslMutatorIntRepListNode* currentNode = *listPtr;

    if (currentNode == (struct cslMutatorIntRepListNode*)NULL)
    {
        currentNode = malloc(sizeof(struct cslMutatorIntRepListNode));

        assert(currentNode != (struct cslMutatorIntRepListNode*)NULL);

        currentNode->nodeContent = contentToAdd;
        currentNode->nextNode = (struct cslMutatorIntRepListNode*)NULL;

        *listPtr = currentNode;
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

void cslMutatorIntRepList_free(struct cslMutatorIntRepListNode** listPtr)
{
    if ((*listPtr) != (struct cslMutatorIntRepListNode*)NULL)
    {
        cslMutatorIntRepList_free(&((*listPtr)->nextNode));
    }

    free(*listPtr);
    *listPtr = (struct cslMutatorIntRepListNode*)NULL;
}

struct cslMutatorIntRep* cslMutatorIntRepList_get(struct cslMutatorIntRepListNode* list, int i)
{
    int currentI = 0;
    struct cslMutatorIntRep* result = (struct cslMutatorIntRep*)NULL;
    struct cslMutatorIntRepListNode* currentNode = list;

    while ((currentI < i) && (currentNode != (struct cslMutatorIntRepListNode*)NULL))
    {
        currentNode = currentNode->nextNode;
    }

    if (currentNode != (struct cslMutatorIntRepListNode*)NULL)
    {
        result = currentNode->nodeContent;
    }

    return result;
}