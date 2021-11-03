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

#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

static const double rehash_factor = 0.8;
static const size_t min_buckets = 32;

static void * const empty = NULL;
static void * const tombstone = NULL + 1;

void hashmap_init(struct hashmap *hashmap, hash_t hash, cmp_t cmp, free_t freefn) {
    hashmap->data = NULL;
    hashmap->entries = 0;
    hashmap->buckets = 0;
    hashmap->hash = hash;
    hashmap->cmp = cmp;
    hashmap->free = freefn;
}

void hashmap_free(struct hashmap *hashmap) {
    if (hashmap->free) {
        for (size_t i = 0; i < hashmap->buckets; ++i) {
            if (hashmap->data[i] != tombstone)
                hashmap->free(hashmap->data[i]);
        }
    }
    free(hashmap->data);
    hashmap->data = NULL;
    hashmap->entries = 0;
    hashmap->buckets = 0;
}

static size_t get_bucket(struct hashmap *hashmap, void const *value, bool allow_ts) {
    size_t index = hashmap->hash(value) % hashmap->buckets;

    size_t ts_index = SIZE_MAX;
    for (; hashmap->data[index] != empty; index = (index + 1) % hashmap->buckets) {
        if (hashmap->data[index] == tombstone && ts_index == SIZE_MAX)
            ts_index = index;

        if (hashmap->cmp(hashmap->data[index], value) == 0)
            return index;

    }

    if (allow_ts && ts_index != SIZE_MAX)
        return ts_index;

    return index;
}

static void insert_op(struct hashmap *hashmap, void *value) {
    size_t index = get_bucket(hashmap, value, true);
    if (hashmap->data[index] == empty) {
        hashmap->entries++;
    } else if (hashmap->free) {
        hashmap->free(hashmap->data[index]);
    }
    hashmap->data[index] = value;
}

static void rehash(struct hashmap *hashmap, size_t new_buckets) {
    void **old_data = hashmap->data;
    size_t old_buckets = hashmap->buckets;

    hashmap->data = malloc(new_buckets * sizeof(void *));
    hashmap->buckets = new_buckets;
    hashmap->entries = 0;
    // Assume NULL == 0
    memset(hashmap->data, 0, hashmap->buckets * sizeof(void *));

    for (size_t i = 0; i < old_buckets; ++i) {
        if (old_data[i] == empty || old_data[i] == tombstone)
            continue;

        insert_op(hashmap, old_data[i]);
    }
    free(old_data);
}

void hashmap_insert_value(struct hashmap *hashmap, void *value) {
    double lf = (hashmap->entries + 0.00000000001) / hashmap->buckets;
    if (lf > rehash_factor) {
        size_t new_buckets = (hashmap->buckets)? 2*hashmap->buckets : min_buckets;
        rehash(hashmap, new_buckets);
    }

    insert_op(hashmap, value);
}

void hashmap_delete_value(struct hashmap *hashmap, void *value) {
    void **ptr = hashmap->data + get_bucket(hashmap, value, false);
    if (ptr != empty)
        *ptr = tombstone;
}

bool hashmap_has_value(struct hashmap *hashmap, void *value) {
    void *bucket = hashmap->data[get_bucket(hashmap, value, false)];
    return bucket != empty;
}

void hashmap_insert(struct hashmap *hashmap, void *key, void *value) {
    assert(hashmap->free);
    struct hashmap_entry *entry = malloc(sizeof(struct hashmap_entry));
    entry->key = key;
    entry->value = value;
    hashmap_insert_value(hashmap, entry);
}

void hashmap_delete(struct hashmap *hashmap, void *key) {
    struct hashmap_entry entry = (struct hashmap_entry) {
        .key = key,
        .value = NULL,
    };
    hashmap_delete_value(hashmap, &entry);
}

void const *hashmap_get(struct hashmap *hashmap, void *key) {
    struct hashmap_entry lookup_entry = (struct hashmap_entry) {
        .key = key,
        .value = NULL,
    };
    struct hashmap_entry *entry = hashmap->data[get_bucket(hashmap, &lookup_entry, false)];

    if (entry == empty)
        return NULL;

    return entry->value;
}

// TODO: write real hash functions.
size_t ptr_value_hash(void const *ptr) {
    return (size_t) ptr;
}

size_t ptr_key_hash(void const *ptr) {
    return ptr_value_hash(((struct hashmap_entry *) ptr)->key);
}

size_t string_value_hash(void const *ptr) {
    char const *str = ptr;
    size_t hash = 0;
    for (size_t i = 0; str[i] != 0; ++i) {
        hash = 7*hash + str[i];
    }
    return hash;
}

size_t string_key_hash(void const *ptr) {
    return string_value_hash(((struct hashmap_entry *) ptr)->key);
}

int ptr_value_cmp(void const *a, void const *b) {
    return a - b;
}

int ptr_key_cmp(void const *a, void const *b) {
    return ((struct hashmap_entry*) a)->key - ((struct hashmap_entry*) b)->key;
}

int string_value_cmp(void const *a, void const *b) {
    return strcmp(a, b);
}

int string_key_cmp(void const *a, void const *b) {
    return strcmp(((struct hashmap_entry*) a)->key, ((struct hashmap_entry*) b)->key);
}
