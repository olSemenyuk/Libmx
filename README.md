# Libmx

A small C utility library that builds as the static archive `libmx.a`. It provides string, memory, list, numeric, and output helpers behind one public header: [`inc/libmx.h`](inc/libmx.h).

## Quick Start

### Requirements

- A C11 compiler: Clang or GCC
- Make and a POSIX environment (Linux or macOS)

Windows is not supported: `mx_realloc` relies on `malloc_size` (macOS) / `malloc_usable_size` (glibc).

### Build

```sh
make              # uses clang
make CC=gcc       # use GCC instead
```

This creates `libmx.a` in the project root. The library is compiled with `-std=c11 -Wall -Wextra -Werror -Wpedantic`, so any warning fails the build.

### Use the library

Include `libmx.h`, then link your program with the archive:

```c
#include "libmx.h"
#include <stdlib.h>

int main(void) {
    char *number = mx_itoa(-42);

    if (number == NULL)
        return 1;
    mx_printstr(number);
    mx_printchar('\n');
    free(number);
    return 0;
}
```

Compile from the project root:

```sh
clang -std=c11 -Wall -Wextra -Werror -Iinc main.c libmx.a -o app
./app
```

Use `gcc` in place of `clang` if preferred.

## What's Included

| Area | Functions |
| --- | --- |
| Strings | `mx_strlen`, `mx_strcpy`, `mx_strncpy`, `mx_strcat`, `mx_strcmp`, `mx_strncmp`, `mx_strchr`, `mx_strstr`, `mx_strdup`, `mx_strndup`, `mx_strnew`, `mx_strdel`, `mx_strjoin`, `mx_strtrim`, `mx_del_extra_spaces`, `mx_strsplit`, `mx_del_strarr`, `mx_replace_substr`, `mx_str_reverse`, `mx_swap_char` |
| Searching and counting | `mx_get_char_index`, `mx_get_substr_index`, `mx_count_substr`, `mx_count_words`, `mx_count_symbol`, `mx_count_letters` |
| Memory | `mx_memset`, `mx_memcpy`, `mx_memccpy`, `mx_memmove`, `mx_memcmp`, `mx_memchr`, `mx_memrchr`, `mx_memmem`, `mx_realloc` |
| Lists | `mx_create_node`, `mx_push_front`, `mx_push_back`, `mx_pop_front`, `mx_pop_back`, `mx_list_size`, `mx_sort_list` |
| Numbers and characters | `mx_atoi`, `mx_itoa`, `mx_pow`, `mx_sqrt`, `mx_nbr_to_hex`, `mx_hex_to_nbr`, `mx_isalpha`, `mx_isdigit`, `mx_islower`, `mx_isupper`, `mx_isspace` |
| Output | `mx_printchar`, `mx_printstr`, `mx_printint`, `mx_print_unicode`, `mx_print_strarr`, `mx_printerr` |
| Algorithms and I/O | `mx_binary_search`, `mx_bubble_sort`, `mx_quicksort`, `mx_foreach`, `mx_file_to_str` |

See [`inc/libmx.h`](inc/libmx.h) for the exact prototypes and types.

## Memory Ownership

The caller owns everything the library allocates and must release it.

| Returned by | Release with |
| --- | --- |
| `mx_strnew`, `mx_strdup`, `mx_strndup`, `mx_strjoin`, `mx_strtrim`, `mx_del_extra_spaces`, `mx_replace_substr`, `mx_itoa`, `mx_nbr_to_hex`, `mx_file_to_str` | `free(s)`, or `mx_strdel(&s)`, which also sets `s` to `NULL` |
| `mx_strsplit` | `mx_del_strarr(&arr)`, which frees every string and the array, then sets `arr` to `NULL` |
| `mx_create_node`, `mx_push_front`, `mx_push_back` | `mx_pop_front` / `mx_pop_back`, or `free(node)` |
| `mx_realloc` | `free()`, like the standard `realloc` |

Notes:

- List nodes store the `data` pointer and never copy or free it. `mx_pop_*` frees the node only.
- Always assign the result of an allocating call to a variable. Overwriting a pointer before releasing it is a leak.
- Check results for `NULL` where allocation can fail.

## Behavior Notes

These behaviors are verified by the test suite.

