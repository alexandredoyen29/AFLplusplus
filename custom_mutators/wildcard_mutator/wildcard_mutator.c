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

    while (wildcardContentPart != (char*)NULL)
    {
        wildcardContentPart = strtok((char*)NULL, "*");

        if (wildcardContentPart != (char*)NULL)
        {
            stringList_add(&(result->baseInput), wildcardContentPart);
        }
    }

    return result;
}

static void generateMutatedInput(struct wildcardMutatorIntRep* parsedWildcard, char* wildcardReplacement, char* outBuffer, size_t outBufferSize)
{
    struct stringListNode* parsedWildcardStaticData = parsedWildcard->baseInput;
    size_t outBufferLength = 0;
    size_t outBufferOldLength = 0;
    size_t outBufferRemainingSpace = outBufferSize - 1;   // Trailing '\0'

    while ((stringList_hasNext(parsedWildcardStaticData) == true) && (outBufferRemainingSpace > 0))
    {
        strConcat(outBuffer, stringList_next(&parsedWildcardStaticData), outBufferSize - 1);
        outBufferLength += (strnlen(outBuffer, outBufferSize) - outBufferOldLength);
        outBufferRemainingSpace -= outBufferLength;
        outBufferOldLength = outBufferLength;

        if ((stringList_hasNext(parsedWildcardStaticData) == true) && (outBufferRemainingSpace > 0))
        {
            strConcat(outBuffer, wildcardReplacement, outBufferSize - 1);
            outBufferLength += (strnlen(outBuffer, outBufferSize) - outBufferOldLength);
            outBufferRemainingSpace -= outBufferLength;
            outBufferOldLength = outBufferLength;
        }
    }
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
        char* wildcardTest = "a *; b *; quit";
        struct wildcardMutatorIntRep* wildcardTestIntRep = parseWildcard(wildcardTest);

        mutator->intRep = wildcardTestIntRep;
        mutator->mutatedOutBufferSize = MAX_STRING_SIZE;
        mutator->mutatedOutBuffer = calloc(mutator->mutatedOutBufferSize, sizeof(char));

        srand(5);

        generateMutatedInput(wildcardTestIntRep, "toto", mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        printf("--\n%s\n", mutator->mutatedOutBuffer);

        strReset(mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        generateMutatedInput(wildcardTestIntRep, "titi", mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        printf("--\n%s\n", mutator->mutatedOutBuffer);

        strReset(mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        generateMutatedInput(wildcardTestIntRep, "tutu", mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        printf("--\n%s\n", mutator->mutatedOutBuffer);

        strReset(mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        generateMutatedInput(wildcardTestIntRep, "tata", mutator->mutatedOutBuffer, mutator->mutatedOutBufferSize);
        printf("--\n%s\n", mutator->mutatedOutBuffer);

        afl_custom_deinit(mutator);

        return EXIT_SUCCESS;
    }
#endif

#pragma region AFL++ functions

struct wildcardMutator* afl_custom_init(afl_state_t *afl, unsigned int seed)
{
    struct wildcardMutator* mutator = (struct wildcardMutator*)malloc(sizeof(struct wildcardMutator));
    char* wildcard = readWildcard(ENV_WILDCARD_FILE_PATH);
    char* rawDebugFlag = getenv(ENV_WILDCARD_DEBUG_MODE);
    bool debugFlag;

    assert(mutator != (struct wildcardMutator*)NULL);
    assert(wildcard != (char*)NULL);

    srand(seed);    // Random initialization

    if (rawDebugFlag == NULL)
    {
        debugFlag = true;
    }
    else
    {
        debugFlag = false;
    }

    mutator->afl = afl;
    mutator->intRep = parseWildcard(wildcard);
    mutator->mutatedOutBufferSize = MAX_STRING_SIZE;
    mutator->mutatedOutBuffer = (char*)calloc(mutator->mutatedOutBufferSize, sizeof(char));
    mutator->debug = debugFlag;

    assert(mutator->mutatedOutBuffer != (char*)NULL);

    free(wildcard);

    OKF("WILDCARD Mutator ready !");

    return mutator;
}

size_t afl_custom_fuzz(struct wildcardMutator* data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size)
{
    size_t mutatedInputLength;

    // Mutate function call to add here
    strReset(data->mutatedOutBuffer, data->mutatedOutBufferSize);
    generateMutatedInput(data->intRep, (char*)buf, data->mutatedOutBuffer, max_size);
    mutatedInputLength = strlen(data->mutatedOutBuffer);

    *out_buf = (unsigned char*)(data->mutatedOutBuffer);

    if (data->debug == true)
    {
        DEBUGF("%s", *out_buf);
    }

    return mutatedInputLength;
}

void afl_custom_deinit(struct wildcardMutator* data)
{
    free(data->intRep);
    free(data->mutatedOutBuffer);
    free(data);
}

#pragma endregion