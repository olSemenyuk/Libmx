#include "libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    size_t i;
    char* cdst = (char*)dst;
    const char* csrc = (char*)src;

    for (i = 0; i < n; ++i)
        *cdst++ = *csrc++;
    
    return cdst;
}
