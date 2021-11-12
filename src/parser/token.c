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
#include "token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


void token_stream_init(struct token_stream *token_stream, FILE *stream) {
    list_init(&token_stream->tokens, sizeof(struct token));
    token_stream->index = 0;
    token_stream->stream = stream;
}

void token_stream_free(struct token_stream *stream) {
    list_foreach(struct token *, tok, stream->tokens) {
        free(tok->text);
    }
    list_free(&stream->tokens);
}

static struct token get_token_for_ident(char *text) {
#define match_kw(str, tok) if (strcmp(str, text) == 0) { return (struct token) { tok, text }; }

    match_kw("auto", TOK_AUTO)
    match_kw("const", TOK_CONST)
    match_kw("double", TOK_DOUBLE)
    match_kw("float", TOK_FLOAT)
    match_kw("int", TOK_INT)
    match_kw("short", TOK_SHORT)
    match_kw("struct", TOK_STRUCT)
    match_kw("unsigned", TOK_UNSIGNED)
    match_kw("break", TOK_BREAK)
    match_kw("continue", TOK_CONTINUE)
    match_kw("else", TOK_ELSE)
    match_kw("for", TOK_FOR)
    match_kw("long", TOK_LONG)
    match_kw("signed", TOK_SIGNED)
    match_kw("switch", TOK_SWITCH)
    match_kw("void", TOK_VOID)
    match_kw("case", TOK_CASE)
    match_kw("default", TOK_DEFAULT)
    match_kw("enum", TOK_ENUM)
    match_kw("goto", TOK_GOTO)
    match_kw("register", TOK_REGISTER)
    match_kw("sizeof", TOK_SIZEOF)
    match_kw("typedef", TOK_TYPEDEF)
    match_kw("volatile", TOK_VOLATILE)
    match_kw("char", TOK_CHAR)
    match_kw("do", TOK_DO)
    match_kw("extern", TOK_EXTERN)
    match_kw("if", TOK_IF)
    match_kw("return", TOK_RETURN)
    match_kw("static", TOK_STATIC)
    match_kw("union", TOK_UNION)
    match_kw("while", TOK_WHILE)
    match_kw("restrict", TOK_RESTRICT)

    return (struct token) { TOK_IDENT, text };
}

static struct token get_2char_token(struct list *buffer, FILE *stream) {
#define match_tok(str, tok)                                         \
    if (str[0] == ((char*)buffer->data)[0]) {                       \
        if (str[1] == '\0') {                                       \
            ungetc(((char*)buffer->data)[1], stream);               \
            list_pop(buffer);                                       \
            return (struct token) { tok, list_release_nt(buffer) }; \
        } else if (str[1] == ((char*)buffer->data)[1]) {            \
            return (struct token) { tok, list_release_nt(buffer) }; \
        }                                                           \
    }

    // all tokens must be matched before their substrings.

    match_tok("(", TOK_LPAREN)
    match_tok(")", TOK_RPAREN)
    match_tok("[", TOK_LBRAK)
    match_tok("]", TOK_RBRAK)
    match_tok("{", TOK_LBRACE)
    match_tok("}", TOK_RBRACE)
    match_tok(",", TOK_COMMA)
    match_tok(";", TOK_SEMICOLON)
    match_tok("++", TOK_PLUS_PLUS)
    match_tok("+=", TOK_PLUS_EQUAL)
    match_tok("+", TOK_PLUS)
    match_tok("--", TOK_MINUS_MINUS)
    match_tok("-=", TOK_MINUS_EQUAL)
    match_tok("->", TOK_ARROW)
    match_tok("-", TOK_MINUS)
    match_tok("*=", TOK_STAR_EQUAL)
    match_tok("*", TOK_STAR)
    match_tok("/=", TOK_SLASH_EQUAL)
    match_tok("/", TOK_SLASH)
    match_tok("%=", TOK_PERCENT_EQUAL)
    match_tok("%", TOK_PERCENT)
    match_tok("&=", TOK_AND_EQUAL)
    match_tok("&&", TOK_AND_AND)
    match_tok("&", TOK_AND)
    match_tok("|=", TOK_OR_EQUAL)
    match_tok("||", TOK_OR_OR)
    match_tok("|", TOK_OR)
    match_tok("^=", TOK_XOR_EQUAL)
    match_tok("^", TOK_XOR)
    match_tok("==", TOK_EQUAL_EQUAL)
    match_tok("=", TOK_EQUAL)
    match_tok("~", TOK_TILDE)
    match_tok("!=", TOK_EXCLAM_EQUAL)
    match_tok("!", TOK_EXCLAM)
    match_tok("<<", TOK_SHL)
    match_tok("<=", TOK_LT_EQUAL)
    match_tok("<", TOK_LT)
    match_tok(">>", TOK_SHR)
    match_tok(">=", TOK_GT_EQUAL)
    match_tok(">", TOK_GT)

    // TODO: fuck these lol
    //match_tok(">>=", TOK_SHL_EQUAL)
    //match_tok("<<=", TOK_SHR_EQUAL)

    return (struct token) { TOK_INVALID, list_release_nt(buffer) };
}

static struct token read_token(FILE *stream) {
    struct list token_buffer;
    list_init(&token_buffer, sizeof(char));

    char ch;
    while ((ch = fgetc(stream)) != EOF) {
        if (!isspace(ch))
            break;
    }

    if (ch == EOF)
        return (struct token) { TOK_EOF, NULL };

    list_append(&token_buffer, &ch);
    if (ch == '"') {
        for (;;) {
            ch = fgetc(stream);
            list_append(&token_buffer, &ch);
            if (ch == '"' || ch == EOF)
                return (struct token) { TOK_STRING, list_release_nt(&token_buffer) };
        }
        abort();
    } else if (ch == '\'') {
        for (;;) {
            ch = fgetc(stream);
            list_append(&token_buffer, &ch);
            if (ch == '\'' || ch == EOF)
                return (struct token) { TOK_CHAR_LIT, list_release_nt(&token_buffer) };
        }
    } else if (isalpha(ch)) {
        for (;;) {
            ch = fgetc(stream);
            if ((!isalnum(ch) && ch != '_') || ch == EOF) {
                ungetc(ch, stream);
                return get_token_for_ident(list_release_nt(&token_buffer));
            }
            list_append(&token_buffer, &ch);
        }
    } else if (isdigit(ch)) {
        for (;;) {
            ch = fgetc(stream);
            if ((!isdigit(ch) && ch != '.') || ch == EOF) {
                ungetc(ch, stream);
                return (struct token) { TOK_NUM, list_release_nt(&token_buffer) };
            }
            list_append(&token_buffer, &ch);
        }
    }

    ch = fgetc(stream);
    list_append(&token_buffer, &ch);
    return get_2char_token(&token_buffer, stream);
}

struct token const *token_stream_next(struct token_stream *stream) {
    if (stream->index == stream->tokens.size) {
        if (stream->tokens.size > 0) {
            struct token *tail = list_at(&stream->tokens, stream->tokens.size - 1);
            if (tail->type == TOK_EOF)
                return tail;
        }

        list_appendv(&stream->tokens, read_token(stream->stream));
    }

    return list_at(&stream->tokens, stream->index++);
}

void token_stream_seek(struct token_stream *stream, int n) {
    stream->index = stream->index + n;
    assert(stream->index <= stream->tokens.size);
}

void token_stream_set_index(struct token_stream *stream, size_t index) {
    assert(index <= stream->tokens.size);
    stream->index = index;
}

struct token const *token_stream_peek(struct token_stream *stream) {
    struct token const* tok = token_stream_next(stream);
    stream->index--;
    return tok;
}
