#include "libmx.h"

void mx_print_unicode(wchar_t c) {
    char temp[5];

    if (c <= 0x0000007F) {
        temp[0] = (char) c;
    }
    else if (c <= 0x000007FF ) {
        temp[0] = (char) (((c >> 6) & 0x1F) | 0XC0);
        temp[1] = (char) (((c >> 0) & 0x3F) | 0X80);
    }
    else if (c <= 0x0000FFFF) {
        temp[0] = (char) (((c >> 12) & 0x0F) | 0xE0);
        temp[1] = (char) (((c >> 6) & 0x3F) | 0X80);
        temp[2] = (char) (((c >> 0) & 0x3F) | 0X80);
    } 
    else if (c <= 0x001FFFFF) {
        temp[0] = (char) (((c >> 18) & 0x07) | 0xF0);
        temp[1] = (char) (((c >> 12) & 0x3F) | 0x80);
        temp[2] = (char) (((c >> 6) & 0x3F) | 0x80);
        temp[3] = (char) (((c >> 0) & 0x3F) | 0x80);
    } 
    write(1, &temp, mx_strlen(temp));
}