| Function | Behavior |
| --- | --- |
| `mx_get_char_index`, `mx_get_substr_index` | Index, `-1` if not found, `-2` if an argument is `NULL` |
| `mx_count_substr`, `mx_count_words` | Count, `-1` if the string is `NULL` |
| `mx_strjoin` | `NULL` + `NULL` gives `NULL`; one `NULL` argument gives a copy of the other |
| `mx_strtrim`, `mx_del_extra_spaces` | `NULL` gives `NULL`; a string of only whitespace gives `""` |
| `mx_strsplit` | Empty tokens are skipped; the array ends with `NULL`; `NULL` input gives `NULL` |
| `mx_strncpy` | Like standard `strncpy`: pads with `\0`, does not terminate if `len <= strlen(src)` |
| `mx_strcat` | The destination must already have enough room |
| `mx_str_reverse` | Reverses in place and prints nothing; `NULL` is ignored |
| `mx_memcmp`, `mx_memchr`, `mx_memrchr`, `mx_memccpy` | Bytes are treated as `unsigned char`; `mx_memccpy` returns a pointer just past the copied byte, or `NULL` |
| `mx_memmove` | Safe for overlapping buffers in both directions |
| `mx_realloc` | `mx_realloc(NULL, n)` behaves like `malloc`; `mx_realloc(p, 0)` frees `p` and returns `NULL` |
| `mx_sqrt` | Integer root for perfect squares only; `0` otherwise, including negatives |
| `mx_atoi` | Skips leading whitespace, accepts a sign, clamps to `INT_MAX` / `INT_MIN` on overflow |
| `mx_hex_to_nbr` | Accepts both letter cases; `0` for `NULL`, empty, or invalid input |
| `mx_nbr_to_hex` | Lowercase digits, no prefix |
| `mx_binary_search` | Index of the match, `-1` if absent; `*count` receives the number of comparisons (`0` if absent) |
| `mx_bubble_sort` | Sorts lexicographically and returns the number of swaps |
| `mx_print_strarr` | Prints the elements separated by the delimiter, then a newline; does nothing for `NULL` |
| `mx_file_to_str` | `NULL` if the file cannot be opened |

## Testing

```sh
make test
```

This builds the library and the tests with AddressSanitizer and UndefinedBehaviorSanitizer, then runs them. It uses separate output paths (`obj/san/`, `build/libmx_san.a`), so your regular `libmx.a` is not touched. UndefinedBehaviorSanitizer is configured to abort on the first error.

To run the tests without sanitizers, for example under Valgrind:

```sh
make test-bin
./build/test_runner
```

### Memory-leak checking

AddressSanitizer detects memory leaks on **Linux only**. On macOS a leaking test still passes locally, so GitHub Actions is the reference. To check leaks locally on macOS, build without sanitizers and use the system tool:

```sh
clang -g -O0 -Iinc -std=c11 src/*.c tests/*.c -o build/leaks_test
leaks --atExit -- ./build/leaks_test
```

### Continuous integration

GitHub Actions runs `make` and `make test` on every push and on pull requests to `master`, using Clang and GCC on Linux and Clang on macOS.

### Writing tests

Tests live in `tests/`, one file per area (`test_string.c`, `test_memory.c`, `test_list.c`, `test_numeric.c`, `test_io.c`). Every library function has its own `static void test_<name>(void)`, and the `run_*_tests()` function at the bottom of the file calls them in order. The Makefile picks up every `tests/*.c` file automatically.

The helpers in `tests/test.h`:

| Macro | Use |
| --- | --- |
| `CHECK(cond)` | Non-fatal check; prints file and line on failure |
| `CHECK_INT(a, b)`, `CHECK_STR(a, b)`, `CHECK_PTR(a, b)`, `CHECK_MEM(a, b, n)` | Same, and print the actual and expected values |
| `CHECK_OWN(expr, "text")` | Compare a returned string with `CHECK_STR` and then `free` it |
| `REQUIRE(cond)` | Fatal check; use before dereferencing a result |

Cover the edge cases for every function: `NULL`, empty input, boundary values, and the "not found" result. A test that only covers the happy path proves very little.

## Make Targets

| Command | Result |
| --- | --- |
| `make` / `make all` / `make install` | Build `libmx.a` |
| `make test` | Build and run the sanitizer-enabled tests |
| `make test-bin` | Build the regular test runner at `build/test_runner` |
| `make clean` | Remove `obj/` and `build/` |
| `make uninstall` | Run `clean` and remove `libmx.a` |
| `make reinstall` | `uninstall`, then build again |

## Project Layout

```text
inc/libmx.h                Public API (the only header users include)
src/                       One .c file per function
tests/                     Test framework (test.h) and tests grouped by area
.github/workflows/ci.yml   GitHub Actions workflow
Makefile                   Build and test targets
```
