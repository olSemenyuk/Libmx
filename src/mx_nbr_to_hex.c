#include "libmx.h"

static char *h_to_d (unsigned long nbr) {
    char hexadecimal[256];
    int j = 0;
    unsigned long remainder = 0;
    char *p = hexadecimal;

    while (nbr != 0) {
        remainder = nbr % 16;          
        if (remainder < 10)
            hexadecimal[j++] = 48 + remainder;   
        else
            hexadecimal[j++] = 87 + remainder;  
        nbr = nbr / 16;            
    }
    return p;
}

char *mx_nbr_to_hex(unsigned long nbr) {
    char *p = NULL;
    char *p1 = NULL;
    int j = mx_strlen(h_to_d(nbr));  
    p1 = h_to_d(nbr);

    p = mx_strnew(j);
        for(int i = 0, k = j - 1; i < j; i++,k--)
            p[k] = p1[i];
    return p;
}
