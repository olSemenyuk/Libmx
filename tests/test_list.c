#include "libmx.h"
#include "test.h"
#include <stdbool.h>

static bool cmp(void *a, void *b) {
    return strcmp((char *)a, (char *)b) > 0;
}

static t_list *build(const char **a, int n) {
    t_list *l = NULL;

    for (int i = 0; i < n; i++)
        mx_push_back(&l, (char *)a[i]);
    return l;
}

static void expect(t_list *l, const char **exp, int n) {
    CHECK_INT(mx_list_size(l), n);
    for (int i = 0; i < n; i++) {
        REQUIRE(l != NULL);
        CHECK_STR((char *)l->data, exp[i]);
        l = l->next;
    }
    CHECK_PTR(l, NULL);
}

static void list_free(t_list *l) {
    while (l != NULL) {
        t_list *next = l->next;

        free(l);
        l = next;
    }
}

static void sort_case(const char **in, const char **out, int n) {
    t_list *l = mx_sort_list(build(in, n), cmp);

    expect(l, out, n);
    list_free(l);
}

static void test_create_node(void) {
    t_list *node = mx_create_node("node");

    REQUIRE(node != NULL);
    CHECK_STR((char *)node->data, "node");
    CHECK_PTR(node->next, NULL);
    free(node);

    node = mx_create_node(NULL);
    REQUIRE(node != NULL);
    CHECK_PTR(node->data, NULL);
    CHECK_PTR(node->next, NULL);
    free(node);
}

static void test_list_size(void) {
    CHECK_INT(mx_list_size(NULL), 0);
}

static void test_push_pop(void) {
    t_list *list = NULL;
    const char *e3[] = {"hello", "world", "!"};
    const char *e2[] = {"world", "!"};
    const char *e1[] = {"world"};

    mx_push_front(&list, "world");
    mx_push_front(&list, "hello");
    mx_push_back(&list, "!");
    expect(list, e3, 3);

    mx_pop_front(&list);
    expect(list, e2, 2);

    mx_pop_back(&list);
    expect(list, e1, 1);

    mx_pop_back(&list);
    CHECK_PTR(list, NULL);

    mx_push_back(&list, "a");
    REQUIRE(list != NULL);
    CHECK_PTR(list->next, NULL);
    mx_pop_front(&list);
    CHECK_PTR(list, NULL);

    mx_push_front(&list, "b");
    REQUIRE(list != NULL);
    CHECK_PTR(list->next, NULL);
    mx_pop_front(&list);
    CHECK_PTR(list, NULL);

    mx_pop_front(&list);
    mx_pop_back(&list);
    CHECK_PTR(list, NULL);
}

static void test_pop_back_long(void) {
    const char *in[] = {"a", "b", "c", "d"};
    const char *e3[] = {"a", "b", "c"};
    const char *e2[] = {"a", "b"};
    t_list *l = build(in, 4);

    mx_pop_back(&l);
    expect(l, e3, 3);
    mx_pop_back(&l);
    expect(l, e2, 2);
    list_free(l);
}

static void test_sort_list(void) {
    const char *rev_in[]   = {"pear", "apple", "grape"};
    const char *rev_out[]  = {"apple", "grape", "pear"};
    const char *sorted[]   = {"a", "b", "c"};
    const char *worst_in[] = {"d", "c", "b", "a"};
    const char *worst_out[] = {"a", "b", "c", "d"};
    const char *dup_in[]   = {"b", "a", "b", "a"};
    const char *dup_out[]  = {"a", "a", "b", "b"};
    const char *single[]   = {"x"};

    sort_case(rev_in, rev_out, 3);
    sort_case(sorted, sorted, 3);
    sort_case(worst_in, worst_out, 4);
    sort_case(dup_in, dup_out, 4);
    sort_case(single, single, 1);
    CHECK_PTR(mx_sort_list(NULL, cmp), NULL);
}

void run_list_tests(void) {
    test_create_node();
    test_list_size();
    test_push_pop();
    test_pop_back_long();
    test_sort_list();
}
