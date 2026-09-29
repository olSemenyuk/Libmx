#include "libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    const char *s;
    const char *lasts;
    char *d;
    char *lastd;

    if(len < 0)
        return NULL;
    d = dst;
    s = src;
    if (d < s)
        while (len--)
            *d++ = *s++;
    else {
        lasts = s + (len - 1);
        lastd = d + (len - 1);
        while (len--)
            *lastd-- = *lasts--;
    }
    return (dst);
}
