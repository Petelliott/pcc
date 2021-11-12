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

#include "scope.h"
#include <string.h>

struct scope *scope_push(struct arena *arena, struct scope *parent, char const *name, struct type type) {
    struct scope *new_scope = arena_allocate(arena, sizeof(struct scope *));
    new_scope->name = name;
    new_scope->type = type;
    new_scope->parent = parent;
    return new_scope;

}

struct scope *scope_find(struct scope *head, char const *name) {
    if (head == NULL || strcmp(head->name, name) == 0)
        return head;

    return scope_find(head->parent, name);
}
