#include "libmx.h"

int mx_strncmp(const char *s1, const char *s2, int n) {
    const unsigned char *a = (const unsigned char *)s1;
    const unsigned char *b = (const unsigned char *)s2;

    for (int i = 0; i < n; i++) {
        if (a[i] != b[i] || a[i] == '\0')
            return a[i] - b[i];
    }
    
    return 0;
}
