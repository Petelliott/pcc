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
#ifndef PCC_POLYMORPHIC_H
#define PCC_POLYMORPHIC_H

#include <stddef.h>
#include <datastructures/hashmap.h>

#define poly_info size_t type
#define poly_struct(name)                                \
    __attribute__((unused))                              \
    static const size_t _##name##_polykey = __LINE__;    \
    struct name                                          \

struct polymorphic {
    poly_info;
};

#define poly_isa(typ, ptr) ((ptr)->type == _##typ##_polykey)
#define poly_to(typ, ptr) (assert(polymorphic_isa(typ, ptr)), ((polymorphic_type(typ) *) ptr)->value)
#define poly(obj) (assert((obj)->type), (struct polymorphic *)(obj))

#define poly_decl(ret, name) \
    extern struct hashmap _poly_table_##name;   \
    typedef ret (*_poly_call_type_##name)

#define poly_def(name)                                  \
    struct hashmap _poly_table_##name;                  \
    __attribute__((constructor))                        \
    static void _poly_def_constructor_##name(void) {    \
        poly_init_fn(&_poly_table_##name);              \
    }

#define poly_static_fun(name, typ) _poly_variant_##typ##_##name
#define poly_variant(ret, name, typ)                                    \
    static ret poly_static_fun(name, typ)();                            \
    __attribute__((constructor))                                        \
    static void _poly_constructor_##name##_##typ(void) {                \
        poly_register_variant(&_poly_table_##name, _##typ##_polykey, poly_static_fun(name, typ)); \
    }                                                                   \
    static ret poly_static_fun(name, typ)

#define poly_call(name, obj)             \
    ((_poly_call_type_##name) poly_get_variant(&_poly_table_##name, (struct polymorphic *)(obj)))

void poly_init_fn(struct hashmap *poly_map);
void poly_register_variant(struct hashmap *poly_map, size_t key, void *fnptr);
void const*poly_get_variant(struct hashmap *poly_map, struct polymorphic *obj);

#endif
