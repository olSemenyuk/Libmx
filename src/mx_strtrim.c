#include "libmx.h"

char *mx_strtrim(const char *str) {
    char *trim = NULL;
    int countdo = 0;
    int countposle = 0;
    int j = 0;

    if (mx_strlen(str) > 0) {
        while (str[countdo] != '\0' 
               && mx_isspace(str[countdo]) == 1) 
            countdo++;
        for (int i = mx_strlen(str) - 1; i >= 0 
             && mx_isspace(str[i]) == 1; i--)
            countposle++;
        trim = mx_strnew(mx_strlen(str) - countdo - countposle);
        for (int i = countdo; i < (mx_strlen(str) - countposle); i++) {
            trim[j] = str[i]; 
            j++;
        }
    }
    return trim;
}
