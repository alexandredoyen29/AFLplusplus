//#include "afl-fuzz.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef DEBUG
    #include <stdio.h>
#endif

struct stringListNode
{
    char* nodeContent;
    struct stringListNode* nextNode;
};

struct cslMutatorIntRep
{
    struct stringListNode* baseInput;
};

// String lists
struct stringListNode* stringList_init();
void stringList_addString(struct stringListNode** stringList, char* contentToAdd);
void stringList_free(struct stringListNode** stringList);

// Fonctions internes pour le mutateur
struct cslMutatorIntRep* parseCsl(char* cslContent);

#ifdef DEBUG
    // DEBUG
    void stringList_printStringList(struct stringListNode** stringList);
#endif

// Fonctions pour AFL++
/*void *afl_custom_init(afl_state_t *afl, unsigned int seed);
unsigned int afl_custom_fuzz_count(void *data, const unsigned char *buf, size_t buf_size);
void afl_custom_splice_optout(void *data);
size_t afl_custom_fuzz(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size);
const char *afl_custom_describe(void *data, size_t max_description_len);
size_t afl_custom_post_process(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf);
int afl_custom_init_trim(void *data, unsigned char *buf, size_t buf_size);
size_t afl_custom_trim(void *data, unsigned char **out_buf);
int afl_custom_post_trim(void *data, unsigned char success);
size_t afl_custom_havoc_mutation(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, size_t max_size);
unsigned char afl_custom_havoc_mutation_probability(void *data);
unsigned char afl_custom_queue_get(void *data, const unsigned char *filename);
void (*afl_custom_fuzz_send)(void *data, const u8 *buf, size_t buf_size);
u8 afl_custom_queue_new_entry(void *data, const unsigned char *filename_new_queue, const unsigned int *filename_orig_queue);
const char* afl_custom_introspection(my_mutator_t *data);
void afl_custom_deinit(void *data);*/