#include "libmx.h"

char *mx_strncpy(char *dst, const char *src, int len) {
    char *ret = (char*)malloc(sizeof(char) * len);
    
    if (mx_strlen(src) >= len) {
        for (int q = 0; q < len; q++) {
		    ret[q] = src[q];
	    }
    }
    else {
        for (int q = 0; q < mx_strlen(src); q++)
		    ret[q] = src[q];
        for (int w = mx_strlen(src); w < len; w++)
            ret[w] = '\0';
    }
    dst = ret;
    return dst;
    free(dst);
}
