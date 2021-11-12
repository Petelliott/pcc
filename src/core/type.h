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
#ifndef PCC_TYPE_H
#define PCC_TYPE_H

#include <stddef.h>

enum numeric_type {
    Char,
    Short,
    Int,
    Long,
    LongLong,
    Float,
    Double,
    LongDouble,
};

enum sign {
    DefaultSign,
    Signed,
    Unsigned,
};

enum type_type {
    Unknown,
    Void,
    Numeric,
    Enum,
    Struct,
    Union,
};

struct type {
    enum type_type type;
    size_t nindirect;
    union {
        struct {
            enum numeric_type size;
            enum sign sign;
        } scalar;
        // TODO: complex types
    } info;
};

extern struct type void_type;

#endif
