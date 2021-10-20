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
#ifndef PCC_TOKEN_H
#define PCC_TOKEN_H

#include <datastructures/list.h>
#include <stdio.h>

enum token_type {
    // reserved words
    TOK_AUTO,
    TOK_CONST,
    TOK_DOUBLE,
    TOK_FLOAT,
    TOK_INT,
    TOK_SHORT,
    TOK_STRUCT,
    TOK_UNSIGNED,
    TOK_BREAK,
    TOK_CONTINUE,
    TOK_ELSE,
    TOK_FOR,
    TOK_LONG,
    TOK_SIGNED,
    TOK_SWITCH,
    TOK_VOID,
    TOK_CASE,
    TOK_DEFAULT,
    TOK_ENUM,
    TOK_GOTO,
    TOK_REGISTER,
    TOK_SIZEOF,
    TOK_TYPEDEF,
    TOK_VOLATILE,
    TOK_CHAR,
    TOK_DO,
    TOK_EXTERN,
    TOK_IF,
    TOK_RETURN,
    TOK_STATIC,
    TOK_UNION,
    TOK_WHILE,
    TOK_RESTRICT,

    // symbols
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRAK,
    TOK_RBRAK,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_COMMA,
    TOK_SEMICOLON,

    // operators
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_PERCENT,
    TOK_AND,
    TOK_OR,
    TOK_XOR,
    TOK_SHL,
    TOK_SHR,
    TOK_EQUAL,
    TOK_PLUS_EQUAL,
    TOK_MINUS_EQUAL,
    TOK_STAR_EQUAL,
    TOK_SLASH_EQUAL,
    TOK_PERCENT_EQUAL,
    TOK_AND_EQUAL,
    TOK_OR_EQUAL,
    TOK_XOR_EQUAL,
    TOK_SHL_EQUAL,
    TOK_SHR_EQUAL,
    TOK_EQUAL_EQUAL,
    TOK_TILDE,
    TOK_EXCLAM,
    TOK_EXCLAM_EQUAL,
    TOK_AND_AND,
    TOK_OR_OR,
    TOK_PLUS_PLUS,
    TOK_MINUS_MINUS,
    TOK_LT,
    TOK_LT_EQUAL,
    TOK_GT,
    TOK_GT_EQUAL,

    // Special
    TOK_EOF,
    TOK_STRING,
    TOK_CHAR_LIT,
    TOK_NUM,
    TOK_IDENT,
    TOK_INVALID,
};

struct token {
    enum token_type type;
    char *text;
};

struct token_stream {
    struct list tokens;
    size_t index;
    FILE *stream;
};

void token_stream_init(struct token_stream *token_stream, FILE *stream);
void token_stream_free(struct token_stream *stream);

struct token const *token_stream_next(struct token_stream *stream);
void token_stream_seek(struct token_stream *stream, int n);
struct token const *token_stream_current(struct token_stream const *stream);

#endif
