#include "wildcard_mutator.h"

#pragma region Mutator internals

static char* readWildcard(char* varEnv)
{
    char* wildcardFilePath = getenv(varEnv);
    char* result = malloc(WILDCARD_FILE_READ_BUFFER_SIZE * sizeof(char));
    int fdWildcardFilePath;

    assert(result != (char*)NULL);

    if (wildcardFilePath == (char*)NULL)
    {
        perror("WILDCARD file not found");
    }
    else
    {
        fdWildcardFilePath = open(wildcardFilePath, O_RDONLY);

        read(fdWildcardFilePath, result, WILDCARD_FILE_READ_BUFFER_SIZE * sizeof(char));

        close(fdWildcardFilePath);
    }

    return result;
}

static struct wildcardMutatorIntRep* parseWildcard(char* wildcardContent)
{
    struct wildcardMutatorIntRep* result = malloc(sizeof(struct wildcardMutatorIntRep));
    char* wildcardContentPart = malloc(strlen(wildcardContent));

    assert(result != (struct wildcardMutatorIntRep*)NULL);
    assert(wildcardContentPart != (char*)NULL);

    strcpy(wildcardContentPart, wildcardContent);

    result->baseInput = stringList_init();

    strtok(wildcardContentPart, "*");
    stringList_add(&(result->baseInput), wildcardContentPart);

    while ((wildcardContentPart != (char*)NULL) && (wildcardContentPart != NULL))
    {
        wildcardContentPart = strtok((char*)NULL, "*");

        if (wildcardContentPart != (char*)NULL)
        {
            stringList_add(&(result->baseInput), wildcardContentPart);
        }
    }

    return result;
}

static char* generateRandomString()
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

static char* generateMutatedInput(struct wildcardMutatorIntRep* parsedWILDCARD, size_t maxSize)
{
    size_t generatedInputSize = min(maxSize, MAX_STRING_SIZE);
    char* generatedInput = calloc(generatedInputSize, sizeof(char));   // The memory is set to 0
    struct stringListNode* parsedWILDCARDStaticData = parsedWILDCARD->baseInput;
    size_t generatedInputLength = 0;
    size_t generatedInputOldLength = 0;
    size_t generatedInputRemainingSpace = generatedInputSize - 1;   // Trailing '\0'

    assert(generatedInput != (char*)NULL);

    while ((stringList_hasNext(parsedWILDCARDStaticData) == true) && (generatedInputRemainingSpace > 0))
    {
        strConcat(generatedInput, stringList_next(&parsedWILDCARDStaticData), generatedInputSize - 1);
        generatedInputLength += (strnlen(generatedInput, generatedInputSize) - generatedInputOldLength);
        generatedInputRemainingSpace -= generatedInputLength;
        generatedInputOldLength = generatedInputLength;

        if ((stringList_hasNext(parsedWILDCARDStaticData) == true) && (generatedInputRemainingSpace > 0))
        {
            strConcat(generatedInput, generateRandomString(), generatedInputSize - 1);
            generatedInputLength += (strnlen(generatedInput, generatedInputSize) - generatedInputOldLength);
            generatedInputRemainingSpace -= generatedInputLength;
            generatedInputOldLength = generatedInputLength;
        }
    }

    return generatedInput;
}

#pragma endregion

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
        struct wildcardMutator* mutator = malloc(sizeof(struct wildcardMutator));
        char* wildcardTest = "USER *\nUSER *\nPASSWD\nCWD *\nLS";
        struct wildcardMutatorIntRep* wildcardTestIntRep = parseWildcard(wildcardTest);

        mutator->intRep = wildcardTestIntRep;

        srand(5);

        printf("%s\n", generateMutatedInput(wildcardTestIntRep, MAX_STRING_SIZE));
        printf("%s\n", generateMutatedInput(wildcardTestIntRep, MAX_STRING_SIZE));
        printf("%s\n", generateMutatedInput(wildcardTestIntRep, MAX_STRING_SIZE));
        printf("%s\n", generateMutatedInput(wildcardTestIntRep, MAX_STRING_SIZE));

        afl_custom_deinit(mutator);

        return EXIT_SUCCESS;
    }
#endif

#pragma region AFL++ functions

struct wildcardMutator* afl_custom_init(afl_state_t *afl, unsigned int seed)
{
    struct wildcardMutator* mutator = malloc(sizeof(struct wildcardMutator));
    char* wildcard = readWildcard(ENV_WILDCARD_FILE_PATH);

    assert(mutator != (struct wildcardMutator*)NULL);
    assert(wildcard != (char*)NULL);

    srand(seed);    // Random initialization

    mutator->afl = afl;
    mutator->intRep = parseWildcard(wildcard);

    free(wildcard);

    OKF("WILDCARD Mutator ready !\n");

    return mutator;
}

size_t afl_custom_fuzz(struct wildcardMutator* data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size)
{
    char* mutatedInput;
    size_t mutatedInputLength;

    // Mutate function call to add here
    mutatedInput = generateMutatedInput(data->intRep, max_size);

    *out_buf = (unsigned char*)mutatedInput;
    mutatedInputLength = strlen(mutatedInput);

    return mutatedInputLength;
}

void afl_custom_deinit(struct wildcardMutator* data)
{
    free(data->intRep);
    free(data);
}

#pragma endregion