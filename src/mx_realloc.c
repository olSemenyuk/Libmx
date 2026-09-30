#if !defined(__APPLE__)
#define _DEFAULT_SOURCE
#endif

#include "libmx.h"

#if defined(__APPLE__)
    #include <malloc/malloc.h>
    #define MX_MSIZE(p) malloc_size(p)
#else
    #include <malloc.h>
    #define MX_MSIZE(p) malloc_usable_size(p)
#endif

void *mx_realloc(void *ptr, size_t size) {
    if (ptr == NULL)
        return malloc(size);
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    size_t old = MX_MSIZE(ptr);
    void *new_ptr = malloc(size);

    if (new_ptr == NULL)
        return NULL;
    mx_memcpy(new_ptr, ptr, old < size ? old : size);
    free(ptr);
    return new_ptr;
}