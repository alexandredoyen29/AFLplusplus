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
    char* cslContentPart = malloc(strlen(cslContent));

    assert(result != (struct cslMutatorIntRep*)NULL);
    assert(cslContentPart != (char*)NULL);

    strcpy(cslContentPart, cslContent);

    result->baseInput = stringList_init();

    strtok(cslContentPart, "*");
    stringList_addString(&(result->baseInput), cslContentPart);

    while ((cslContentPart != (char*)NULL) && (cslContentPart != NULL))
    {
        cslContentPart = strtok((char*)NULL, "*");

        if (cslContentPart != (char*)NULL)
        {
            stringList_addString(&(result->baseInput), cslContentPart);
        }
    }

    return result;
}

#ifdef DEBUG
    // DEBUG
    void stringList_printStringList(struct stringListNode** stringList)
    {
        struct stringListNode* currentNode = *stringList;
        int i = 0;

        while (currentNode != (struct stringListNode*)NULL)
        {
            printf("list[%d] = %s\n", i, currentNode->nodeContent);
            currentNode = currentNode->nextNode;
            ++i;
        }
    }
#endif

// TODO : Sera à retirer à la fin (Ou du moins quand la partie AFL++ sera implémentée)
int main()
{
    char* test = "USER *\nLIST\nCWD *\nPWD";
    struct cslMutatorIntRep* testResult;

    testResult = parseCsl(test);

    stringList_printStringList(&(testResult->baseInput));

    return EXIT_SUCCESS;
}