#include "libmx.h"

char *mx_del_extra_spaces(const char *str) {
    char *tmp;
    int size = mx_strlen(str);
    char *ret = (char*) malloc(size * sizeof(char) + 1);
    int j = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (mx_isspace(str[i])) {
          ret[j++] = str[i];
          while (mx_isspace(str[i++]));
          i--;
          ret[j++] = str[i];
          continue;
        }
        ret[j++] = str[i];
    }
    ret[size] = '\0';
    tmp = mx_strtrim(ret);
    return tmp;
}
