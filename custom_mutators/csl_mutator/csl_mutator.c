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
        char* cslTest = "USER *\nUSER *\nPASSWD\nCWD *\nLS";
        struct cslMutatorIntRepListNode* mutator = cslMutatorIntRepList_init();
        struct cslMutatorIntRep* cslTestIntRep = parseCsl(cslTest);

        cslMutatorIntRepList_add(&mutator, cslTestIntRep);

        //stringList_iteri(&(cslTestIntRep->baseInput), printStri);
        //stringList_iteri(cslMutatorIntRepList_get(mutator, 0)->baseInput, printStri);

        srand(time(NULL));

        printf("%s\n", generateMutatedInput(cslTestIntRep, 3));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 4));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 5));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 6));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 7));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 8));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 9));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 10));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 11));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 12));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 13));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 14));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 15));
        printf("%s\n", generateMutatedInput(cslTestIntRep, 16));

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

    cslMutatorIntRepList_add(&(data->cslMutatorsList), parseCsl((char*)buf));
    currentMutationId = data->currentMutationId;
    data->currentMutationId++;

    // Mutate function call to add here
    mutatedInput = generateMutatedInput(cslMutatorIntRepList_get(data->cslMutatorsList, currentMutationId), max_size);

    *out_buf = (unsigned char*)mutatedInput;

    return 0;
}

#pragma endregion