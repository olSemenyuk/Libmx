#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
   int first = 0;
   int last = size - 1;
   int middle = (first + last) / 2;
    
    while (first <= last) {
        ++*count;
        middle = (first + last) / 2;
        if (mx_strcmp(arr[middle], s) < 0) 
            first = middle + 1; 
        else if (mx_strcmp(arr[middle], s) == 0) 
            return middle;  
        else 
            last = middle - 1;
        if (first > last) {
            *count = 0;
            return -1;
        }
    }
    *count = 0;
    return -1;
}
