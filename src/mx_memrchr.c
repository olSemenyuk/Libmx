#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    const unsigned char *nachalo = s;
    const unsigned char *konec = (const unsigned char *)s + n;
    void *neobh = NULL;

    while (konec > nachalo) {
        if (*nachalo == (unsigned char)c)
        neobh = (void*)nachalo;
        nachalo++;
    }
    return neobh;
}
