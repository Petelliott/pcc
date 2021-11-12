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

#include "parser.h"
#include <datastructures/list.h>

#define option1(v1) (v1)
#define option2(v1, v2) ({                       \
            typeof(v1) _value = (v1);            \
            if (!_value) _value = (v2);          \
            _value;                              \
        })
#define option3(v1, v2, v3) option2(option2(v1, v2), v3)
#define option4(v1, v2, v3, v4) option2(option3(v1, v2, v3), v4)
#define option5(v1, v2, v3, v4, v5) option2(option4(v1, v2, v3, v4), v5)
#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
#define option(...) GET_MACRO(__VA_ARGS__, option5, option4, option3, option2, option1)(__VA_ARGS__)

#define save() size_t _save = stream->index
#define parse_fail() { token_stream_set_index(stream, _save); return NULL; }
#define expect(tok) ({                                              \
            struct token const *_tok = token_stream_next(stream);   \
            if (_tok->type != (tok)) parse_fail();                  \
            _tok;                                                   \
        })
#define consume(tok) ({                                             \
            struct token const *_tok = token_stream_next(stream);   \
            if (_tok->type != (tok)) {                              \
                token_stream_seek(stream, -1);                      \
                _tok = NULL;                                        \
            }                                                       \
            _tok;                                                   \
        })

static struct ast_node *parse_function(struct ast *ast, struct token_stream *stream, struct scope **scope) {
    save();
    expect(TOK_VOID);
    struct token const *name = expect(TOK_IDENT);
    *scope = scope_push(&(ast->allocator), *scope, name->text, void_type);
    expect(TOK_LPAREN);
    struct list args;
    list_init(&args, sizeof(struct ast_node *));
    struct scope *fnscope = *scope;
    do {
        struct ast_node *arg = (void*)parse_decl(ast, stream, &fnscope);
        list_append(&args, &arg);
    } while (consume(TOK_COMMA));
    // this is definetly a memory leak lol.
    expect(TOK_RPAREN);
    struct ast_node *block = parse_block(ast, stream, scope);
    return (void*) make_ast_function(
}

static struct ast_node *parse_decl(struct ast *ast, struct token_stream *stream, struct scope **scope) {
    save();
    expect(TOK_VOID);
    struct token const *tok = expect(TOK_IDENT);
    *scope = scope_push(&(ast->allocator), *scope, tok->text, void_type);
    expect(TOK_SEMICOLON);
    return (void*)make_ast_decl(ast, *scope);
}

static struct ast_node *parse_toplevel_stmt(struct ast *ast, struct token_stream *stream, struct scope **scope) {
    return option(
        parse_decl(ast, stream, scope)
        //parse_function(ast, stream)
    );
}

static struct ast_node *parse_prog(struct ast *ast, struct token_stream *stream, struct scope **scope) {
    struct list stmts;
    list_init(&stmts, sizeof(struct ast_node *));

    while (token_stream_peek(stream)->type != EOF) {
        struct ast_node *stmt = parse_toplevel_stmt(ast, stream, scope);
        list_append(&stmts, &stmt);
    }

    struct ast_node *prog = (void*)make_ast_prog(ast, stmts.data, stmts.size);
    list_free(&stmts);
    return prog;
}

struct ast parse(struct token_stream *stream) {
    struct ast ast;
    ast_init(&ast);
    struct scope *scope = NULL;
    ast.root = parse_prog(&ast, stream, &scope);
    return ast;
}
