#include "test.h"

int g_checks = 0;
int g_failed = 0;

void run_string_tests(void);
void run_memory_tests(void);
void run_list_tests(void);
void run_numeric_tests(void);
void run_io_tests(void);

int main(void) {
    run_string_tests();
    run_memory_tests();
    run_list_tests();
    run_numeric_tests();
    run_io_tests();
    printf("checks: %d, failed: %d\n", g_checks, g_failed);
    return g_failed != 0;
}
