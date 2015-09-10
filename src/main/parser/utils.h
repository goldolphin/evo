/**
 * @author goldolphin
 *         2015-09-08 16:26:26.
 */

#ifndef EVO_UTILS_H
#define EVO_UTILS_H

#include <stdbool.h>
#include <lexer/source_info.h>
#include "token_stream.h"

static inline void parser_error(const char * func_name, const char * message, token_stream_t * stream) {
    source_info_t *source_info = token_stream_source_info(stream);
    fprintf(stderr, "[PARSER ERROR] %s: %s\n\tat [%d, %d] %s~~~\n", func_name, message, source_info->line, source_info->column, source_info->buffer.buf);
}

static inline void parser_require(const char * func_name, bool condition, const char * message, token_stream_t * stream) {
    if (!condition) {
        parser_error(func_name, message, stream);
        ensure(false);
    }
}

static inline void parser_require_token(const char * func_name, token_t * token, token_type_t type, token_stream_t * stream) {
    if (token == NULL || token->type != type) {
        SBUILDER(builder, 1024);
        sbuilder_format(&builder, "Need token `%s`, but given ", token_type_value(type));
        sbuilder_token(&builder, token);
        parser_require(func_name, false, builder.buf, stream);
    }
}

static inline void parser_require_id(const char * func_name, token_t * token, const char * value, token_stream_t * stream) {
    if (token == NULL || token->type != TOKEN_ID || !string_equals_c(&token->value, value)) {
        SBUILDER(builder, 1024);
        sbuilder_format(&builder, "Need id %s, but given ", value);
        sbuilder_token(&builder, token);
        parser_require(func_name, false, builder.buf, stream);
    }
}

#define require(condition, message, stream) parser_require(__FUNCTION__, condition, message, stream)

#define require_token(token, type, stream) parser_require_token(__FUNCTION__, token, type, stream)

#define require_id(token, value, stream) parser_require_id(__FUNCTION__, token, value, stream)

static inline string_t * string_dup(string_t * from) {
    uint8_t * s = new_array(uint8_t, from->len);
    memcpy(s, from->value, (size_t) from->len);
    string_t * to = new_data(string_t);
    string_init(to, s, from->len);
    return to;
}

#endif //EVO_UTILS_H
