#ifndef JSGEN_LEXER_H
#define JSGEN_LEXER_H

#include "common.h"

typedef struct {
    const char *head;
    size_t line;
    size_t column;
} Lexer;

typedef enum {
    TOKEN_OBJ_START = 0,
    TOKEN_OBJ_END      ,
    TOKEN_ARR_START    ,
    TOKEN_ARR_END      ,
    TOKEN_INT          ,
    TOKEN_NUMBER       ,
    TOKEN_STRING       ,
    TOKEN_EOF          ,
    TOKEN_COLON        ,
    TOKEN_COMMA        ,

    // keep this last
    __TOKEN_TYPE_CNT
} Token_Type;

static const char *token_names[] = {
    [TOKEN_OBJ_START]  = "OBJ_START",
    [TOKEN_OBJ_END]    = "OBJ_END",
    [TOKEN_ARR_START]  = "ARR_START",
    [TOKEN_ARR_END]    = "ARR_END",
    [TOKEN_INT]        = "INT",
    [TOKEN_NUMBER]     = "NUMBER",
    [TOKEN_STRING]     = "STRING",
    [TOKEN_EOF]        = "EOF",
    [TOKEN_COLON]      = "COLON",
    [TOKEN_COMMA]      = "COMMA",
};

static_assert( ARRAY_SIZE(token_names) == __TOKEN_TYPE_CNT );

#define TOKEN_NAME(token) token_names[(token.type)]

typedef struct {
    Token_Type type;
    const char *start;
    int offset;
    size_t line;
    size_t column;
} Token;

Token next_token(Lexer *lexer);
void  lexer_init(Lexer *lexer, const char *json);

#endif // JSGEN_LEXER_H