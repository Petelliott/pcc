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
#ifndef PCC_ARENA_H
#define PCC_ARENA_H

#include <stddef.h>

struct arena_node;
struct arena {
    size_t block_size;
    struct arena_node *head;
};

void arena_init(struct arena *arena, size_t block_size);
void arena_free(struct arena *arena);

void *arena_allocate(struct arena *arena, size_t size);

char *arena_strdup(struct arena *arena, char const *str);

#endif
