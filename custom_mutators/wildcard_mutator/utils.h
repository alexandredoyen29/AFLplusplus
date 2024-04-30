#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <string.h>

size_t min(size_t a, size_t b);
void strConcat(char* dst, char* src, size_t len);
void strReset(char* str, size_t len);

#endif