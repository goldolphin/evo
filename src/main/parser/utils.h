/**
 * @author goldolphin
 *         2015-09-08 16:26:26.
 */

#ifndef EVO_UTILS_H
#define EVO_UTILS_H

#include <stdbool.h>
#include <lexer/source_info.h>
#include "token_stream.h"

static inline void require(bool condition, const char * message, token_stream_t * stream) {
    if (!condition) {
        source_info_t *source_info = token_stream_source_info(stream);
        fprintf(stderr, "[PARSER ERROR] %s\n\tat [%d, %d] %s~~~\n", message, source_info->line, source_info->column, source_info->buffer.buf);
        ensure(false);
    }
}

static inline void require_token(token_t * token, token_type_t type, token_stream_t * stream) {
    if (token == NULL || token->type != type) {
        SBUILDER(builder, 1024);
        sbuilder_format(&builder, "Need token %s, but given ", token_type_name(type));
        sbuilder_token(&builder, token);
        require(false, builder.buf, stream);
    }
}

static inline void require_id(token_t * token, const char * value, token_stream_t * stream) {
    if (token == NULL || token->type != TOKEN_ID || !string_equals_c(&token->value, value)) {
        SBUILDER(builder, 1024);
        sbuilder_format(&builder, "Need id %s, but given ", value);
        sbuilder_token(&builder, token);
        require(false, builder.buf, stream);
    }
}

#endif //EVO_UTILS_H
