#ifndef WILDCARD_MUTATOR_H
#define WILDCARD_MUTATOR_H

#include "afl-fuzz.h"
#include "string_list.h"
#include "config.h"
#include "utils.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DEFINE_TUPLE_PTR(type)  struct tuple_ptr_##type \
                                {                       \
                                    type* a;            \
                                    type* b;            \
                                }

struct wildcardMutatorIntRep
{
    struct stringListNode* baseInput;
};

struct wildcardMutator
{
    afl_state_t* afl;

    struct wildcardMutatorIntRep* intRep;
};

#ifdef DEBUG
    // DEBUG
    void stringList_printStringList(struct stringListNode* stringList);
#endif

#pragma region AFL++ functions
/**
 * Initialize this custom mutator
 *
 * @param[in] afl a pointer to the internal state object. Can be ignored for
 * now.
 * @param[in] seed A seed for this mutator - the same seed should always mutate
 * in the same way.
 * @return Pointer to the data object this custom mutator instance should use.
 *         There may be multiple instances of this mutator in one afl-fuzz run!
 *         Return NULL on error.
 */
struct wildcardMutator* afl_custom_init(afl_state_t *afl, unsigned int seed);

/*unsigned int afl_custom_fuzz_count(void *data, const unsigned char *buf, size_t buf_size);
void afl_custom_splice_optout(void *data);*/

/**
 * Perform custom mutations on a given input
 *
 * (Optional for now. Required in the future)
 *
 * @param[in] data pointer returned in afl_custom_init for this fuzz case
 * @param[in] buf Pointer to input data to be mutated
 * @param[in] buf_size Size of input data
 * @param[out] out_buf the buffer we will work on. we can reuse *buf. NULL on
 * error.
 * @param[in] add_buf Buffer containing the additional test case
 * @param[in] add_buf_size Size of the additional test case
 * @param[in] max_size Maximum size of the mutated output. The mutation must not
 *     produce data larger than max_size.
 * @return Size of the mutated output.
 */
size_t afl_custom_fuzz(struct wildcardMutator* data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size);

/*const char *afl_custom_describe(void *data, size_t max_description_len);
size_t afl_custom_post_process(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf);
int afl_custom_init_trim(void *data, unsigned char *buf, size_t buf_size);
size_t afl_custom_trim(void *data, unsigned char **out_buf);
int afl_custom_post_trim(void *data, unsigned char success);
size_t afl_custom_havoc_mutation(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, size_t max_size);
unsigned char afl_custom_havoc_mutation_probability(void *data);
unsigned char afl_custom_queue_get(void *data, const unsigned char *filename);
void (*afl_custom_fuzz_send)(void *data, const u8 *buf, size_t buf_size);
u8 afl_custom_queue_new_entry(void *data, const unsigned char *filename_new_queue, const unsigned int *filename_orig_queue);
const char* afl_custom_introspection(my_mutator_t *data);*/

/**
 * Deinitialize everything
 *
 * @param data The data ptr from afl_custom_init
 */
void afl_custom_deinit(struct wildcardMutator* data);

#pragma endregion

#endif