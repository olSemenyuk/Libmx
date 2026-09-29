#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    unsigned long res = 0;

    if (hex == NULL)
        return 0;
    for (int i = 0; hex[i] != '\0'; i++) {
        int val;

        if (hex[i] >= '0' && hex[i] <= '9')
            val = hex[i] - '0';
        else if (hex[i] >= 'a' && hex[i] <= 'f')
            val = hex[i] - 'a' + 10;
        else if (hex[i] >= 'A' && hex[i] <= 'F')
            val = hex[i] - 'A' + 10;
        else
            return 0;
        res = res * 16 + (unsigned long)val;
    }
    return res;
}