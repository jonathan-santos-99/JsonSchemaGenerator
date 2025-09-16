#ifndef JGS_ERROR_H
#define JGS_ERROR_H

#include "common.h"
#include "jlexer.h"

enum {
    JSON_INVALID_CHARACTER = 0,
    JSON_EXPECTED_ENTRY,
    JSON_MALFORMED_STRING,
    JSON_EXPECTED_COLON,
    JSON_EXPECTED_OBJ_END,
    JSON_EXPECTED_ARR_END,
    JSON_INVALID
};

const char *error_str(Token reason);
void set_error(int errotype);
#endif // JGS_ERROR_H