#include "utils.h"

size_t min(size_t a, size_t b)
{
    size_t result;

    if (a < b)
    {
        result = a;
    }
    else
    {
        result = b;
    }

    return result;
}

void strConcat(char* dst, char* src, size_t len)
{
    size_t srcLen = strnlen(src, len);
    size_t dstLen = strnlen(dst, len);
    int i = 0;

    while ((i < srcLen) && ((dstLen + i) < (len - 1)))
    {
        dst[dstLen + i] = src[i];
        i++;
    }
}