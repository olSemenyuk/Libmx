#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    unsigned long decimal = 0;
    int i = 0;
    int val;
    int len = mx_strlen(hex) - 1;

    for (i = 0; hex[i] != '\0'; i++) {
        if (hex[i] >= '0' && hex[i] <= '9')
            val = hex[i] - 48;
        if (mx_islower(hex[i]))
            val = hex[i] - 97 + 10;
        if (mx_isupper(hex[i]))
            val = hex[i] - 65 + 10;
        decimal += val * mx_pow(16, len);
        len--;
    }
    return decimal;
}
