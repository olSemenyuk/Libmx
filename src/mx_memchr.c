#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    size_t i;

    for (i = 0; i < n; i++)
        if (*((char *)s + i) == c) return (char *)s + i;
    return NULL;
}
