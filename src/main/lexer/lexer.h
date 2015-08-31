/**
 * @author goldolphin
 *         2015-08-19 20:44:44.
 */

#ifndef EVO_LEXER_H
#define EVO_LEXER_H

#include <utils/matcher.h>
#include "token.h"
#include "char_stream.h"

#define MAX_TOKEN_LEN 1024

typedef struct {
    matcher_t keyword_matcher;
    matcher_pattern_t * keywords;
    token_t * punctuations;
} lexer_t;

typedef void (* lexer_callback_t) (token_t * token, void * extra);

void lexer_init(lexer_t * lexer);

void lexer_destroy(lexer_t * lexer);

bool lexer_match(lexer_t * lexer, char_stream_t * stream, lexer_callback_t callback, void * extra);

int lexer_read_line(uint8_t * line, int len, FILE * file);

#endif //EVO_LEXER_H
