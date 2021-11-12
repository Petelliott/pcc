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


#include "../test.h"
#include <datastructures/polymorphic.h>
#include <stdlib.h>

poly_struct(plus) {
    poly_info;
    struct polymorphic *lhs;
    struct polymorphic *rhs;
};

poly_struct(times) {
    poly_info;
    struct polymorphic *lhs;
    struct polymorphic *rhs;
};

poly_struct(integer) {
    poly_info;
    int value;
};

poly_decl(int, eval)(struct polymorphic *obj);
poly_def(eval);

poly_variant(int, eval, plus)(struct plus *node) {
    return poly_call(eval, node->lhs)(node->lhs) + poly_call(eval, node->rhs)(node->rhs);
}

poly_variant(int, eval, times)(struct times *node) {
    return poly_call(eval, node->lhs)(node->lhs) * poly_call(eval, node->rhs)(node->rhs);
}

poly_variant(int, eval, integer)(struct integer *node) {
    return node->value;
}

/*
struct polymorphic *plus(struct polymorphic *a, struct polymorphic *b) {
    struct plus *ptr = malloc(sizeof(struct plus));
    ptr->lhs = a;
    ptr->rhs = b;
    return poly(ptr);
}
*/

testcase() {

}
