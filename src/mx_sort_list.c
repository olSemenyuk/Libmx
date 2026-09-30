#include "libmx.h"

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *)) {
    if (lst == NULL || cmp == NULL) return lst;

    int count = mx_list_size(lst);

    for (int i = 0; i < count - 1; i++) {
        t_list *a = lst;

        for (t_list *b = lst->next; b != NULL; a = b, b = b->next) {
            if (cmp(a->data, b->data)) {
                void *tmp = a->data;

                a->data = b->data;
                b->data = tmp;
            }
        }
    }

    return lst;
}
