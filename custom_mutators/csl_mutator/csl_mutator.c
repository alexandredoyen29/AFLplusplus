#include "csl_mutator.h"

// String lists
struct stringListNode* stringList_init()
{
    return (struct stringListNode*)NULL;
}

void stringList_addString(struct stringListNode** stringList, char* contentToAdd)
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

struct cslMutatorIntRep* parseCsl(char* cslContent)
{
    struct cslMutatorIntRep* result = malloc(sizeof(struct cslMutatorIntRep));
    char* cslContentPart;

    assert(result != NULL);

    result->baseInput = stringList_init();

    cslContentPart = strtok(cslContent, "*");
    stringList_addString(&(result->baseInput), cslContentPart);

    while (cslContentPart != (char*)NULL)
    {
        cslContentPart = strtok(cslContent, "*");
        stringList_addString(&(result->baseInput), cslContentPart);
    }

    return result;
}

#ifdef DEBUG
    // DEBUG
    void stringList_printStringList(struct stringListNode** stringList)
    {
        struct stringListNode* currentNode = *stringList;

        while (currentNode != (struct stringListNode*)NULL)
        {
            printf("%s\n", currentNode->nodeContent);
            currentNode = currentNode->nextNode;
        }
    }
#endif

// TODO : Sera à retirer à la fin (Ou du moins quand la partie AFL++ sera implémentée)
int main()
{
    char* test = "USER *";
    struct cslMutatorIntRep* testResult;

    testResult = parseCsl(test);

    stringList_printStringList(&(testResult->baseInput));

    return EXIT_SUCCESS;
}