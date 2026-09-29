#include "libmx.h"

char *mx_del_extra_spaces(const char *str) {
    if (str == NULL)
        return NULL;

    char *res = mx_strnew(mx_strlen(str));
    if (res == NULL)
        return NULL;

    int j = 0;
    int pending_space = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (mx_isspace(str[i])) {
            pending_space = (j > 0);
        } else {
            if (pending_space)
                res[j++] = ' ';
            res[j++] = str[i];
            pending_space = 0;
        }
    }
    return res;
}
