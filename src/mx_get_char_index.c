#include "libmx.h"

int mx_get_char_index(const char *str, char c) {
    int i = 0;

    if ( str == NULL) 
        return -2;
    while (*str != '\0') {
        if (*str == c)
            return i;
    str++;
    i++;
    }
    return -1;
}
