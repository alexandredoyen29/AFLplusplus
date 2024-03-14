#include "string_list.h"

struct stringListNode* stringList_init()
{
    return (struct stringListNode*)NULL;
}

void stringList_add(struct stringListNode** stringList, char* contentToAdd)
{
    struct stringListNode* currentNode = *stringList;

    if (currentNode == (struct stringListNode*)NULL)
    {
        currentNode = malloc(sizeof(struct stringListNode));

        assert(currentNode != (struct stringListNode*)NULL);

        currentNode->nodeContent = contentToAdd;
        currentNode->nextNode = (struct stringListNode*)NULL;

        *stringList = currentNode;
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

void stringList_free(struct stringListNode** stringList)
{
    if ((*stringList) != (struct stringListNode*)NULL)
    {
        stringList_free(&((*stringList)->nextNode));
    }

    free(*stringList);
    *stringList = (struct stringListNode*)NULL;
}

void stringList_iter(struct stringListNode** stringList, void (*action)(char* str))
{
    struct stringListNode* currentNode = *stringList;

    while (currentNode != (struct stringListNode*)NULL)
    {
        action(currentNode->nodeContent);

        currentNode = currentNode->nextNode;
    }
}

void stringList_iteri(struct stringListNode** stringList, void (*action)(char* str, int i))
{
    struct stringListNode* currentNode = *stringList;
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