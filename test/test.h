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
#ifndef PCC_TEST_H
#define PCC_TEST_H

#include <stdbool.h>

struct test {
    char const* filename;
    char const* testname;
    void (*testcase)(void);
    bool enabled;
};

void add_testcase(struct test const* test);

#define testcase(name)                                                  \
    static void _testcase_##name(void);                                 \
    __attribute__((constructor))                                        \
    static void _test_constructor_##name(void) {                        \
        struct test test = {                                            \
            .filename = __FILE__,                                       \
            .testname = #name,                                          \
            .testcase = _testcase_##name,                               \
            .enabled = true,                                            \
        };                                                              \
        add_testcase(&test);                                            \
    }                                                                   \
    static void _testcase_##name(void)


void fail(char const* msg);

#define ASSERT(expr) if (!(expr)) fail(#expr);

#endif
