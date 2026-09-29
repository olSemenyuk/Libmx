#include "libmx.h"

int mx_list_size(t_list *list) {
    int count = 0;

    for (t_list *walker = list; walker != NULL; walker = walker->next) {
        count++;
    }
    return count;
}
