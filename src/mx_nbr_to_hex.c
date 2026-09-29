#include "libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    const char digits[] = "0123456789abcdef";
    int len = 1;

    for (unsigned long t = nbr; t >= 16; t /= 16)
        len++;

    char *res = mx_strnew(len);
    if (res == NULL)
        return NULL;

    for (int i = len - 1; i >= 0; i--) {
        res[i] = digits[nbr % 16];
        nbr /= 16;
    }
    return res;
}