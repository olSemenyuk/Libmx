#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    void *real;

    real = malloc(size);
    mx_memset(real, 0, size);
    if (real)
        mx_memcpy(real, ptr, size);
    free(ptr);
    return (real);
}
