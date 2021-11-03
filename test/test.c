/* Copyright (C) 2021 Peter Elliott
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "test.h"
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

static struct test *tests = NULL;
static size_t tests_size = 0;
static size_t tests_cap = 0;

void add_testcase(struct test const* test) {
    if (tests_size == tests_cap) {
        tests_cap = (tests_cap)? 2*tests_cap : 32;
        tests = realloc(tests, tests_cap * sizeof(struct test));
    }

    tests[tests_size++] = *test;
}

static jmp_buf testcase_assert_target;
static char const* fail_msg = NULL;

void fail(char const* msg) {
    fail_msg = msg;
    longjmp(testcase_assert_target, 1);
}

static bool run_test(void (*test)(void)) {
    if (setjmp(testcase_assert_target) != 0)
        return false;

    test();
    return true;
}

int main() {
    char const* lastfile = NULL;
    size_t passcount = 0;

    fprintf(stderr, "RUNNING %lu TESTS...\n", tests_size);
    for (size_t i = 0; i < tests_size; ++i) {
        char const* file = tests[i].filename;
        if (file != lastfile)
            fprintf(stderr, "\n%s:\n", file);

        fprintf(stderr, "    %-32s", tests[i].testname);

        if (!tests[i].enabled) {
            passcount++;
            fprintf(stderr, "\e[1;33;7m%s\e[0m\n", "SKIPPED");
        } else if (run_test(tests[i].testcase)) {
            passcount++;
            fprintf(stderr, "\e[1;32;7m%s\e[0m\n", "PASSED");
        } else {
            fprintf(stderr, "\e[1;31;7m%s\e[0m\n", "FAILED");
            if (fail_msg)
                fprintf(stderr, "%s\n", fail_msg);
        }

        lastfile = file;
    }

    fprintf(stderr, "(%lu/%lu) ", passcount, tests_size);
    if (passcount == tests_size) {
        fprintf(stderr, "\e[1;32;7m%s\e[0m\n", "PASSED");
        return 0;
    } else {
        fprintf(stderr, "\e[1;31;7m%s\e[0m\n", "FAILED");
        return 1;
    }

}
