/**
 * @author goldolphin
 *         2015-09-01 15:37:37.
 */

#include "token_stream.h"

void token_stream_init(token_stream_t * stream, lexer_t * lexer, char_stream_t * char_stream) {
    stream->lexer = lexer;
    lexer_reset_context(lexer, &stream->lc);
    stream->cs = char_stream;
    stream->token = NULL;
}

token_t * token_stream_peek(token_stream_t * stream) {
    if (stream->token == NULL) {
        stream->token = lexer_poll(stream->lexer, &stream->lc, stream->cs);
        if (stream->token == NULL) {
            return NULL;
        }
    }
    return stream->token;
}

token_t * token_stream_poll(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token == NULL || token == TOKEN_END) {
        return token;
    }
    stream->token = NULL;
    return token;
}
