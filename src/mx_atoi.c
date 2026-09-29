#include "libmx.h"

int mx_atoi(const char *str) {
    int sign = 1;
    long long result = 0;

    while (mx_isspace(*str))
        str++;
    if (*str == '-' || *str == '+') {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (mx_isdigit(*str)) {
        result = result * 10 + (*str - '0');
        if (sign > 0 && result > INT_MAX)
            return INT_MAX;
        if (sign < 0 && result > (long long)INT_MAX + 1)
            return INT_MIN;
        str++;
    }
    return (int)(result * sign);
}
