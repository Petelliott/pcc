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
#ifndef PCC_LIST_H
#define PCC_LIST_H

#include <stddef.h>
#include <assert.h>

struct list {
    size_t item_size;
    size_t size;
    size_t cap;
    void *data;
};

void list_init(struct list *list, size_t item_size);
void list_free(struct list *list);
void *list_release(struct list *list);
void *list_release_nt(struct list *list);

void list_append(struct list *list, void const *item);
#define list_appendv(list, item) {                                      \
        assert(sizeof(item) == (list)->item_size);                      \
        typeof(item) _value = item;                                     \
        list_append(list, &_value);                                     \
    }

void list_pop(struct list *list);

void *list_at_unchecked(struct list *list, size_t index);
void *list_at(struct list *list, size_t index);

#define list_foreach(type, name, list)                                  \
    size_t _i = 0;                                                      \
    for (type name = list_at(&list, _i); _i < list.size; ++_i, name=list_at_unchecked(&list,_i))

#endif
