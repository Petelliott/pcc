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
#include <stddef.h>
#include <stdlib.h>
#include <datastructures/hashmap.h>
#include <string.h>

testcase(hashmap_init_free) {
    struct hashmap hashmap;
    hashmap_init(&hashmap, ptr_value_hash, ptr_value_cmp, NULL);
    ASSERT(hashmap.entries == 0);
    hashmap_free(&hashmap);
}

testcase(hashmap_insert_value) {
    struct hashmap hashmap;
    hashmap_init(&hashmap, ptr_value_hash, ptr_value_cmp, NULL);

    size_t n = 1000;
    void *ptrs[n];
    for (size_t i = 0; i < n; ++i) {
        // fake ptrs are not 0 or 1.
        ptrs[i] = (void *)(rand() % n) + 2;
        hashmap_insert_value(&hashmap, ptrs[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        ASSERT(hashmap_has_value(&hashmap, ptrs[i]));
    }

    for (size_t i = n + 2; i < 10000; ++i) {
        ASSERT(!hashmap_has_value(&hashmap, (void *)i));
    }

    hashmap_free(&hashmap);
}

testcase(hashmap_delete_value) {
    struct hashmap hashmap;
    hashmap_init(&hashmap, ptr_value_hash, ptr_value_cmp, NULL);

    size_t n = 1000;
    void *ptrs[n];
    for (size_t i = 0; i < n; ++i) {
        // fake ptrs are not 0 or 1.
        ptrs[i] = (void *)(i + 2);
        hashmap_insert_value(&hashmap, ptrs[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        if (i % 2)
            hashmap_delete_value(&hashmap, ptrs[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        if (i % 2) {
            ASSERT(!hashmap_has_value(&hashmap, ptrs[i]));
        } else {
            ASSERT(hashmap_has_value(&hashmap, ptrs[i]));
        }
    }

    hashmap_free(&hashmap);
}

testcase(hashmap_insert) {
    struct hashmap hashmap;
    hashmap_init(&hashmap, ptr_key_hash, ptr_key_cmp, free);

    for (size_t i = 0; i < 1000; ++i) {
        hashmap_insert(&hashmap, (void *) i, (void *) i + 7);
    }
    for (size_t i = 0; i < 500; ++i) {
        hashmap_insert(&hashmap, (void *) i, (void *) i + 8);
    }

    for (size_t i = 0; i < 500; ++i) {
        ASSERT(hashmap_get(&hashmap, (void *)i) == (void *) i + 8);
    }
    for (size_t i = 500; i < 1000; ++i) {
        ASSERT(hashmap_get(&hashmap, (void *)i) == (void *) i + 7);
    }

    hashmap_free(&hashmap);
}

testcase(hashmap_string) {
    struct hashmap hashmap;
    hashmap_init(&hashmap, string_key_hash, string_key_cmp, free);

    hashmap_insert(&hashmap, "peter", "elliott");
    hashmap_insert(&hashmap, "key", "value");
    hashmap_insert(&hashmap, "lhs", "rhs");

    ASSERT(strcmp(hashmap_get(&hashmap, "peter"), "elliott") == 0);
    ASSERT(strcmp(hashmap_get(&hashmap, "key"), "value") == 0);
    ASSERT(strcmp(hashmap_get(&hashmap, "lhs"), "rhs") == 0);

    ASSERT(hashmap_get(&hashmap, "peterr") == NULL);
    ASSERT(hashmap_get(&hashmap, "pete") == NULL);

    hashmap_free(&hashmap);
}
