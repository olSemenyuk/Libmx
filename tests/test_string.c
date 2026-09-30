#include "libmx.h"
#include "test.h"

static void fill(char *b, size_t n) {
    memset(b, 'x', n);
}

static int arr_len(char **a) {
    int n = 0;

    while (a[n] != NULL)
        n++;
    return n;
}

static void test_strlen(void) {
    CHECK_INT(mx_strlen(""), 0);
    CHECK_INT(mx_strlen("hello"), 5);
    CHECK_INT(mx_strlen("a b\tc"), 5);
}

static void test_strcpy(void) {
    char dst[16];

    memset(dst, 'x', sizeof dst);
    CHECK_PTR(mx_strcpy(dst, "abc"), dst);
    CHECK_MEM(dst, "abc\0xxxx", 8);

    mx_strcpy(dst, "");
    CHECK_STR(dst, "");
}

static void test_strncpy(void) {
    char b[8];

    fill(b, sizeof b);
    CHECK_PTR(mx_strncpy(b, "abcdef", 3), b);
    CHECK_MEM(b, "abcxxxxx", 8);

    fill(b, sizeof b);
    mx_strncpy(b, "ab", 5);
    CHECK_MEM(b, "ab\0\0\0xxx", 8);

    fill(b, sizeof b);
    mx_strncpy(b, "abc", 0);
    CHECK_MEM(b, "xxxxxxxx", 8);

    fill(b, sizeof b);
    mx_strncpy(b, "abc", 3);
    CHECK_MEM(b, "abcxxxxx", 8);
}

static void test_strdup(void) {
    const char src[] = "libmx";
    char *d = mx_strdup(src);

    REQUIRE(d != NULL);
    CHECK(d != src);
    CHECK_STR(d, "libmx");
    free(d);

    CHECK_OWN(mx_strdup(""), "");
}

static void test_strndup(void) {
    CHECK_OWN(mx_strndup("abcdef", 3), "abc");
    CHECK_OWN(mx_strndup("abc", 10), "abc");
    CHECK_OWN(mx_strndup("abc", 3), "abc");
    CHECK_OWN(mx_strndup("abc", 0), "");
}

static void test_strnew(void) {
    char *s = mx_strnew(5);

    REQUIRE(s != NULL);
    CHECK_MEM(s, "\0\0\0\0\0\0", 6);
    free(s);

    CHECK_OWN(mx_strnew(0), "");
    CHECK_PTR(mx_strnew(-1), NULL);
}

static void test_strcat(void) {
    char buf[16] = "hello";

    CHECK_PTR(mx_strcat(buf, "!"), buf);
    CHECK_STR(buf, "hello!");
    mx_strcat(buf, "");
    CHECK_STR(buf, "hello!");
    mx_strcat(buf, " w");
    CHECK_STR(buf, "hello! w");
}

static void test_strcmp(void) {
    CHECK_INT(mx_strcmp("a", "a"), 0);
    CHECK_INT(mx_strcmp("", ""), 0);
    CHECK(mx_strcmp("a", "b") < 0);
    CHECK(mx_strcmp("b", "a") > 0);
    CHECK(mx_strcmp("", "a") < 0);
    CHECK(mx_strcmp("a", "") > 0);
    CHECK(mx_strcmp("\xD0\xB0", "a") > 0);
}

static void test_strncmp(void) {
    CHECK_INT(mx_strncmp("abc", "abd", 2), 0);
    CHECK(mx_strncmp("abc", "abd", 3) < 0);
    CHECK_INT(mx_strncmp("abc", "xyz", 0), 0);
    CHECK_INT(mx_strncmp("abc", "abc", 10), 0);
    CHECK(mx_strncmp("\xff", "a", 1) > 0);
}

static void test_strchr_strstr(void) {
    const char s[] = "banana";

    CHECK_PTR(mx_strchr(s, 'n'), s + 2);
    CHECK_PTR(mx_strchr(s, 'b'), s);
    CHECK_PTR(mx_strchr(s, 'z'), NULL);
    CHECK_PTR(mx_strstr(s, "ana"), s + 1);
    CHECK_PTR(mx_strstr(s, "ban"), s);
    CHECK_PTR(mx_strstr(s, "nana"), s + 2);
    CHECK_PTR(mx_strstr(s, "xyz"), NULL);
    CHECK_PTR(mx_strstr(s, "bananas"), NULL);
}

static void test_indexes(void) {
    CHECK_INT(mx_get_char_index("banana", 'a'), 1);
    CHECK_INT(mx_get_char_index("banana", 'b'), 0);
    CHECK_INT(mx_get_char_index("banana", 'z'), -1);
    CHECK_INT(mx_get_char_index(NULL, 'a'), -2);

    CHECK_INT(mx_get_substr_index("banana", "na"), 2);
    CHECK_INT(mx_get_substr_index("banana", "ban"), 0);
    CHECK_INT(mx_get_substr_index("banana", "xyz"), -1);
    CHECK_INT(mx_get_substr_index(NULL, "a"), -2);
    CHECK_INT(mx_get_substr_index("banana", NULL), -2);
}

