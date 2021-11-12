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
#ifndef PCC_AST_H
#define PCC_AST_H

#include <core/type.h>
#include <core/scope.h>
#include <datastructures/arena.h>
#include <stdbool.h>

enum ast_type {
    AT_INT_LIT,
    AT_FLOAT_LIT,
    AT_STR_LIT,
    AT_UN_EXP,
    AT_BIN_EXP,
    AT_CALL,
    AT_VAR_REF,
    AT_IF,
    AT_WHILE,
    AT_DO,
    AT_FOR,
    AT_DECL,
    AT_FUNCTION,
    AT_RETURN,
    AT_BLOCK,
    AT_PROG,
};

enum operator {
    OP_PLUS,
    OP_MINUS,
    OP_ARROW,
    OP_STAR,
    OP_SLASH,
    OP_PERCENT,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_SHL,
    OP_SHR,
    OP_EQUAL,
    OP_PLUS_EQUAL,
    OP_MINUS_EQUAL,
    OP_STAR_EQUAL,
    OP_SLASH_EQUAL,
    OP_PERCENT_EQUAL,
    OP_AND_EQUAL,
    OP_OR_EQUAL,
    OP_XOR_EQUAL,
    OP_SHL_EQUAL,
    OP_SHR_EQUAL,
    OP_EQUAL_EQUAL,
    OP_TILDE,
    OP_EXCLAM,
    OP_EXCLAM_EQUAL,
    OP_AND_AND,
    OP_OR_OR,
    OP_PLUS_PLUS,
    OP_MINUS_MINUS,
    OP_LT,
    OP_LT_EQUAL,
    OP_GT,
    OP_GT_EQUAL,
};

struct ast {
    struct arena allocator;
    struct ast_node *root;
};

void ast_init(struct ast *ast);
void ast_free(struct ast *ast);

struct ast_node {
    enum ast_type node_type;
    struct type expr_type;
    bool is_lvalue;
};

struct ast_int_lit {
    struct ast_node info;
    size_t number;
};
struct ast_int_lit *make_ast_int_lit(struct ast *ast, size_t number, struct type expr_type);

struct ast_float_lit {
    struct ast_node info;
    long double number;
};
struct ast_float_lit *make_ast_float_lit(struct ast *ast, long double number, struct type expr_type);

struct ast_str_lit {
    struct ast_node info;
    char *str;
};
struct ast_str_lit *make_ast_str_lit(struct ast *ast, char const *str);

struct ast_un_exp {
    struct ast_node info;
    enum operator operator;
    struct ast_node *child;
};
struct ast_un_exp *make_ast_un_exp(struct ast *ast, enum operator operator, struct ast_node *child);

struct ast_bin_exp {
    struct ast_node info;
    enum operator operator;
    struct ast_node *lhs;
    struct ast_node *rhs;
};
struct ast_bin_exp *make_ast_bin_exp(struct ast *ast, enum operator operator, struct ast_node *lhs, struct ast_node *rhs);

struct ast_call {
    struct ast_node info;
    struct ast_node *fn;
    size_t nargs;
    struct ast_node **args;
};
struct ast_call *make_ast_call(struct ast *ast, struct ast_node *fn, struct ast_node *const *args, size_t nargs);

struct ast_var_ref {
    struct ast_node info;
    struct scope *name;
};
struct ast_var_ref *make_ast_var_ref(struct ast *ast, struct scope *name);

struct ast_if {
    struct ast_node info;
    struct ast_node *cond;
    struct ast_node *then;
    struct ast_node *els;
};
struct ast_if *make_ast_if(struct ast *ast, struct ast_node *cond, struct ast_node *then, struct ast_node *els);

struct ast_decl {
    struct ast_node info;
    struct scope *name;
};
struct ast_decl *make_ast_decl(struct ast *ast, struct scope *name);

struct ast_function {
    struct ast_node info;
    struct scope *name;
    struct ast_node **args;
    struct ast_node *body;
};
struct ast_function *make_ast_function(struct ast *ast, struct scope *name, struct ast_node *body);

struct ast_return {
    struct ast_node info;
    struct ast_node *expr;
};
struct ast_return *make_ast_return(struct ast *ast, struct ast_node *expr);

struct ast_block {
    struct ast_node info;
    struct ast_node **statements;
    size_t length;
};
struct ast_block *make_ast_block(struct ast *ast, struct ast_node *const *statements, size_t length);

struct ast_prog {
    struct ast_node info;
    struct ast_node **decls;
    size_t length;
};
struct ast_prog *make_ast_prog(struct ast *ast, struct ast_node *const *decls, size_t length);



#endif
