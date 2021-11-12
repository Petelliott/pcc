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
#ifndef PCC_SCOPE_H
#define PCC_SCOPE_H

#include <core/type.h>
#include <datastructures/arena.h>

struct scope {
    char const *name;
    struct type type;
    struct scope *parent;
};

struct scope *scope_push(struct arena *arena, struct scope *parent, char const *name, struct type const type);
struct scope *scope_find(struct scope *head, char const *name);

#endif
