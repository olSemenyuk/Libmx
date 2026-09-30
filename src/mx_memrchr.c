#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    const unsigned char *p = s;
    unsigned char uc = (unsigned char)c;

    while (n > 0) {
        n--;
        if (p[n] == uc)
            return (void *)(p + n);
    }
    return NULL;
}