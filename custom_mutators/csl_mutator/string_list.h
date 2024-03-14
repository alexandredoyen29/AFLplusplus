#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <assert.h>
#include <stdlib.h>

struct stringListNode
{
    char* nodeContent;
    struct stringListNode* nextNode;
};

struct stringListNode* stringList_init();
void stringList_add(struct stringListNode** stringList, char* contentToAdd);
void stringList_free(struct stringListNode** stringList);
void stringList_iter(struct stringListNode** stringList, void (*action)(char* str));
void stringList_iteri(struct stringListNode** stringList, void (*action)(char* str, int i));

#endif