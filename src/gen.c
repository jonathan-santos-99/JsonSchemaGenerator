#include <stdio.h>

#include "gen.h"

inline private void
append_kv_string(Generator *generator, const char *key, const char *value)
{
    sb_append(&generator->sb, "\"");
    sb_append(&generator->sb, key);
    sb_append(&generator->sb, "\":");
    sb_append(&generator->sb, "\"");
    sb_append(&generator->sb, value);
    sb_append(&generator->sb, "\"");
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

int
generate_schema(Generator *generator)
{
    JSG_ASSERT(generator->lexer.head != NULL);
    append_obj_start(generator);

    append_kv_string(generator, "$schema", DRAFT);
    append_comma(generator);

    Token token = next_token(&generator->lexer);
    while (token.type != TOKEN_EOF) {
        // printf("(%s, `%.*s`)\n", TOKEN_NAME(token), token.offset, token.start);

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

            default: {
                fprintf(stderr, "TODO: %s not implemented\n", TOKEN_NAME(token));
                DIE();
            }
        }

        token = next_token(&generator->lexer);
    }

    append_obj_end(generator);

    return GENERATE_SUCCESS;
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
