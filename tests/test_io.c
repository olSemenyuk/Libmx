#ifndef __APPLE__
#define _POSIX_C_SOURCE 200809L
#endif

#include "libmx.h"
#include "test.h"
#include <unistd.h>

static int g_saved_fd = -1;
static int g_cap_fd_file = -1;
static int g_cap_fd = STDOUT_FILENO;
static int g_sum = 0;

static int g_calls = 0;

static void cap_begin_fd(int fd) {
    char path[] = "/tmp/libmx_capture_XXXXXX";

    fflush(NULL);

    g_cap_fd = fd;
    g_saved_fd = dup(fd);
    g_cap_fd_file = mkstemp(path);

    REQUIRE(g_saved_fd >= 0);
    REQUIRE(g_cap_fd_file >= 0);
    REQUIRE(unlink(path) == 0);

    REQUIRE(dup2(g_cap_fd_file, fd) >= 0);
}

static void cap_begin(void) {
    cap_begin_fd(STDOUT_FILENO);
}

static void cap_end(char *buf, size_t size) {
    fflush(NULL);

    REQUIRE(dup2(g_saved_fd, g_cap_fd) >= 0);

    close(g_saved_fd);
    g_saved_fd = -1;

    REQUIRE(lseek(g_cap_fd_file, 0, SEEK_SET) >= 0);
    ssize_t bytes = read(g_cap_fd_file, buf, size - 1);
    REQUIRE(bytes >= 0);
    size_t n = (size_t)bytes;
    buf[n] = '\0';

    close(g_cap_fd_file);
    g_cap_fd_file = -1;

    for (size_t i = 0; i < n; i++) {
        if (buf[i] == '\0')
            buf[i] = '\1';
    }
}

static void add(int x) {
    g_sum += x;
    g_calls++;
}

static void test_printstr(void) {
    char buf[256];

    cap_begin();
    mx_printstr("hello");
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "hello");

    cap_begin();
    mx_printstr("");
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "");

    cap_begin();
    mx_printstr(NULL);
    cap_end(buf, sizeof buf);
}

static void test_printchar(void) {
    char buf[256];

    cap_begin();
    mx_printchar('x');
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "x");
}

static void test_printint(void) {
    char buf[256];

    cap_begin();
    mx_printint(0);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "0");

    cap_begin();
    mx_printint(-123);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "-123");

    cap_begin();
    mx_printint(2147483647);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "2147483647");

    cap_begin();
    mx_printint(-2147483647 - 1);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "-2147483648");
}

static void test_print_unicode(void) {
    char buf[256];

    cap_begin();
    mx_print_unicode(0x41);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "A");

    cap_begin();
    mx_print_unicode(0x0407);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "\xD0\x87");

    cap_begin();
    mx_print_unicode(0x20AC);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "\xE2\x82\xAC");

    cap_begin();
    mx_print_unicode(0x1F600);
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "\xF0\x9F\x98\x80");
}

static void test_print_strarr(void) {
    char *arr[] = {"a", "b", "c", NULL};
    char buf[256];

    cap_begin();
    mx_print_strarr(arr, ",");
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "a,b,c\n");

    cap_begin();
    mx_print_strarr(NULL, ",");
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "");
}

static void test_printerr(void) {
    char buf[256];

    cap_begin_fd(STDERR_FILENO);
    mx_printerr("oops");
    cap_end(buf, sizeof buf);
    CHECK_STR(buf, "oops");
}

static void test_foreach(void) {
    int arr[] = {1, 2, 3, 4};

    g_sum = 0;
    g_calls = 0;
    mx_foreach(arr, 4, add);
    CHECK_INT(g_sum, 10);
    CHECK_INT(g_calls, 4);

    g_calls = 0;
    mx_foreach(arr, 0, add);
    CHECK_INT(g_calls, 0);
}

static int make_tmp(char *path) {
    strcpy(path, "/tmp/libmx_test_XXXXXX");
    return mkstemp(path);
}

static void test_file_to_str(void) {
    char path[64];
    const char data[] = "line one\nline two\n";
    size_t big_len = 10000;
    char *big = malloc(big_len + 1);
    int fd = make_tmp(path);

    REQUIRE(fd >= 0 && big != NULL);
    REQUIRE(write(fd, data, sizeof data - 1) == (ssize_t)(sizeof data - 1));
    close(fd);
    CHECK_OWN(mx_file_to_str(path), data);
    unlink(path);

    for (size_t i = 0; i < big_len; i++)
        big[i] = (char)('a' + i % 26);
    big[big_len] = '\0';
    fd = make_tmp(path);
    REQUIRE(fd >= 0);
    REQUIRE(write(fd, big, big_len) == (ssize_t)big_len);
    close(fd);
    CHECK_OWN(mx_file_to_str(path), big);
    unlink(path);
    free(big);

    CHECK_PTR(mx_file_to_str("/nonexistent/libmx_no_such_file"), NULL);
}

void run_io_tests(void) {
    test_printstr();
    test_printchar();
    test_printint();
    test_print_unicode();
    test_print_strarr();
    test_printerr();
    test_foreach();
    test_file_to_str();
}
