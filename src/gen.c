#include <stdio.h>

#include "gen.h"

private int append_json_type(Generator *, Token);

inline private void
append_str(Generator *generator, const char *str)
{
    sb_append(&generator->sb, "\"");
    sb_append(&generator->sb, str);
    sb_append(&generator->sb, "\"");
}

inline private void
append_str_len(Generator *generator, const char *str, size_t slen)
{
    sb_append(&generator->sb, "\"");
    sb_append_len(&generator->sb, str, slen);
    sb_append(&generator->sb, "\"");
}

inline private void
append_colon(Generator *generator)
{
    sb_append(&generator->sb, ":");
}

inline private void
append_kv_string(Generator *generator, const char *key, const char *value)
{
    append_str(generator, key);
    append_colon(generator);
    append_str(generator, value);
}

inline private void
append_comma(Generator *generator)
{
    sb_append(&generator->sb, ",");
}

inline private void
append_obj_start(Generator *generator)
{
    sb_append(&generator->sb, "{");
}

inline private void
append_obj_end(Generator *generator)
{
    sb_append(&generator->sb, "}");
}

private bool
next_token_match(Generator *generator, Token_Type type)
{
    Token token = next_token(&generator->lexer);
    if (token.type != type) {
        fprintf(
            stderr,
            "ERROR: expected token %s at position %ld:%ld. Actual: %s",
            token_names[type], token.line, token.column,
            TOKEN_NAME(token)
        );

        return false;
    }

    return true;
}

private int
generate_obj(Generator *generator)
{
    append_str(generator, "properties");
    append_colon(generator);
    append_obj_start(generator);

    Token token = next_token(&generator->lexer);
    bool first = true;
    for (;;) {
        if (!first) {
            append_comma(generator);
        }

        if (token.type != TOKEN_STRING) {
            fprintf(
                stderr,
                "ERROR: expected entry key at position %ld:%ld. Actual: %s\n",
                token.line, token.column,
                TOKEN_NAME(token)
            );

            return GENERATE_INVALID_JSON;
        }

        append_str_len(generator, token.start, token.offset);
        append_colon(generator);
        append_obj_start(generator);
        if (!next_token_match(generator, TOKEN_COLON)) {
            return GENERATE_INVALID_JSON;
        }

        token = next_token(&generator->lexer);
        if (append_json_type(generator, token) != GENERATE_SUCCESS) {
            return GENERATE_INVALID_JSON;
        }

        append_obj_end(generator);
        token = next_token(&generator->lexer);
        if (token.type == TOKEN_COMMA) {
            token = next_token(&generator->lexer);
            first = false;
        } else {
            break;
        }
    }

    if (token.type != TOKEN_OBJ_END) {
        fprintf(
            stderr,
            "ERROR: expected end of object at position %ld:%ld. Actual: %s",
            token.line, token.column,
            TOKEN_NAME(token)
        );

        return GENERATE_INVALID_JSON;
    }

    append_obj_end(generator);
    return GENERATE_SUCCESS;
}

private int
append_json_type(Generator *generator, Token token)
{
    switch (token.type) {
        case TOKEN_STRING: {
            append_kv_string(generator, "type", "string");
        } break;

        case TOKEN_INT: {
            append_kv_string(generator, "type", "integer");
        } break;

        case TOKEN_NUMBER: {
            append_kv_string(generator, "type", "number");
        } break;

        case TOKEN_OBJ_START: {
            append_kv_string(generator, "type", "object");
            append_comma(generator);
            return generate_obj(generator);
        } break;

        case TOKEN_ARR_START: {
            append_kv_string(generator, "type", "array");
        } break;

        default: {
            fprintf(stderr, "ERROR: malformed json at position %ld:%ld", token.line, token.column);
            return GENERATE_INVALID_JSON;
        }
    }

    return GENERATE_SUCCESS;
}

int
generate_schema(Generator *generator)
{
    JSG_ASSERT(generator->lexer.head != NULL);
    append_obj_start(generator);

    append_kv_string(generator, "$schema", DRAFT);
    append_comma(generator);

    Token token = next_token(&generator->lexer);
    int result = append_json_type(generator, token);

    append_obj_end(generator);

    return result;
}

void
generator_init(Generator *generator, const char *json)
{
    lexer_init(&generator->lexer, json);
    generator->sb.capacity = 0;
    generator->sb.items = NULL;
    generator->sb.count = 0;
}

void
generator_denit(Generator *generator)
{
    sb_denit(&generator->sb);
}