static void test_counts(void) {
    CHECK_INT(mx_count_substr("banana", "ana"), 1);
    CHECK_INT(mx_count_substr("aaaa", "aa"), 2);
    CHECK_INT(mx_count_substr("aaa", "aa"), 1);
    CHECK_INT(mx_count_substr("banana", ""), 0);

    CHECK_INT(mx_count_words("one two three", ' '), 3);
    CHECK_INT(mx_count_words("  one   two ", ' '), 2);
    CHECK_INT(mx_count_words("", ' '), 0);
    CHECK_INT(mx_count_words("one", ' '), 1);
    CHECK_INT(mx_count_words(NULL, ' '), -1);
}

static void test_count_symbol(void) {
    CHECK_INT(mx_count_symbol("banana", 'a'), 3);
    CHECK_INT(mx_count_symbol("banana", 'b'), 1);
    CHECK_INT(mx_count_symbol("banana", 'z'), 0);
    CHECK_INT(mx_count_symbol("", 'a'), 0);
}

static void test_strjoin(void) {
    CHECK_OWN(mx_strjoin("Hello", " World"), "Hello World");
    CHECK_OWN(mx_strjoin("", ""), "");
    CHECK_OWN(mx_strjoin("a", NULL), "a");
    CHECK_OWN(mx_strjoin(NULL, "b"), "b");
    CHECK_PTR(mx_strjoin(NULL, NULL), NULL);
}

static void test_strtrim(void) {
    CHECK_OWN(mx_strtrim("   hello   "), "hello");
    CHECK_OWN(mx_strtrim("\t a b \n"), "a b");
    CHECK_OWN(mx_strtrim("a"), "a");
    CHECK_OWN(mx_strtrim(""), "");
    CHECK_OWN(mx_strtrim("   "), "");
    CHECK_PTR(mx_strtrim(NULL), NULL);
}

static void test_del_extra_spaces(void) {
    CHECK_OWN(mx_del_extra_spaces("  hello   world  "), "hello world");
    CHECK_OWN(mx_del_extra_spaces(""), "");
    CHECK_OWN(mx_del_extra_spaces("   "), "");
    CHECK_OWN(mx_del_extra_spaces("a"), "a");
    CHECK_OWN(mx_del_extra_spaces("a  b"), "a b");
    CHECK_OWN(mx_del_extra_spaces("\t a \n b "), "a b");
    CHECK_PTR(mx_del_extra_spaces(NULL), NULL);
}

static void test_strsplit(void) {
    char **p = mx_strsplit("one,two,three", ',');

    REQUIRE(p != NULL);
    CHECK_INT(arr_len(p), 3);
    CHECK_STR(p[0], "one");
    CHECK_STR(p[1], "two");
    CHECK_STR(p[2], "three");
    mx_del_strarr(&p);
    CHECK_PTR(p, NULL);

    p = mx_strsplit(",,a,,b,", ',');
    REQUIRE(p != NULL);
    CHECK_INT(arr_len(p), 2);
    CHECK_STR(p[0], "a");
    CHECK_STR(p[1], "b");
    mx_del_strarr(&p);

    p = mx_strsplit("abc", ',');
    REQUIRE(p != NULL);
    CHECK_INT(arr_len(p), 1);
    CHECK_STR(p[0], "abc");
    mx_del_strarr(&p);

    CHECK_PTR(mx_strsplit(NULL, ','), NULL);
}

static void test_replace_substr(void) {
    CHECK_OWN(mx_replace_substr("one two two", "two", "three"), "one three three");
    CHECK_OWN(mx_replace_substr("abc", "abc", ""), "");
    CHECK_OWN(mx_replace_substr("abc", "xyz", "Q"), "abc");
    CHECK_OWN(mx_replace_substr("abc", "xyz", "Q"), "abc");
    CHECK_PTR(mx_replace_substr(NULL, "a", "b"), NULL);
    CHECK_PTR(mx_replace_substr("a", NULL, "b"), NULL);
    CHECK_PTR(mx_replace_substr("a", "a", NULL), NULL);
}

static void test_str_reverse(void) {
    char even[] = "abcd";
    char odd[] = "abc";
    char one[] = "a";
    char empty[] = "";

    mx_str_reverse(even);
    CHECK_STR(even, "dcba");
    mx_str_reverse(odd);
    CHECK_STR(odd, "cba");
    mx_str_reverse(one);
    CHECK_STR(one, "a");
    mx_str_reverse(empty);
    CHECK_STR(empty, "");
    mx_str_reverse(NULL);
}

static void test_swap_char(void) {
    char a = 'a';
    char b = 'b';

    mx_swap_char(&a, &b);
    CHECK_INT(a, 'b');
    CHECK_INT(b, 'a');
    mx_swap_char(&a, &a);
    CHECK_INT(a, 'b');
}

static void test_strdel(void) {
    char *s = mx_strdup("abc");

    REQUIRE(s != NULL);
    mx_strdel(&s);
    CHECK_PTR(s, NULL);
}

void run_string_tests(void) {
    test_strlen();
    test_strcpy();
    test_strncpy();
    test_strdup();
    test_strndup();
    test_strnew();
    test_strcat();
    test_strcmp();
    test_strncmp();
    test_strchr_strstr();
    test_indexes();
    test_counts();
    test_count_symbol();
    test_strjoin();
    test_strtrim();
    test_del_extra_spaces();
    test_strsplit();
    test_replace_substr();
    test_str_reverse();
    test_swap_char();
    test_strdel();
}
