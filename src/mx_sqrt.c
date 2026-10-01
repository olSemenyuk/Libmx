#include "libmx.h"

int mx_sqrt(int x) {
    if (x <= 0)
        return 0;
    for (long i = 1; i * i <= x; i++) {
        if (i * i == x)
            return (int)i;
    }
    return 0;
}
