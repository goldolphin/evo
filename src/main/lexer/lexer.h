/**
 * @author goldolphin
 *         2015-08-19 20:44:44.
 */

#ifndef EVO_LEXER_H
#define EVO_LEXER_H

#include <utils/matcher.h>
#include "token.h"
#include "char_stream.h"
#include "source_info.h"

#define MAX_TOKEN_LEN 1024

typedef struct {
    matcher_t keyword_matcher;
    matcher_pattern_t * keywords;
    token_t * punctuations;
} lexer_t;

typedef struct {
    uint8_t buf[MAX_TOKEN_LEN];
    int buf_len;
    token_t token;
    source_info_t source_info;
} lexer_context_t ;

typedef void (* lexer_callback_t) (token_t * token, void * extra);

void lexer_init(lexer_t * lexer);

void lexer_destroy(lexer_t * lexer);

void lexer_init_context(lexer_t * lexer, lexer_context_t * context);

// Returns NULL on error, retValue->type == TOKEN_END on EOF, and valid tokens otherwise.
token_t * lexer_poll(lexer_t * lexer, lexer_context_t * context, char_stream_t * stream);

#endif //EVO_LEXER_H
