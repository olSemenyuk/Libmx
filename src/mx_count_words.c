#include "libmx.h"

int mx_count_words(const char *str, char c) {
    int words = 0;

    if(str == NULL)
        return -1;
    if (str[0] != '\0' && str[0] != c)
        words++;
    for (int i = 0; str[i] != '\0'; i++) {
        if(str[i] == c &&
            str[i + 1] != c &&
            str[i + 1] != '\0')
        {
            words++;
        }
    }
    return words;
}
