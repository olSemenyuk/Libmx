#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    const char *temp;
    const char *c;
    int i = 0;

    if (str == NULL || sub == NULL) 
        return -2;
        
    temp = sub;
    while (*str) {
        c = str;
        while (*(str++) == *(sub++)) {
            if (!(*sub))
                return i;
        }
        str = c;
        sub = temp;
        i++;
    }
    return -1;
}
