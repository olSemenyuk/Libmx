#include "libmx.h"

void mx_printerr(const char *c) {
    write(2, c, mx_strlen(c));
}
