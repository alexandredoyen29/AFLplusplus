#include "string_list.h"

struct stringListNode* stringList_init()
{
    return (struct stringListNode*)NULL;
}

void stringList_add(struct stringListNode** stringListPtr, char* contentToAdd)
{
    struct stringListNode* currentNode = *stringListPtr;

    if (currentNode == (struct stringListNode*)NULL)
    {
        currentNode = malloc(sizeof(struct stringListNode));

        assert(currentNode != (struct stringListNode*)NULL);

        currentNode->nodeContent = contentToAdd;
        currentNode->nextNode = (struct stringListNode*)NULL;

        *stringListPtr = currentNode;
    }
    else
    {
        while (currentNode->nextNode != (struct stringListNode*)NULL)
        {
            currentNode = currentNode->nextNode;
        }

        currentNode->nextNode = malloc(sizeof(struct stringListNode));

        assert(currentNode->nextNode != (struct stringListNode*)NULL);

        currentNode->nextNode->nodeContent = contentToAdd;
        currentNode->nextNode->nextNode = (struct stringListNode*)NULL;
    }
}

void stringList_free(struct stringListNode** stringListPtr)
{
    if ((*stringListPtr) != (struct stringListNode*)NULL)
    {
        stringList_free(&((*stringListPtr)->nextNode));
    }

    free(*stringListPtr);
    *stringListPtr = (struct stringListNode*)NULL;
}

void stringList_iter(struct stringListNode* stringList, void (*action)(char* str))
{
    struct stringListNode* currentNode = stringList;

    while (currentNode != (struct stringListNode*)NULL)
    {
        action(currentNode->nodeContent);

        currentNode = currentNode->nextNode;
    }
}

void stringList_iteri(struct stringListNode* stringList, void (*action)(char* str, int i))
{
    struct stringListNode* currentNode = stringList;
    int i = 0;

    while (currentNode != (struct stringListNode*)NULL)
    {
        action(currentNode->nodeContent, i);

        currentNode = currentNode->nextNode;
        i = i + 1;
    }
}

void stringList_iterd(struct stringListNode* stringList, void (*action)(char* str, void* data), void* data)
{
    struct stringListNode* currentNode = stringList;
    int i = 0;

    while (currentNode != (struct stringListNode*)NULL)
    {
        action(currentNode->nodeContent, data);

        currentNode = currentNode->nextNode;
        i = i + 1;
    }
}

bool stringList_hasNext(struct stringListNode* stringList)
{
    return (stringList != (struct stringListNode*)NULL);
}

char* stringList_next(struct stringListNode** stringListPtr)
{
    char* result = (char*)NULL;
    struct stringListNode* list = *stringListPtr;

    if (list != (struct stringListNode*)NULL)
    {
        result = list->nodeContent;
        *stringListPtr = list->nextNode;
    }

    return result;
}

void stringList_map_inplace(struct stringListNode* stringList, void (*transformation)(char** str))
{
    struct stringListNode* currentNode = stringList;

    while (currentNode != (struct stringListNode*)NULL)
    {
        transformation(&currentNode->nodeContent);

        currentNode = currentNode->nextNode;
    }
}

void stringList_map_inplace_d(struct stringListNode* stringList, void (*transformation)(char** str, void* data), void* data)
{
    struct stringListNode* currentNode = stringList;

    while (currentNode != (struct stringListNode*)NULL)
    {
        transformation(&currentNode->nodeContent, data);

        currentNode = currentNode->nextNode;
    }
}