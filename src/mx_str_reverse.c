#include "../inc/libmx.h"

void mx_str_reverse(char *s) {
    for(int len = mx_strlen(s) - 1; len >= 0; len--)
        mx_printchar(s[len]);
}
