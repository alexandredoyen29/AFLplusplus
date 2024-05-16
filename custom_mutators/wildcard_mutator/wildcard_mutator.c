#include "wildcard_mutator.h"

#pragma region Mutator internals

static char* readWildcard(char* wildcardFileName)
{
    char* wildcardFilePath = getenv(wildcardFileName);
    char* result = malloc(WILDCARD_FILE_READ_BUFFER_SIZE * sizeof(char));
    int fdWildcardFilePath;

    assert(result != (char*)NULL);

    if (wildcardFilePath == (char*)NULL)
    {
        perror("[readWildcard()] -> WILDCARD file not found");
    }
    else
    {
        fdWildcardFilePath = open(wildcardFilePath, O_RDONLY);

        read(fdWildcardFilePath, result, WILDCARD_FILE_READ_BUFFER_SIZE * sizeof(char));

        close(fdWildcardFilePath);
    }

    return result;
}

static struct stringListNode* readWildcardsFilenames(char* wildcardsDirEnvVar)
{
    struct dirent* dirElement;
    struct stringListNode* result = stringList_init();
    char* str;
    DIR* dirCursor;
    char* wildcardsDir = getenv(wildcardsDirEnvVar);

    dirCursor = opendir(wildcardsDir);

    if (dirCursor != (DIR*)NULL)
    {
        do
        {
            dirElement = readdir(dirCursor);

            str = (char*)malloc(MAX_STRING_SIZE * sizeof(char*));

            strncpy(str, dirElement->d_name, MAX_STRING_SIZE);
            stringList_add(&result, str);
        } while (dirCursor != (DIR*)NULL);
    }
    else
    {
        perror("[readWildcard()] -> Error when reading wildcard dir");
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

static void freeIntRep(struct wildcardMutatorIntRep* intRep)
{
    free(intRep);
}

static void addWildcardMutatorIntRepToIntRepsFromFilename(char* wildcard, void* intRepList)
{
    struct wildcardMutatorIntRepListNode** intRepListConverted = (struct wildcardMutatorIntRepListNode**)intRepList;

    wildcardMutatorIntRepList_add(intRepListConverted, parseWildcard(wildcard));
}

static void freeString(char* string)
{
    free(string);
}

static void readWildcardToStringList(char* filename, void* wildcardList)
{
    struct stringListNode** wildcardListConverted = (struct stringListNode**)wildcardList;

    stringList_add(wildcardListConverted, readWildcard(filename));
}

static struct wildcardMutatorIntRep* getRandomWildcard(struct wildcardMutatorIntRepListNode* list)
{
    int randomWildcardIndex = (rand() / RAND_MAX) * wildcardMutatorIntRepList_length(list);

    return wildcardMutatorIntRepList_get(list, randomWildcardIndex);
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
#endif

#pragma region AFL++ functions

struct wildcardMutator* afl_custom_init(afl_state_t *afl, unsigned int seed)
{
    struct wildcardMutator* mutator = (struct wildcardMutator*)malloc(sizeof(struct wildcardMutator));
    struct stringListNode* wildcardsFilenames = readWildcardsFilenames(ENV_WILDCARD_DIR_PATH);
    struct stringListNode* wildcards = stringList_init();
    char* rawDebugFlag = getenv(ENV_WILDCARD_DEBUG_MODE);
    bool debugFlag;

    assert(mutator != (struct wildcardMutator*)NULL);
    assert(wildcardsFilenames != (struct stringListNode*)NULL);

    srand(seed);    // Random initialization

    if (rawDebugFlag == NULL)
    {
        debugFlag = false;
    }
    else
    {
        debugFlag = true;
    }

    mutator->afl = afl;
    mutator->intRepList = wildcardMutatorIntRepList_init();
    mutator->mutatedOutBufferSize = MAX_STRING_SIZE;
    mutator->mutatedOutBuffer = (char*)calloc(mutator->mutatedOutBufferSize, sizeof(char));
    mutator->debug = debugFlag;

    stringList_iterd(wildcardsFilenames, readWildcardToStringList, &wildcards);
    stringList_iterd(wildcards, addWildcardMutatorIntRepToIntRepsFromFilename, &mutator->intRepList);

    assert(mutator->mutatedOutBuffer != (char*)NULL);

    stringList_iter(wildcardsFilenames, freeString);
    stringList_iter(wildcards, freeString);

    stringList_free(&wildcardsFilenames);
    stringList_free(&wildcards);

    OKF("WILDCARD Mutator ready !");

    return mutator;
}

size_t afl_custom_fuzz(struct wildcardMutator* data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size)
{
    size_t mutatedInputLength;
    struct wildcardMutatorIntRep* intRep = getRandomWildcard(data->intRepList);

    // Mutate function call to add here
    strReset(data->mutatedOutBuffer, data->mutatedOutBufferSize);
    generateMutatedInput(intRep, (char*)buf, data->mutatedOutBuffer, max_size);
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
    wildcardMutatorIntRepList_iter(data->intRepList, freeIntRep);
    wildcardMutatorIntRepList_free(&(data->intRepList));
    free(data->mutatedOutBuffer);
    free(data);
}

#pragma endregion