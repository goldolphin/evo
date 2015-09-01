/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include <lexer/token.h>
#include "parser.h"
#include "token_stream.h"

void * parse_module_id(token_stream_t * stream) {

}

void * parse_import(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_IMPORT) {
        return NULL;
    }
    token_stream_poll(stream);

}

void * parse_statement(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token == TOKEN_END) {
        return NULL;
    }

    switch (token->type) {
        case TOKEN_IMPORT:
            break;
    }
}

void parser_parse(token_stream_t *stream) {
}