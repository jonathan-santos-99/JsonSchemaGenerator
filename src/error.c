#include <stdio.h>

#include "error.h"

int error = -1;

void
set_error(int errortype)
{
    error = errortype;
}

const char *
error_str(Token reason)
{
    static char buffer[256];
    switch (error) {
        case JSON_INVALID_CHARACTER: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: invalid character at position %ld:%ld\n",
                reason.line, reason.column
            );
        } break;

        case JSON_MALFORMED_STRING: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: malformed string at %ld:%ld\n",
                reason.line, reason.column
            );
        } break;

        case JSON_EXPECTED_ENTRY: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: expected entry key at position %ld:%ld. Actual: `%s`\n",
                reason.line, reason.column,
                TOKEN_NAME(reason)
            );
        } break;

        case JSON_EXPECTED_COLON: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: expected colon at position %ld:%ld\n",
                reason.line, reason.column
            );
        } break;

        case JSON_EXPECTED_OBJ_END: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: expected end of object at position %ld:%ld. Actual: `%s`\n",
                reason.line, reason.column,
                TOKEN_NAME(reason)
            );
        } break;

        case JSON_EXPECTED_ARR_END: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: expected end of array at position %ld:%ld. Actual: `%s`\n",
                reason.line, reason.column,
                TOKEN_NAME(reason)
            );
        } break;

        case JSON_INVALID: {
            snprintf(buffer, ARRAY_SIZE(buffer), "ERROR: not a valid JSON\n");
        } break;

        default: {
            snprintf(
                buffer, ARRAY_SIZE(buffer),
                "ERROR: invalid json. Reason at %ld:%ld \n",
                reason.line, reason.column
            );
        }
    }

    return buffer;
}