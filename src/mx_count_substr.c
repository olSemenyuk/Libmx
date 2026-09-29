#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    int count = 0;

    if (str == NULL || sub == NULL) 
        return -1;
    if (mx_strlen(sub) <= 0)
        return 0;
    while(*str) {
        if (mx_memcmp(str, sub, mx_strlen(sub)) == 0) {
        count++;
        }
        str++;
    }
    return count;
}
