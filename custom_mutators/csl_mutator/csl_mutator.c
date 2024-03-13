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

int readCslFromEnv_CSL_FILE(char* cslContent, size_t cslContentSize)
{
    char* filename = getenv("CSL_FILE");
    int result;
    FILE* file;

    if (filename == (char*)NULL)
    {
        result = 1;
    }
    else
    {
        file = fopen(filename, "r");
        fread(cslContent, 1, cslContentSize, file);
        fclose(file);

        result = 0;
    }

    return result;
}

#ifdef DEBUG
    // DEBUG
    void printStr(char* str)
    {
        printf("%s\n", str);
    }

    void printStri(char* str, int i)
    {
        printf("tab[%d] = %s\n", i, str);
    }

    void stringList_printStringList(struct stringListNode** stringList)
    {
        stringList_iteri(stringList, printStri);
    }
#endif

struct cslMutator* afl_custom_init(afl_state_t *afl, unsigned int seed)
{
    struct cslMutator* mutator = malloc(sizeof(struct cslMutator));
    char cslContent[CSL_MAX_SIZE];
    int cslReadResult;

    assert(mutator != (struct cslMutator*)NULL);

    srand(seed);    // Random initialization

    // CSL Mutator initialization
    cslReadResult = readCslFromEnv_CSL_FILE(cslContent, CSL_MAX_SIZE);
    assert(cslReadResult == 0);
    mutator->cslMutator = parseCsl(cslContent);

    mutator->afl = afl;

    return mutator;
}

// TODO : Sera à retirer à la fin (Ou du moins quand la partie AFL++ sera implémentée)
int main()
{
    char test[CSL_MAX_SIZE];
    struct cslMutatorIntRep* testResult;

    readCslFromEnv_CSL_FILE(test, CSL_MAX_SIZE);
    testResult = parseCsl(test);

    stringList_printStringList(&(testResult->baseInput));

    stringList_free(&(testResult->baseInput));

    return EXIT_SUCCESS;
}