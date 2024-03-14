#include "csl_mutator.h"

struct cslMutatorIntRep* parseCsl(char* cslContent)
{
    struct cslMutatorIntRep* result = malloc(sizeof(struct cslMutatorIntRep));
    char* cslContentPart = malloc(strlen(cslContent));

    assert(result != (struct cslMutatorIntRep*)NULL);
    assert(cslContentPart != (char*)NULL);

    strcpy(cslContentPart, cslContent);

    result->baseInput = stringList_init();

    strtok(cslContentPart, "*");
    stringList_add(&(result->baseInput), cslContentPart);

    while ((cslContentPart != (char*)NULL) && (cslContentPart != NULL))
    {
        cslContentPart = strtok((char*)NULL, "*");

        if (cslContentPart != (char*)NULL)
        {
            stringList_add(&(result->baseInput), cslContentPart);
        }
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

int main()
{
    return EXIT_SUCCESS;
}

#pragma region AFL++ functions

struct cslMutator* afl_custom_init(afl_state_t *afl, unsigned int seed)
{
    struct cslMutator* mutator = malloc(sizeof(struct cslMutator));

    assert(mutator != (struct cslMutator*)NULL);

    srand(seed);    // Random initialization

    mutator->cslMutatorsList = cslMutatorIntRepList_init();
    mutator->afl = afl;

    return mutator;
}

size_t afl_custom_fuzz(struct cslMutator* data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size)
{
    return 0;
}

#pragma endregion