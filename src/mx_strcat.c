#include "libmx.h"

char *mx_strcat(char *restrict s1, const char *restrict s2) {
    if (s1 == NULL) return NULL;
    if (s2 == NULL) return s1;

    int len = mx_strlen(s1);
    int i = 0;

    for (; s2[i] != '\0'; i++)
        s1[len + i] = s2[i];
    s1[len + i] = '\0';

    return s1;
}
