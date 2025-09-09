#include <stdio.h>

#include "jlexer.h"

private bool
is_at_end(Lexer *lexer)
{
    return *lexer->head == '\0';
}

private char
next_char(Lexer *lexer)
{
    if (is_at_end(lexer)) {
        return '\0';
    }

    char c = *(lexer->head)++;
    if (c == '\n') {
        lexer->column = 1;
        lexer->line += 1;
    } else {
        lexer->column += 1;
    }

    return c;
}

private char
peek_char(Lexer *lexer)
{
    if (is_at_end(lexer)) {
        return '\0';
    }

    return *lexer->head;
}

private bool
j_isdigit(char c)
{
    return c >= '0' && c <= '9';
}

private bool
j_isspace(char c)
{
    return c == ' '
        || c == '\n'
        || c == '\r'
        || c == '\t';
}

private void
trim_left(Lexer *lexer) {
    while (!is_at_end(lexer) && j_isspace(peek_char(lexer)))
        next_char(lexer);
}

private Token
make_token(Lexer *lexer, Token_Type type)
{
    Token token = {
        .type   = type,
        .start  = lexer->head,
        .offset = 1,
        .column = lexer->column,
        .line   = lexer->line
    };

    next_char(lexer);

    return token;
}

private Token
make_token_str(Lexer *lexer)
{
    next_char(lexer); // consumes "

    Token token = {
        .line    = lexer->line,
        .column  = lexer->column,
        .type    = TOKEN_STRING,
        .start   = lexer->head,
        .offset  = 0
    };

    char c;
    for (
        c = peek_char(lexer);
        !is_at_end(lexer) && c != '\n' && c != '"';
        c = next_char(lexer)
    );

    if (c != '"') {
        fprintf(
            stderr,
            "ERROR: malformed string at %ld:%ld\n",
            lexer->line, lexer->column
        );
        return make_token(lexer, TOKEN_INVALID);
    }

    token.offset = lexer->head - token.start - 1;
    return token;
}

private char
consume_num(Lexer *lexer)
{
    char c;
    for (;;) {
        c = peek_char(lexer);
        if (is_at_end(lexer) || !j_isdigit(c)) {
            break;
        }

        next_char(lexer);
    }

    return c;
}

private Token
make_token_num(Lexer *lexer)
{
    Token token = {
        .line    = lexer->line,
        .column  = lexer->column,
        .start   = lexer->head,
        .type    = TOKEN_INT,
        .offset  = 0
    };

    char c = peek_char(lexer);
    if (c == '-' || c == '+') {
        next_char(lexer);
    }

    c = consume_num(lexer);
    if (c == '.') {
        next_char(lexer);
        c = consume_num(lexer);
        token.type = TOKEN_NUMBER;
    }

    if (c == 'e') {
        next_char(lexer);
        c = consume_num(lexer);
        token.type = TOKEN_NUMBER;
    }

    if (c == '-' || c == '+') {
        next_char(lexer);
        c = consume_num(lexer);
        token.type = TOKEN_NUMBER;
    }

    token.offset = lexer->head - token.start;
    return token;
}

private bool
next_char_match(Lexer *lexer, const char c)
{
    return next_char(lexer) == c;
}

private Token
make_boolean_true(Lexer *lexer)
{
    Token token = {
        .line    = lexer->line,
        .column  = lexer->column,
        .start   = lexer->head,
        .type    = TOKEN_BOOLEAN,
        .offset  = 0
    };

    bool matched = next_char_match(lexer, 't') &&
                   next_char_match(lexer, 'r') &&
                   next_char_match(lexer, 'u') &&
                   next_char_match(lexer, 'e');

    if (!matched) {
        fprintf(
            stderr,
            "ERROR: invalid character '%c' at position %ld:%ld\n",
            peek_char(lexer), token.line, token.column
        );

        make_token(lexer, TOKEN_INVALID);
    }

    token.offset = lexer->head - token.start;
    return token;
}

private Token
make_boolean_false(Lexer *lexer)
{
    Token token = {
        .line    = lexer->line,
        .column  = lexer->column,
        .start   = lexer->head,
        .type    = TOKEN_BOOLEAN,
        .offset  = 0
    };

    bool matched = next_char_match(lexer, 'f') &&
                   next_char_match(lexer, 'a') &&
                   next_char_match(lexer, 'l') &&
                   next_char_match(lexer, 's') &&
                   next_char_match(lexer, 'e');

    if (!matched) {
        fprintf(
            stderr,
            "ERROR: invalid character '%c' at position %ld:%ld\n",
            peek_char(lexer), token.line, token.column
        );

        make_token(lexer, TOKEN_INVALID);
    }

    token.offset = lexer->head - token.start;
    return token;
}

private Token
_next_token(Lexer *lexer)
{
    trim_left(lexer);
    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }

    char c = peek_char(lexer);
    switch (c) {
        case '{': return make_token(lexer, TOKEN_OBJ_START);
        case '}': return make_token(lexer, TOKEN_OBJ_END);
        case '[': return make_token(lexer, TOKEN_ARR_START);
        case ']': return make_token(lexer, TOKEN_ARR_END);
        case ':': return make_token(lexer, TOKEN_COLON);
        case ',': return make_token(lexer, TOKEN_COMMA);

        case 't': return make_boolean_true(lexer);
        case 'f': return make_boolean_false(lexer);
        case '"': return make_token_str(lexer);

        default: {
            if (j_isdigit(c) || c == '-' || c == '+') {
                return make_token_num(lexer);
            }

            fprintf(
                stderr,
                "ERROR: invalid character '%c' at position %ld:%ld\n",
                c, lexer->line, lexer->column
            );

            return make_token(lexer, TOKEN_INVALID);
        }
    }
}

Token
next_token(Lexer *lexer)
{
    Token token = _next_token(lexer);
    // printf("(`%s`, %.*s)\n", TOKEN_NAME(token), token.offset, token.start);
    return token;
}

void
lexer_denit(Lexer *lexer)
{
    if (lexer->content) {
        free(lexer->content);
    }
}

void
lexer_init(Lexer *lexer, const char *json)
{
    lexer->head = json;
    lexer->content = (char *) json;
    lexer->column = lexer->line = 1;
}