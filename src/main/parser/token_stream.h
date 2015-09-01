/**
 * @author goldolphin
 *         2015-09-01 15:37:37.
 */

#ifndef EVO_TOKEN_STREAM_H
#define EVO_TOKEN_STREAM_H

#include <lexer/lexer.h>

typedef struct {
    lexer_t * lexer;
    lexer_context_t lc;
    char_stream_t * cs;
    token_t * token;
} token_stream_t;

void token_stream_init(token_stream_t * stream, lexer_t * lexer, char_stream_t * char_stream);

token_t * token_stream_peek(token_stream_t * stream);

token_t * token_stream_poll(token_stream_t * stream);

#endif //EVO_TOKEN_STREAM_H
