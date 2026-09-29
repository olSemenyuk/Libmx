#include "libmx.h"

char *mx_strndup(const char *s1, size_t n) {
    char *dst = mx_strnew(n);

    if (dst == 0)
        return 0;
    if (sizeof(char) * mx_strlen(s1) >= n)
        for (size_t q = sizeof(char) * 0; q < n; q++) 
		    dst[q] = s1[q];
    else {
        for (int q = sizeof(char) * 0; q < mx_strlen(s1); q++)
		    dst[q] = s1[q];
        for (size_t w = sizeof(char) * mx_strlen(s1); w < n; w++)
            dst[w] = '\0';
    }
    return dst;
    free(dst);
}

