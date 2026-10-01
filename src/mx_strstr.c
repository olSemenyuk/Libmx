#include "libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    const char *temp;
    const char *c;

    temp = needle;
    while (*haystack) {
        c = haystack;
        while (*(haystack++) == *(needle++)) {
            if (!(*needle))
                return (char*)c;
            if (!(*haystack))
                return (char*)0;
        }
        needle = temp;
    }
    return (char*)0;
}
