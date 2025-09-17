#ifndef JSGEN_STR_H
#define JSGEN_STR_H

#include "common.h"

typedef struct {
    char *items;
    size_t capacity;
    int count;
} String_Builder;

void sb_append(String_Builder *sb, const char *s);
void sb_append_len(String_Builder *sb, const char *s, size_t slen);
void sb_denit(String_Builder *sb);
#endif // JSGEN_STR_H