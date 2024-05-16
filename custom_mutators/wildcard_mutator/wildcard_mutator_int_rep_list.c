#include "wildcard_mutator_int_rep_list.h"

struct wildcardMutatorIntRepListNode* wildcardMutatorIntRepList_init()
{
    return (struct wildcardMutatorIntRepListNode*)NULL;
}

void wildcardMutatorIntRepList_add(struct wildcardMutatorIntRepListNode** listPtr, struct wildcardMutatorIntRep* contentToAdd)
{
    struct wildcardMutatorIntRepListNode* currentNode = *listPtr;

    if (currentNode == (struct wildcardMutatorIntRepListNode*)NULL)
    {
        currentNode = malloc(sizeof(struct wildcardMutatorIntRepListNode));

        assert(currentNode != (struct wildcardMutatorIntRepListNode*)NULL);

        currentNode->nodeContent = contentToAdd;
        currentNode->nextNode = (struct wildcardMutatorIntRepListNode*)NULL;

        *listPtr = currentNode;
    }
    else
    {
        while (currentNode->nextNode != (struct wildcardMutatorIntRepListNode*)NULL)
        {
            currentNode = currentNode->nextNode;
        }

        currentNode->nextNode = malloc(sizeof(struct wildcardMutatorIntRepListNode));

        assert(currentNode->nextNode != (struct wildcardMutatorIntRepListNode*)NULL);

        currentNode->nextNode->nodeContent = contentToAdd;
        currentNode->nextNode->nextNode = (struct wildcardMutatorIntRepListNode*)NULL;
    }
}

void wildcardMutatorIntRepList_free(struct wildcardMutatorIntRepListNode** listPtr)
{
    if ((*listPtr) != (struct wildcardMutatorIntRepListNode*)NULL)
    {
        wildcardMutatorIntRepList_free(&((*listPtr)->nextNode));
    }

    free(*listPtr);
    *listPtr = (struct wildcardMutatorIntRepListNode*)NULL;
}

struct wildcardMutatorIntRep* wildcardMutatorIntRepList_get(struct wildcardMutatorIntRepListNode* list, int i)
{
    int currentI = 0;
    struct wildcardMutatorIntRep* result = (struct wildcardMutatorIntRep*)NULL;
    struct wildcardMutatorIntRepListNode* currentNode = list;

    while ((currentI < i) && (currentNode != (struct wildcardMutatorIntRepListNode*)NULL))
    {
        currentNode = currentNode->nextNode;
    }

    if (currentNode != (struct wildcardMutatorIntRepListNode*)NULL)
    {
        result = currentNode->nodeContent;
    }

    return result;
}

void wildcardMutatorIntRepList_iter(struct wildcardMutatorIntRepListNode* list, void (*action)(struct wildcardMutatorIntRep* str))
{
    struct wildcardMutatorIntRepListNode* currentNode = list;

    while (currentNode != (struct wildcardMutatorIntRepListNode*)NULL)
    {
        action(currentNode->nodeContent);

        currentNode = currentNode->nextNode;
    }
}

int wildcardMutatorIntRepList_length(struct wildcardMutatorIntRepListNode* list)
{
    int result = 0;
    struct wildcardMutatorIntRepListNode* currentNode = list;

    while (currentNode != (struct wildcardMutatorIntRepListNode*)NULL)
    {
        result++;

        currentNode = currentNode->nextNode;
    }

    return result;
}