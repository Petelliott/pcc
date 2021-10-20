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
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


void list_init(struct list *list, size_t item_size) {
    list->item_size = item_size;
    list->size = 0;
    list->cap  = 0;
    list->data = NULL;
}

void list_free(struct list *list) {
    free(list_release(list));
}

void *list_release(struct list *list) {
    void *data = list->data;
    list->size = 0;
    list->cap  = 0;
    list->data = NULL;
    return data;
}

void *list_release_nt(struct list *list) {
    list_append(list, NULL);
    return list_release(list);
}

void list_append(struct list *list, void *item) {
    assert(list->size <= list->cap);
    if (list->size == list->cap) {
        list->cap  = (list->cap)? 2*list->cap : 32;
        list->data = realloc(list->data, list->cap * list->item_size);
        assert(list->data);
    }
    list->size++;
    if (item == NULL) {
        memset(list_at(list, list->size - 1), 0, list->item_size);
    } else {
        memcpy(list_at(list, list->size - 1), item, list->item_size);
    }
}

void list_pop(struct list *list) {
    assert(list->size > 0);
    list->size--;
}

void *list_at(struct list *list, size_t index) {
    assert(index < list->size);
    return list->data + (index * list->item_size);
}
