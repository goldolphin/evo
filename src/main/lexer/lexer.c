/**
 * @author goldolphin
 *         2015-08-19 20:44:44.
 */

#include <stdio.h>
#include <utils/matcher.h>
#include <utils/sbuilder.h>
#include "lexer.h"
#include "character.h"
#include "char_stream.h"

static token_type_t keywords[] = {
        TOKEN_IMPORT,
        TOKEN_LET,
        TOKEN_STRUCT,
        TOKEN_EXTENDS,
        TOKEN_FUN,
};

static token_type_t punctuations[] = {
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_COMMA,
        TOKEN_PERIOD,
        TOKEN_COLON,
        TOKEN_SEMICOLON,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_GRAVE,
};

static inline void buffer_reset(lexer_context_t * context) {
    context->buf_len = 0;
}

static inline void buffer_add(lexer_context_t * context, uint8_t c) {
    context->buf[context->buf_len++] = c;
}

void lexer_init(lexer_t * lexer) {
    int keyword_num = (int) array_size(keywords);
    lexer->keywords = new_array(matcher_pattern_t, keyword_num);
    for (int i = 0; i < keyword_num; ++i) {
        token_t * token = new_data(token_t);
        const char * value = token_type_value(keywords[i]);
        token_init(token, keywords[i], (uint8_t *) value, (int) strlen(value));
        matcher_pattern_init(&lexer->keywords[i], token->value, token->value_len, token);
    }
    matcher_init(&lexer->keyword_matcher, lexer->keywords, keyword_num);

    int punctuation_num = (int) array_size(punctuations);
    lexer->punctuations = new_array(token_t, punctuation_num);
    for (int i = 0; i < punctuation_num; ++i) {
        const char * value = token_type_value(punctuations[i]);
        token_init(&lexer->punctuations[i], punctuations[i], (uint8_t *)value, (int)strlen(value));
    }
}

void lexer_destroy(lexer_t * lexer) {
    int keyword_num = (int) array_size(keywords);
    for (int i = 0; i < keyword_num; ++i) {
        free(lexer->keywords[i].extra);
    }
    matcher_destroy(&lexer->keyword_matcher);
    free(lexer->keywords);
    free(lexer->punctuations);
}

void lexer_reset_context(lexer_t * lexer, lexer_context_t * context) {

}

static inline token_t * parse_punctuation(lexer_t * lexer, uint8_t c) {
    int punctuation_num = (int) array_size(punctuations);
    for (int i = 0; i < punctuation_num; ++i) {
        if (lexer->punctuations[i].value[0] == c) {
            return &lexer->punctuations[i];
        }
    }
    return NULL;
}

static inline void print_error(const char * msg, char_stream_t * stream, lexer_context_t * ctx) {
    SBUILDER(builder, 4096);
    sbuilder_nstr(&builder, (const char *) ctx->buf, ctx->buf_len);
    sbuilder_str(&builder, "_###_");
    while (true) {
        bool_char_t c = char_stream_poll(stream);
        if (!c.success || is_linebreak(c.c)) break;
        sbuilder_char(&builder, c.c);
    }
    printf("%s: %s\n", msg, builder.buf);
}

static inline bool parse_integer(char_stream_t * stream, lexer_context_t *ctx) {
    bool success = false;
    while (true) {
        bool_char_t c = char_stream_peek(stream);
        if (!c.success || !is_digit(c.c)) break;
        buffer_add(ctx, c.c);
        success = true;
        char_stream_poll(stream);
    }
    return success;
}

static inline bool parse_float(char_stream_t * stream, lexer_context_t *ctx) {
    parse_integer(stream, ctx);
    bool_char_t c = char_stream_peek(stream);
    if (c.success && c.c == '.') {
        char_stream_poll(stream);
        if (!parse_integer(stream, ctx)) {
            return false;
        }
    }

    c = char_stream_peek(stream);
    if (c.success && c.c == 'e') {
        char_stream_poll(stream);
        if (!parse_integer(stream, ctx)) {
            return false;
        }
    }
    return true;
}

static inline bool parse_string(char_stream_t * stream, lexer_context_t *ctx) {
    while (true) {
        bool_char_t c = char_stream_poll(stream);
        if (!c.success) break;
        buffer_add(ctx, c.c);
        if (c.c == '"') {
            return true;
        }
        if (c.c == '\\') {
            c = char_stream_poll(stream);
            if (c.success) {
                buffer_add(ctx, c.c);
            } else {
                return false;
            }
        }
    }
    return false;
}

static inline token_t * parse_keyword(lexer_t * lexer, char_stream_t * stream, lexer_context_t *ctx, uint8_t c) {
    matcher_context_t mc;
    matcher_reset_context(&lexer->keyword_matcher, &mc);
    buffer_add(ctx, c);
    if (!matcher_match(&lexer->keyword_matcher, &mc, c)) {
        return NULL;
    }
    while (true) {
        bool_char_t next = char_stream_peek(stream);
        if (!next.success || !is_identifier_letter(next.c)) {
            matcher_pattern_t * pattern = matcher_context_pattern(&mc);
            if (pattern != NULL) {
                return pattern->extra;
            } else {
                return NULL;
            }
        }
        if (!matcher_match(&lexer->keyword_matcher, &mc, next.c)) {
            return NULL;
        }
        buffer_add(ctx, next.c);
        char_stream_poll(stream);
    }
}

token_t * lexer_poll(lexer_t * lexer, lexer_context_t * context, char_stream_t * stream) {
    buffer_reset(context);

    while (true) {
        bool_char_t c = char_stream_poll(stream);
        if (!c.success) return TOKEN_END;

        // Parse comments
        if (c.c == '/') {
            bool_char_t next = char_stream_peek(stream);
            if (next.success && next.c == '/') {
                char_stream_poll(stream);
                while (true) {
                    next = char_stream_poll(stream);
                    if (!next.success || is_linebreak(next.c)) break;
                }
                continue;
            }
        }

        // Parse string literals.
        if (c.c == '"') {
            buffer_add(context, c.c);
            if (parse_string(stream, context)) {
                token_init(&context->token, TOKEN_STRING, context->buf, context->buf_len);
                return &context->token;
            }
            print_error("Invalid string", stream, context);
            return NULL;
        }

        // Parse number literals.
        if (is_digit(c.c)) {
            buffer_add(context, c.c);
            if (parse_float(stream, context)) {
                token_init(&context->token, TOKEN_NUMBER, context->buf, context->buf_len);
                return &context->token;
            }
            print_error("Invalid number", stream, context);
            return NULL;
        }

        if (!is_identifier_letter(c.c)) {
            // Parse punctuations
            token_t * token = parse_punctuation(lexer, c.c);
            if (token != NULL) {
                return token;
            }
            continue;
        }

        // Parse keywords
        token_t * keyword = parse_keyword(lexer, stream, context, c.c);
        if (keyword != NULL) {
            return keyword;
        }

        // Parse IDs.
        while (true) {
            bool_char_t next = char_stream_peek(stream);
            if (!next.success || !is_identifier_letter(next.c)) {
                token_init(&context->token, TOKEN_ID, context->buf, context->buf_len);
                return &context->token;
            }
            buffer_add(context, next.c);
            char_stream_poll(stream);
        }
    }
}
