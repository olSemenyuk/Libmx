#include "libmx.h"

static int count_matches(const char *str, const char *sub, int sub_len) {
    int count = 0;

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

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (str == NULL || sub == NULL || replace == NULL || *sub == '\0')
        return NULL;

    int sub_len = mx_strlen(sub);
    int rep_len = mx_strlen(replace);
    int count = count_matches(str, sub, sub_len);
    char *res = mx_strnew(mx_strlen(str) + count * (rep_len - sub_len));
    char *out = res;

    if (res == NULL)
        return NULL;
    while (*str != '\0') {
        if (mx_strncmp(str, sub, sub_len) == 0) {
            for (int k = 0; k < rep_len; k++)
                *out++ = replace[k];
            str += sub_len;
        } else {
            *out++ = *str++;
        }
    }
    return res;
}
