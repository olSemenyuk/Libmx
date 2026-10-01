#include "libmx.h"
#include "test.h"

static void test_memset(void) {
    char buf[16];

    memset(buf, 0, sizeof buf);
    CHECK_PTR(mx_memset(buf, 'x', 5), buf);
    CHECK_MEM(buf, "xxxxx\0\0", 7);

    mx_memset(buf, 'z', 0);
    CHECK_MEM(buf, "xxxxx\0\0", 7);
}

static void test_memcpy(void) {
    char dst[16] = {0};

    CHECK_PTR(mx_memcpy(dst, "abcd", 5), dst);
    CHECK_STR(dst, "abcd");

    CHECK_PTR(mx_memcpy(dst, "ZZ", 0), dst);
    CHECK_STR(dst, "abcd");

    mx_memcpy(dst, "\xE4\x80\xFF", 3);
    CHECK_MEM(dst, "\xE4\x80\xFF", 3);
}

static void test_memmove(void) {
    char a[] = "abcdef";
    char b[] = "abcdef";
    char c[] = "1234";
    char d[] = "abcd";

    CHECK_PTR(mx_memmove(a + 2, a, 4), a + 2);
    CHECK_MEM(a, "ababcd", 7);

    CHECK_PTR(mx_memmove(b, b + 2, 4), b);
    CHECK_MEM(b, "cdefef", 7);

    CHECK_PTR(mx_memmove(d, c, 4), d);
    CHECK_STR(d, "1234");

    mx_memmove(d, "ZZ", 0);
    CHECK_STR(d, "1234");
}

static void test_memcmp(void) {
    CHECK_INT(mx_memcmp("abc", "abc", 3), 0);
    CHECK(mx_memcmp("abc", "abd", 3) < 0);
    CHECK(mx_memcmp("abd", "abc", 3) > 0);
    CHECK_INT(mx_memcmp("abc", "abd", 2), 0);
    CHECK_INT(mx_memcmp("a", "b", 0), 0);
    CHECK(mx_memcmp("\x80", "\x01", 1) > 0);
    CHECK(mx_memcmp("\xFF", "\x00", 1) > 0);
    CHECK(mx_memcmp("\x01", "\x80", 1) < 0);
}

static void test_memchr(void) {
    const char b[] = "banana";

    CHECK_PTR(mx_memchr(b, 'n', 6), b + 2);
    CHECK_PTR(mx_memchr(b, 'b', 6), b);
    CHECK_PTR(mx_memchr(b, 'a', 6), b + 1);
    CHECK_PTR(mx_memchr(b, 'n', 2), NULL);
    CHECK_PTR(mx_memchr(b, 'z', 6), NULL);
    CHECK_PTR(mx_memchr(b, 'a', 0), NULL);

    const char hi[] = "\x01\xE4";
    CHECK_PTR(mx_memchr(hi, 0xE4, 2), hi + 1);
}

static void test_memrchr(void) {
    const char b[] = "banana";

    CHECK_PTR(mx_memrchr(b, 'a', 6), b + 5);
    CHECK_PTR(mx_memrchr(b, 'n', 6), b + 4);
    CHECK_PTR(mx_memrchr(b, 'b', 6), b);
    CHECK_PTR(mx_memrchr(b, 'a', 5), b + 3);
    CHECK_PTR(mx_memrchr(b, 'z', 6), NULL);
    CHECK_PTR(mx_memrchr(b, 'a', 0), NULL);
}

static void test_memmem(void) {
    const char big[] = "abcdef";

    CHECK_PTR(mx_memmem(big, 6, "cd", 2), big + 2);
    CHECK_PTR(mx_memmem(big, 6, "ab", 2), big);
    CHECK_PTR(mx_memmem(big, 6, "ef", 2), big + 4);
    CHECK_PTR(mx_memmem(big, 6, "abcdef", 6), big);
    CHECK_PTR(mx_memmem(big, 6, "xy", 2), NULL);
    CHECK_PTR(mx_memmem(big, 2, "abc", 3), NULL);
    CHECK_PTR(mx_memmem(big, 5, "ef", 2), NULL);

    const char aab[] = "aab";
    CHECK_PTR(mx_memmem(aab, 3, "ab", 2), aab + 1);
    CHECK_PTR(mx_memmem(big, 6, "c", 1), big + 2);
    CHECK_PTR(mx_memmem(big, 6, "f", 1), big + 5);
    CHECK_PTR(mx_memmem(big, 0, "a", 1), NULL);
}

static void test_memccpy(void) {
    char d[8];
    void *r;

    memset(d, 'x', sizeof d);
    r = mx_memccpy(d, "abcdef", 'c', 6);
    CHECK_PTR(r, d + 3);
    CHECK_MEM(d, "abcxxxxx", 8);

    memset(d, 'x', sizeof d);
    r = mx_memccpy(d, "abc", 'c', 3);
    CHECK_PTR(r, d + 3);
    CHECK_MEM(d, "abcxxxxx", 8);

    memset(d, 'x', sizeof d);
    r = mx_memccpy(d, "abcdef", 'c', 2);
    CHECK_PTR(r, NULL);
    CHECK_MEM(d, "abxxxxxx", 8);

    memset(d, 'x', sizeof d);
    r = mx_memccpy(d, "abcdef", 'z', 4);
    CHECK_PTR(r, NULL);
    CHECK_MEM(d, "abcdxxxx", 8);

    memset(d, 'x', sizeof d);
    r = mx_memccpy(d, "abc", 'a', 3);
    CHECK_PTR(r, d + 1);
    CHECK_MEM(d, "axxxxxxx", 8);

    memset(d, 'x', sizeof d);
    r = mx_memccpy(d, "abc", 'c', 0);
    CHECK_PTR(r, NULL);
    CHECK_MEM(d, "xxxxxxxx", 8);

    memset(d, 0, sizeof d);
    r = mx_memccpy(d, "\xE4\x01", 0xE4, 2);
    CHECK_PTR(r, d + 1);
}

static void test_realloc(void) {
    char *p = malloc(4);
    char *g;
    char *n;

    REQUIRE(p != NULL);
    strcpy(p, "abc");
    g = mx_realloc(p, 16);
    REQUIRE(g != NULL);
    CHECK_STR(g, "abc");

    g = mx_realloc(g, 2);
    REQUIRE(g != NULL);
    CHECK_MEM(g, "ab", 2);
    free(g);

    p = malloc(100);
    REQUIRE(p != NULL);
    for (int i = 0; i < 100; i++)
        p[i] = (char)i;
    g = mx_realloc(p, 1000);
    REQUIRE(g != NULL);
    for (int i = 0; i < 100; i++)
        CHECK_INT((unsigned char)g[i], i);
    free(g);

    n = mx_realloc(NULL, 8);
    CHECK(n != NULL);
    free(n);

    CHECK_PTR(mx_realloc(malloc(8), 0), NULL);
}

void run_memory_tests(void) {
    test_memset();
    test_memcpy();
    test_memmove();
    test_memcmp();
    test_memchr();
    test_memrchr();
    test_memmem();
    test_memccpy();
    test_realloc();
}
