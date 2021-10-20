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
#include <stdio.h>
#include <parser/token.h>

int main() {
    struct token_stream stream;
    token_stream_init(&stream, stdin);

    struct token const *tok;
    for (;;) {
        tok = token_stream_next(&stream);
        assert(tok->type != TOK_INVALID);
        if (tok->type == TOK_EOF)
            break;

        printf("%u, '%s'\n", tok->type, tok->text);
    }
    return 0;
}
