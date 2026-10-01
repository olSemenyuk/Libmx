#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    if (str == NULL || sub == NULL)
        return -1;

    int sub_len = mx_strlen(sub);
    int count = 0;

    if (sub_len == 0)
        return 0;
    while (*str != '\0') {
        if (mx_strncmp(str, sub, sub_len) == 0) {
            count++;
            str += sub_len;
        } else {
            str++;
        }
    }
    return count;
}
