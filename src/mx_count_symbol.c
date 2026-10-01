#include "libmx.h"

int mx_count_symbol(const char *s, char c) {
    int i = 0;
    int count = 0;

    while (s[i]) {
        if (s[i] == c) 
            count++;
        i++;
    }
    return count;
}
