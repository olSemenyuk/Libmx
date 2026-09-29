#include "libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    char* tmp_mem = (char*)b;

    if (b) {
    while (len--) 
        *tmp_mem++ = c;
    }
    return b;
}
