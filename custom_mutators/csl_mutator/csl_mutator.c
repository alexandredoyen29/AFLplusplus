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

// Fonctions internes pour le mutateur
void fail()
{
    exit(EXIT_FAILURE);
}

struct cslMutatorIntRep* parseCsl(char* cslContent)
{
    struct cslMutatorIntRep* result = malloc(sizeof(struct cslMutatorIntRep));

    assert(result != NULL);

    return result;
}

// TODO : Sera à retirer à la fin (Ou du moins quand la partie AFL++ sera implémentée)
int main()
{
}