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
#include "arena.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct arena_node {
    size_t block_offset;
    void *data;
    struct arena_node *next;
};

void arena_init(struct arena *arena, size_t block_size) {
    arena->block_size = block_size;
    arena->head = NULL;
}

void arena_free(struct arena *arena) {
    struct arena_node *head = arena->head;
    while (head != NULL) {
        struct arena_node *next = head->next;
        free(head->data);
        free(head);
        head = next;
    }
    arena->head = NULL;
}

void *arena_allocate(struct arena *arena, size_t size) {
    if (arena->head == NULL || (arena->block_size - arena->head->block_offset) < size) {
        struct arena_node *node = malloc(sizeof(struct arena_node));
        node->block_offset = 0;
        node->data = malloc(arena->block_size);
        node->next = arena->head;
        arena->head = node;
    }

    void *ptr = arena->head->data + arena->head->block_offset;
    arena->head->block_offset += size;
    assert(arena->head->block_offset <= arena->block_size);
    return ptr;
}

char *arena_strdup(struct arena *arena, char const *str) {
    size_t len = strlen(str);
    void *mem = arena_allocate(arena, len);
    memcpy(mem, str, len + 1);
    return mem;
}
