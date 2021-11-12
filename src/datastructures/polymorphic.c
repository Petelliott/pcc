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

#include "polymorphic.h"
#include <stdlib.h>

void poly_init_fn(struct hashmap *poly_map) {
    hashmap_init(poly_map, ptr_key_hash, ptr_key_cmp, free);
}

void poly_register_variant(struct hashmap *poly_map, size_t key, void *fnptr) {
    hashmap_insert(poly_map, (void *) key, fnptr);
}

void const*poly_get_variant(struct hashmap *poly_map, struct polymorphic *obj) {
    return hashmap_get(poly_map, (void *)obj->type);
}
