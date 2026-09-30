#include "libmx.h"

char *mx_strdup(const char *str) {
    char *dst;
    int size;

    size = mx_strlen(str);
    dst = mx_strnew(size);

    if (dst == 0) return 0;

    mx_strcpy(dst, str);
    return dst;
}
