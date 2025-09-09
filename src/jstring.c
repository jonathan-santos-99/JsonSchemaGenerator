#include <stdlib.h>
#include <string.h>

#include "jstring.h"

#define SB_INITIAL_CAP 1024*1024

void
sb_append(String_Builder *sb, const char *s)
{
    if (s == NULL) {
        JSG_ASSERT(0 && "append 'null'");
    }

    if (sb->items == NULL || sb->capacity == 0) {
        sb->capacity = SB_INITIAL_CAP;
        sb->count = 0;
        sb->items = malloc(sizeof(char)*SB_INITIAL_CAP);
        JSG_ASSERT(sb->items != NULL);
    }

    size_t slen = strlen(s);
    if (sb->capacity <= sb->count + slen) {
        sb->capacity = MAX(slen, sb->capacity*2);
        sb->items = realloc(sb->items, sb->capacity);
    }

    strcpy(sb->items + sb->count, s);
    sb->count += slen;
}

void
sb_append_len(String_Builder *sb, const char *s, size_t slen)
{
    if (s == NULL) {
        JSG_ASSERT(0 && "append 'null'");
    }

    if (sb->items == NULL || sb->capacity == 0) {
        sb->capacity = SB_INITIAL_CAP;
        sb->count = 0;
        sb->items = malloc(sizeof(char)*SB_INITIAL_CAP);
        JSG_ASSERT(sb->items != NULL);
    }

    if (sb->capacity <= sb->count + slen) {
        sb->capacity = MAX(slen, sb->capacity*2);
        sb->items = realloc(sb->items, sb->capacity);
    }

    strncpy(sb->items + sb->count, s, slen);
    sb->count += slen;
}

void
sb_denit(String_Builder *sb)
{
    if (sb->items) free(sb->items);
}