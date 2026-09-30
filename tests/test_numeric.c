#include "libmx.h"
#include "test.h"
#include <limits.h>

static void ctype_case(const char *name, int c, int got, int exp) {
    g_checks++;
    if ((got != 0) != (exp != 0)) {
        g_failed++;
        fprintf(stderr, "%s:%d: FAIL: %s(%d) returned %d, expected %d\n", __FILE__, __LINE__, name, c, got != 0, exp != 0);
    }
}

static void test_ctype(void) {
    for (int c = 0; c < 128; c++) {
        int lower = c >= 'a' && c <= 'z';
        int upper = c >= 'A' && c <= 'Z';
        int digit = c >= '0' && c <= '9';
        int space = c == ' ' || (c >= '\t' && c <= '\r');

        ctype_case("mx_islower", c, mx_islower(c), lower);
        ctype_case("mx_isupper", c, mx_isupper(c), upper);
        ctype_case("mx_isalpha", c, mx_isalpha(c), lower || upper);
        ctype_case("mx_isdigit", c, mx_isdigit(c), digit);
        ctype_case("mx_isspace", c, mx_isspace(c), space);
    }
}

static void test_atoi(void) {
    CHECK_INT(mx_atoi(" -42abc"), -42);
    CHECK_INT(mx_atoi(" +7 "), 7);
    CHECK_INT(mx_atoi("0"), 0);
    CHECK_INT(mx_atoi("123"), 123);
    CHECK_INT(mx_atoi(""), 0);
    CHECK_INT(mx_atoi("abc"), 0);
    CHECK_INT(mx_atoi("+"), 0);
    CHECK_INT(mx_atoi("2147483647"), INT_MAX);
    CHECK_INT(mx_atoi("-2147483648"), INT_MIN);
}

static void test_itoa(void) {
    CHECK_OWN(mx_itoa(0), "0");
    CHECK_OWN(mx_itoa(7), "7");
    CHECK_OWN(mx_itoa(-1), "-1");
    CHECK_OWN(mx_itoa(-42), "-42");
    CHECK_OWN(mx_itoa(10), "10");
    CHECK_OWN(mx_itoa(100), "100");
    CHECK_OWN(mx_itoa(1000000), "1000000");
    CHECK_OWN(mx_itoa(INT_MAX), "2147483647");
    CHECK_OWN(mx_itoa(INT_MIN), "-2147483648");
}

static void test_pow_sqrt(void) {
    CHECK(mx_pow(2.0, 3) == 8.0);
    CHECK(mx_pow(5.0, 0) == 1.0);
    CHECK(mx_pow(5.0, 1) == 5.0);
    CHECK(mx_pow(10.0, 5) == 100000.0);
    CHECK(mx_pow(0.0, 3) == 0.0);

    CHECK_INT(mx_sqrt(0), 0);
    CHECK_INT(mx_sqrt(1), 1);
    CHECK_INT(mx_sqrt(16), 4);
    CHECK_INT(mx_sqrt(17), 0);
    CHECK_INT(mx_sqrt(-4), 0);
    CHECK_INT(mx_sqrt(2147395600), 46340);
    CHECK_INT(mx_sqrt(INT_MAX), 0);
}

static void test_nbr_to_hex(void) {
    CHECK_OWN(mx_nbr_to_hex(0), "0");
    CHECK_OWN(mx_nbr_to_hex(9), "9");
    CHECK_OWN(mx_nbr_to_hex(15), "f");
    CHECK_OWN(mx_nbr_to_hex(16), "10");
    CHECK_OWN(mx_nbr_to_hex(254), "fe");
    CHECK_OWN(mx_nbr_to_hex(255), "ff");
    CHECK_OWN(mx_nbr_to_hex(4096), "1000");
    CHECK_OWN(mx_nbr_to_hex(0xdeadbeefUL), "deadbeef");

    if (sizeof(unsigned long) == 8) CHECK_OWN(mx_nbr_to_hex(ULONG_MAX), "ffffffffffffffff");
}

static void test_hex_to_nbr(void) {
    CHECK_INT(mx_hex_to_nbr("ff"), 255);
    CHECK_INT(mx_hex_to_nbr("FF"), 255);
    CHECK_INT(mx_hex_to_nbr("fe"), 254);
    CHECK_INT(mx_hex_to_nbr("0"), 0);
    CHECK_INT(mx_hex_to_nbr(""), 0);
    CHECK_INT(mx_hex_to_nbr(NULL), 0);
    CHECK_INT(mx_hex_to_nbr("1000"), 4096);
    CHECK_INT(mx_hex_to_nbr("deadbeef"), 0xdeadbeefUL);
    CHECK_INT(mx_hex_to_nbr("xyz"), 0);
    CHECK_INT(mx_hex_to_nbr("12g4"), 0);

    for (unsigned long x = 0; x < 5000; x += 7) {
        char *h = mx_nbr_to_hex(x);

        REQUIRE(h != NULL);
        CHECK_INT(mx_hex_to_nbr(h), x);
        free(h);
    }
}

static void test_binary_search(void) {
    char *arr[] = {"apple", "banana", "cherry"};
    int count = 0;

    CHECK_INT(mx_binary_search(arr, 3, "banana", &count), 1);
    CHECK_INT(count, 1);

    count = 0;
    CHECK_INT(mx_binary_search(arr, 3, "apple", &count), 0);
    CHECK_INT(mx_binary_search(arr, 3, "cherry", &count), 2);
    CHECK_INT(mx_binary_search(arr, 3, "zzz", &count), -1);
    CHECK_INT(count, 0);
    CHECK_INT(mx_binary_search(arr, 3, "aaa", &count), -1);
    CHECK_INT(mx_binary_search(arr, 0, "apple", &count), -1);
}

static void test_bubble_sort(void) {
    char *sort_arr[] = {"pear", "apple", "grape"};
    char *sorted[] = {"a", "b", "c"};
    char *rev[] = {"c", "b", "a"};
    char *one[] = {"x"};

    CHECK_INT(mx_bubble_sort(sort_arr, 3), 2);
    CHECK_STR(sort_arr[0], "apple");
    CHECK_STR(sort_arr[1], "grape");
    CHECK_STR(sort_arr[2], "pear");

    CHECK_INT(mx_bubble_sort(sorted, 3), 0);
    CHECK_INT(mx_bubble_sort(rev, 3), 3);
    CHECK_STR(rev[0], "a");
    CHECK_STR(rev[2], "c");
    CHECK_INT(mx_bubble_sort(one, 1), 0);
    CHECK_INT(mx_bubble_sort(one, 0), 0);
}

static void test_quicksort(void) {
    char *arr[] = {"dddd", "ccc", "a", "bb"};
    char *one[] = {"x"};

    mx_quicksort(arr, 0, 3);
    CHECK_STR(arr[0], "a");
    CHECK_STR(arr[1], "bb");
    CHECK_STR(arr[2], "ccc");
    CHECK_STR(arr[3], "dddd");

    mx_quicksort(one, 0, 0);
    CHECK_STR(one[0], "x");
    CHECK_INT(mx_quicksort(NULL, 0, 0), -1);
}

void run_numeric_tests(void) {
    test_ctype();
    test_atoi();
    test_itoa();
    test_pow_sqrt();
    test_nbr_to_hex();
    test_hex_to_nbr();
    test_binary_search();
    test_bubble_sort();
    test_quicksort();
}
