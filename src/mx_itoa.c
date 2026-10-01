#include "libmx.h"

char *mx_itoa(int number) {
    int len = 1;
    int i = 0;
    char *new_str = NULL;
    long int buf = number;

    for ( ;number / 10; len++)
        number /= 10;
    buf > 0 ? (new_str = mx_strnew(len)) : (new_str = mx_strnew(len + 1));
    buf < 0 ? (i = 1, new_str[0] = '-', buf *= -1) : len--;
    
    for ( ; len >= 0 + i; buf /= 10)
        new_str[len--] = buf % 10 + 48;
    return new_str;
}
