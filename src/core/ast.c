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

#include "ast.h"
#include <string.h>

void ast_init(struct ast *ast) {
    ast->root = NULL;
    arena_init(&(ast->allocator), 8192);
}

void ast_free(struct ast *ast) {
    ast->root = NULL;
    arena_free(&(ast->allocator));
}

struct ast_int_lit *make_ast_int_lit(struct ast *ast, size_t number, struct type expr_type) {
    struct ast_int_lit *node = arena_allocate(&(ast->allocator), sizeof(struct ast_int_lit));
    node->info.node_type = AT_INT_LIT;
    node->info.expr_type = expr_type;
    node->info.is_lvalue = false;
    node->number = number;
    return node;
}

struct ast_float_lit *make_ast_float_lit(struct ast *ast, long double number, struct type expr_type) {
    struct ast_float_lit *node = arena_allocate(&(ast->allocator), sizeof(struct ast_float_lit));
    node->info.node_type = AT_FLOAT_LIT;
    node->info.expr_type = expr_type;
    node->info.is_lvalue = false;
    node->number = number;
    return node;
}

struct ast_str_lit *make_ast_str_lit(struct ast *ast, char const *str) {
    struct ast_str_lit *node = arena_allocate(&(ast->allocator), sizeof(struct ast_str_lit));
    node->info.node_type = AT_STR_LIT;

    node->info.expr_type.type = Numeric;
    node->info.expr_type.nindirect = 1;
    node->info.expr_type.info.scalar.size = Char;
    node->info.expr_type.info.scalar.sign = true;

    node->info.is_lvalue = false;
    node->str = arena_strdup(&(ast->allocator), str);
    return node;
}

struct ast_un_exp *make_ast_un_exp(struct ast *ast, enum operator operator, struct ast_node *child) {
    struct ast_un_exp *node = arena_allocate(&(ast->allocator), sizeof(struct ast_un_exp));
    node->info.node_type = AT_UN_EXP;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->operator = operator;
    node->child = child;
    return node;
}

struct ast_bin_exp *make_ast_bin_exp(struct ast *ast, enum operator operator, struct ast_node *lhs, struct ast_node *rhs) {
    struct ast_bin_exp *node = arena_allocate(&(ast->allocator), sizeof(struct ast_bin_exp));
    node->info.node_type = AT_UN_EXP;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->operator = operator;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

struct ast_call *make_ast_call(struct ast *ast, struct ast_node *fn, struct ast_node * const *args, size_t nargs) {
    struct ast_call *node = arena_allocate(&(ast->allocator), sizeof(struct ast_call));
    node->info.node_type = AT_CALL;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->fn = fn;
    node->nargs = nargs;
    node->args = arena_allocate(&(ast->allocator), nargs * sizeof(struct ast_node*));
    memcpy(node->args, args, nargs * sizeof(struct ast_node*));
    return node;
}

struct ast_var_ref *make_ast_var_ref(struct ast *ast, struct scope *name) {
    struct ast_var_ref *node = arena_allocate(&(ast->allocator), sizeof(struct ast_var_ref));
    node->info.node_type = AT_VAR_REF;
    node->info.expr_type = name->type;
    node->info.is_lvalue = true;
    node->name = name;
    return node;
}

struct ast_if *make_ast_if(struct ast *ast, struct ast_node *cond, struct ast_node *then, struct ast_node *els) {
    struct ast_if *node = arena_allocate(&(ast->allocator), sizeof(struct ast_if));
    node->info.node_type = AT_IF;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->cond = cond;
    node->then = then;
    node->els = els;
    return node;
}

struct ast_decl *make_ast_decl(struct ast *ast, struct scope *name) {
    struct ast_decl *node = arena_allocate(&(ast->allocator), sizeof(struct ast_decl));
    node->info.node_type = AT_FUNCTION;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->name = name;
    return node;
}

struct ast_function *make_ast_function(struct ast *ast, struct scope *name, struct ast_node *body) {
    struct ast_function *node = arena_allocate(&(ast->allocator), sizeof(struct ast_function));
    node->info.node_type = AT_FUNCTION;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->name = name;
    node->body = body;
    return node;
}

struct ast_return *make_ast_return(struct ast *ast, struct ast_node *expr) {
    struct ast_return *node = arena_allocate(&(ast->allocator), sizeof(struct ast_return));
    node->info.node_type = AT_RETURN;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;
    node->expr = expr;
    return node;
}

struct ast_block *make_ast_block(struct ast *ast, struct ast_node *const *statements, size_t length) {
    struct ast_block *node = arena_allocate(&(ast->allocator), sizeof(struct ast_block));
    node->info.node_type = AT_BLOCK;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;

    node->length = length;
    node->statements = arena_allocate(&(ast->allocator), length * sizeof(struct ast_node *));
    memcpy(node->statements, statements, length * sizeof(struct ast_node *));
    return node;
}

struct ast_prog *make_ast_prog(struct ast *ast, struct ast_node *const *decls, size_t length) {
    struct ast_prog *node = arena_allocate(&(ast->allocator), sizeof(struct ast_prog));
    node->info.node_type = AT_PROG;
    node->info.expr_type.type = Unknown;
    node->info.is_lvalue = false;

    node->length = length;
    node->decls = arena_allocate(&(ast->allocator), length * sizeof(struct ast_node *));
    memcpy(node->decls, decls, length * sizeof(struct ast_node *));
    return node;
}
