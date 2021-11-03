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

#include "../test.h"
#include <datastructures/list.h>
#include <stdlib.h>
#include <string.h>

testcase(list_init_free) {
    struct list list;
    list_init(&list, sizeof(size_t));
    ASSERT(list.size == 0);
    list_free(&list);
}

testcase(list_release) {
    struct list list;
    list_init(&list, sizeof(size_t));
    size_t a = 5;
    list_append(&list, &a);
    void *data = list_release(&list);
    list_free(&list);
    free(data); // There should be no double free
}

testcase(list_release_nt) {
    struct list list;
    list_init(&list, sizeof(char));

    char const *str1 = "hello";
    for (size_t i = 0; i < 4; ++i) {
        list_append(&list, str1 + i);
    }
    char *str2 = list_release_nt(&list);
    list_free(&list);

    ASSERT(strcmp(str2, "hell") == 0);
    free(str2); // There should be no double free
}

testcase(list_append) {
    struct list list;
    list_init(&list, sizeof(size_t));

    for (size_t i = 0; i < 1000; ++i) {
        list_append(&list, &i);
    }

    ASSERT(list.size == 1000);
    ASSERT(list.cap >= list.size);

    for (size_t i = 0; i < 1000; ++i) {
        ASSERT(i == *(size_t *)list_at(&list, i));
    }

    list_free(&list);
}

testcase(list_pop) {
    struct list list;
    list_init(&list, sizeof(size_t));

    for (size_t i = 0; i < 10; ++i) {
        list_append(&list, &i);
    }

    ASSERT(list.size == 10);
    list_pop(&list);
    ASSERT(list.size == 9);
    list_pop(&list);
    ASSERT(list.size == 8);

    list_free(&list);
}

testcase(foreach) {
    struct list list;
    list_init(&list, sizeof(size_t));

    for (size_t i = 0; i < 10; ++i) {
        list_append(&list, &i);
    }

    size_t i = 0;
    list_foreach(size_t *, n, list) {
        ASSERT(*n == i++);
    }
    ASSERT(i == 10);


    list_free(&list);
}
