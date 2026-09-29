#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    char *csrc = (char *)src;
    char *cdst = (char *)dst;
    size_t i = 0;

    if (n < 0 && c < 0) {
        return NULL;
    }
    while (csrc[i] != c) {
        cdst[i] = csrc[i];
        i++;
        if (i >= n)
            return 0;
    }
    cdst[i] = c;
    return &cdst[i];
}
