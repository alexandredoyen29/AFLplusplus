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

char* generateRandomString()
{
    char* alphabet = MUTATION_ALPHABET;
    size_t size = (size_t)rand() % MAX_RANDOM_STRING_SIZE;
    size_t sizeofAlphabet = strlen(alphabet);
    int i = 0;
    char* result = malloc((size + 1) * sizeof(char));

    assert(result != (char*)NULL);
    
    for (i = 0; i < size; ++i)
    {
        result[i] = alphabet[rand() % sizeofAlphabet];
    }

    return result;
}

char* generateMutatedInput(struct cslMutatorIntRep* parsedCSL, size_t maxSize)
{
    size_t generatedInputSize = min(maxSize, MAX_STRING_SIZE);
    char* generatedInput = calloc(generatedInputSize, sizeof(char));   // The memory is set to 0
    struct stringListNode* parsedCSLStaticData = parsedCSL->baseInput;
    size_t generatedInputLength = 0;
    size_t generatedInputOldLength = 0;
    size_t generatedInputRemainingSpace = generatedInputSize - 1;   // Trailing '\0'

    assert(generatedInput != (char*)NULL);

    while ((stringList_hasNext(parsedCSLStaticData) == true) && (generatedInputRemainingSpace > 0))
    {
        strConcat(generatedInput, stringList_next(&parsedCSLStaticData), generatedInputSize - 1);
        generatedInputLength += (strnlen(generatedInput, generatedInputSize) - generatedInputOldLength);
        generatedInputRemainingSpace -= generatedInputLength;
        generatedInputOldLength = generatedInputLength;

        if ((stringList_hasNext(parsedCSLStaticData) == true) && (generatedInputRemainingSpace > 0))
        {
            strConcat(generatedInput, generateRandomString(), generatedInputSize - 1);
            generatedInputLength += (strnlen(generatedInput, generatedInputSize) - generatedInputOldLength);
            generatedInputRemainingSpace -= generatedInputLength;
            generatedInputOldLength = generatedInputLength;
        }
    }

    return generatedInput;
}

void cslMutatorIntRep_free(struct cslMutatorIntRep* target)
{
    stringList_free(&(target->baseInput));
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

    void stringList_printStringList(struct stringListNode* stringList)
    {
        stringList_iteri(stringList, printStri);
    }

    int main()
    {
        struct cslMutator* mutator = malloc(sizeof(struct cslMutator));
        char* cslTest = "USER *\nUSER *\nPASSWD\nCWD *\nLS";
        struct cslMutatorIntRepListNode* mutatorIntRepList = cslMutatorIntRepList_init();
        struct cslMutatorIntRep* cslTestIntRep = parseCsl(cslTest);

        mutator->cslMutatorsList = mutatorIntRepList;

        cslMutatorIntRepList_add(&mutatorIntRepList, cslTestIntRep);

        //stringList_iteri(&(cslTestIntRep->baseInput), printStri);
        //stringList_iteri(cslMutatorIntRepList_get(mutator, 0)->baseInput, printStri);

        srand(5);

        printf("%s\n", generateMutatedInput(cslTestIntRep, MAX_STRING_SIZE));
        printf("%s\n", generateMutatedInput(cslTestIntRep, MAX_STRING_SIZE));
        printf("%s\n", generateMutatedInput(cslTestIntRep, MAX_STRING_SIZE));
        printf("%s\n", generateMutatedInput(cslTestIntRep, MAX_STRING_SIZE));

        afl_custom_deinit(mutator);

        return EXIT_SUCCESS;
    }
#endif

#pragma region AFL++ functions

struct cslMutator* afl_custom_init(afl_state_t *afl, unsigned int seed)
{
    struct cslMutator* mutator = malloc(sizeof(struct cslMutator));

    assert(mutator != (struct cslMutator*)NULL);

    srand(seed);    // Random initialization

    mutator->currentMutationId = 0;
    mutator->cslMutatorsList = cslMutatorIntRepList_init();
    mutator->afl = afl;

    return mutator;
}

size_t afl_custom_fuzz(struct cslMutator* data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size)
{
    char* mutatedInput;
    int currentMutationId;
    size_t mutatedInputLength;

    cslMutatorIntRepList_add(&(data->cslMutatorsList), parseCsl((char*)buf));
    currentMutationId = data->currentMutationId;
    data->currentMutationId++;

    // Mutate function call to add here
    mutatedInput = generateMutatedInput(cslMutatorIntRepList_get(data->cslMutatorsList, currentMutationId), max_size);

    *out_buf = (unsigned char*)mutatedInput;
    mutatedInputLength = strlen(mutatedInput);

    return mutatedInputLength;
}

void afl_custom_deinit(struct cslMutator* data)
{
    struct cslMutatorIntRepListNode* mutatorIntRepList = data->cslMutatorsList;

    cslMutatorIntRepList_iter(mutatorIntRepList, cslMutatorIntRep_free);
    cslMutatorIntRepList_free(&(data->cslMutatorsList));

    free(data);
}

#pragma endregion