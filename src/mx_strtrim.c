#include "libmx.h"

char *mx_strtrim(const char *str) {
    if (str == NULL)
        return NULL;

    int start = 0;

    while (str[start] != '\0' && mx_isspace(str[start]))
        start++;

    int end = mx_strlen(str);

    while (end > start && mx_isspace(str[end - 1]))
        end--;

    return mx_strndup(str + start, end - start);
}
