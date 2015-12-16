/**
 * tests.h:
 *   Testing utils.
 *
 * Copyright (C) 2015 Emilio Cobos Álvarez (70912324N) <emiliocobos@usal.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TESTS_H
#define TESTS_H
#include <stdlib.h>
#include <stdio.h>

#define STATIC_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

#define ASSERT(expr)                                                           \
    do {                                                                       \
        if (!(expr)) {                                                         \
            fprintf(stderr, "\tAssertion failed (%s, %d): %s\n", __FILE__,     \
                    __LINE__, #expr);                                          \
            return -1;                                                         \
        }                                                                      \
    } while (0)

#define ASSERT_FALSE(expr) ASSERT(!(expr))

#define TEST(fn, ...)                                                          \
    int test_##fn() {                                                          \
        printf("Running: %s (%s:%d)\n", #fn, __FILE__, __LINE__);              \
        __VA_ARGS__                                                            \
        return 0;                                                              \
    }

#define RUN_TEST(name)                                                         \
    do {                                                                       \
        const char* result_string;                                             \
        run++;                                                                 \
        if (test_##name() == -1) {                                             \
            failures++;                                                        \
            result_string = "fail";                                            \
        } else {                                                               \
            result_string = "ok";                                              \
        }                                                                      \
        printf("result: %s\n", result_string);                                 \
    } while (0);

#define TEST_MAIN(...)                                                         \
    int main() {                                                               \
        size_t failures = 0;                                                   \
        size_t run = 0;                                                        \
        __VA_ARGS__                                                            \
        printf("\nrun: %zu, pass: %zu, fail: %zu\n\n", run, run - failures,    \
               failures);                                                      \
        if (failures) {                                                        \
            printf("Some tests failed, see avobe for more details\n");         \
            return 1;                                                          \
        }                                                                      \
        printf("Everything ok!\n");                                            \
        return 0;                                                              \
    }

#endif
