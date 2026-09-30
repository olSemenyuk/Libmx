#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int g_checks;
extern int g_failed;

static inline void t_fail(const char *file, int line, const char *expr) {
    g_failed++;
    fprintf(stderr, "%s:%d: FAIL: %s\n", file, line, expr);
}

static inline void t_bool(const char *f, int l, const char *expr, int ok) {
    g_checks++;
    if (!ok) t_fail(f, l, expr);
}

static inline void t_int(const char *f, int l, const char *expr, long long got, long long exp) {
    g_checks++;
    if (got != exp) {
        t_fail(f, l, expr);
        fprintf(stderr, "    got:      %lld\n    expected: %lld\n", got, exp);
    }
}

static inline void t_str(const char *f, int l, const char *expr, const char *got, const char *exp) {
    int ok = (got == NULL && exp == NULL) || (got != NULL && exp != NULL && strcmp(got, exp) == 0);

    g_checks++;
    if (!ok) {
        t_fail(f, l, expr);
        fprintf(stderr, "    got:      [%s]\n    expected: [%s]\n", got ? got : "(null)", exp ? exp : "(null)");
    }
}

static inline void t_own(const char *f, int l, const char *expr, char *got, const char *exp) {
    t_str(f, l, expr, got, exp);
    free(got);
}

static inline void t_ptr(const char *f, int l, const char *expr, const void *got, const void *exp) {
    g_checks++;
    if (got != exp) {
        t_fail(f, l, expr);
        fprintf(stderr, "    got:      %p\n    expected: %p\n", got, exp);
    }
}

static inline void t_mem(const char *f, int l, const char *expr, const void *got, const void *exp, size_t n) {
    g_checks++;
    if (memcmp(got, exp, n) != 0) t_fail(f, l, expr);
}

#define CHECK(c)            t_bool(__FILE__, __LINE__, #c, (c) ? 1 : 0)
#define CHECK_INT(a, b)     t_int(__FILE__, __LINE__, #a, (long long)(a), (long long)(b))
#define CHECK_STR(a, b)     t_str(__FILE__, __LINE__, #a, (a), (b))
#define CHECK_OWN(a, b)     t_own(__FILE__, __LINE__, #a, (a), (b))
#define CHECK_PTR(a, b)     t_ptr(__FILE__, __LINE__, #a, (a), (b))
#define CHECK_MEM(a, b, n)  t_mem(__FILE__, __LINE__, #a, (a), (b), (n))

#define REQUIRE(c) do { \
    g_checks++; \
    if (!(c)) { \
        fprintf(stderr, "%s:%d: FATAL: %s\n", __FILE__, __LINE__, #c); \
        exit(2); \
    } \
} while (0)

#endif
