#ifndef JSGEN_GEN_H
#define JSGEN_GEN_H

#include "common.h"
#include "jlexer.h"
#include "jstring.h"

#define DRAFT "http://json-schema.org/draft-04/schema#"

typedef struct {
    Lexer lexer;
    String_Builder sb;
} Generator;

void generator_init (Generator *generator, const char *json);
bool generate_schema(Generator *generator);
void generator_denit(Generator *generator);

#endif // JSGEN_LEXER_H