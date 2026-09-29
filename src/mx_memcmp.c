#include "libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    char *cs1 = (char *)s1;
    char *cs2 = (char *)s2;
    size_t i;   

    for (i = 0; i < n; i++, cs1++, cs2++) {
        if (*cs1 < *cs2)
            return *cs1 - *cs2;
        else if (*cs1 > *cs2)
            return *cs1 - *cs2;
    }
    return 0;
}
