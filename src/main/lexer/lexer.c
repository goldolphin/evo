/**
 * @author goldolphin
 *         2015-08-19 20:44:44.
 */

#include <stdio.h>
#include <utils/matcher.h>
#include <utils/sbuilder.h>
#include <utils/string.h>
#include "lexer.h"
#include "character.h"
#include "char_stream.h"

static token_t s_token_end = {TOKEN_END, {"end", 3}};

static token_type_t keywords[] = {
        TOKEN_IMPORT,
        TOKEN_LET,
        TOKEN_STRUCT,
        TOKEN_EXTENDS,
        TOKEN_FUN,
        TOKEN_OPERATOR,
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
        TOKEN_LINEBREAK,
};

static inline void buffer_reset(lexer_context_t * context) {
    context->buf_len = 0;
}

static inline void buffer_add(lexer_context_t * context, uint8_t c) {
    ensure(context->buf_len < MAX_TOKEN_LEN);
    context->buf[context->buf_len++] = c;
}

void lexer_init(lexer_t * lexer) {
    int keyword_num = (int) array_size(keywords);
    lexer->keywords = new_array(matcher_pattern_t, keyword_num);
    for (int i = 0; i < keyword_num; ++i) {
        token_t * token = new_data(token_t);
        const char * value = token_type_value(keywords[i]);
        token_init(token, keywords[i], (char *) value, (int) strlen(value));
        matcher_pattern_init(&lexer->keywords[i], (uint8_t *) token->value.value, token->value.len, token);
    }
    matcher_init(&lexer->keyword_matcher, lexer->keywords, keyword_num);

    int punctuation_num = (int) array_size(punctuations);
    lexer->punctuations = new_array(token_t, punctuation_num);
    for (int i = 0; i < punctuation_num; ++i) {
        const char * value = token_type_value(punctuations[i]);
        token_init(&lexer->punctuations[i], punctuations[i], (char *) value, (int)strlen(value));
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

void lexer_init_context(lexer_t * lexer, lexer_context_t * context) {
    source_info_init(&context->source_info, 1024);
}

static inline token_t * parse_punctuation(lexer_t * lexer, uint8_t c) {
    int punctuation_num = (int) array_size(punctuations);
    for (int i = 0; i < punctuation_num; ++i) {
        if (string_equals_s(&lexer->punctuations[i].value, (char *) &c, 1)) {
            return &lexer->punctuations[i];
        }
    }
    return NULL;
}

static inline bool_char_t poll_char(lexer_context_t * context, char_stream_t * stream) {
    bool_char_t c = char_stream_poll(stream, 0);
    if (c.success) {
        source_info_update(&context->source_info, c.c);
    }
    return c;
}

static inline void print_error(const char * msg, char_stream_t * stream, lexer_context_t * ctx) {
    fprintf(stderr, "[LEXER ERROR] %s\n\tat [%d, %d] %s~~~\n", msg, ctx->source_info.line, ctx->source_info.column, ctx->source_info.buffer.buf);
}

#define to_int(c) ((c)-'0')

static inline bool parse_integer(char_stream_t * stream, int *pos) {
    enum {
        STATE_START,
        STATE_SIGN,
        STATE_DIGIT,
    };
    int state = STATE_START;
    for (int i = *pos; ; ++ i) {
        bool_char_t c = char_stream_peek(stream, i);
        switch (state) {
            case STATE_START:
                if (!c.success) {
                    return false;
                } else if (is_digit(c.c)) {
                    state = STATE_DIGIT;
                } else if (c.c == '+' || c.c == '-') {
                    state = STATE_SIGN;
                } else {
                    return false;
                }
                break;
            case STATE_SIGN:
                if (!c.success) {
                    return false;
                } else if (is_digit(c.c)) {
                    state = STATE_DIGIT;
                } else {
                    return false;
                }
                break;
            case STATE_DIGIT:
                if (!c.success || !is_digit(c.c)) {
                    *pos = i;
                    return true;
                }
                break;
        }
    }
}

static inline void poll_to_buffer(char_stream_t *stream, lexer_context_t *ctx, int n) {
    for (int i = 0; i < n; ++i) {
        bool_char_t c = poll_char(ctx, stream);
        ensure(c.success);
        buffer_add(ctx, c.c);
    }
}

static inline bool parse_number(char_stream_t *stream, lexer_context_t *ctx, bool *is_double) {
    int pos = 0;
    bool is_d = false;

    bool has_base = parse_integer(stream, &pos);
    bool_char_t c = char_stream_peek(stream, pos);
    if (c.success && c.c == '.') {
        ++pos;
        if (parse_integer(stream, &pos)) {
            is_d = true;
            has_base = true;
        }
    }
    if (!has_base) return false;

    c = char_stream_peek(stream, pos);
    if (c.success && c.c == 'e') {
        ++pos;
        if (!parse_integer(stream, &pos)) {
            return false;
        }
        is_d = true;
    }

    c = char_stream_peek(stream, pos);
    if (c.success && is_regular_identifier_letter(c.c)) {
        return false;
    }

    poll_to_buffer(stream, ctx, pos);
    *is_double = is_d;
    return true;
}

static inline bool parse_string(char_stream_t * stream, lexer_context_t *ctx) {
    bool_char_t c = poll_char(ctx, stream);
    if (!c.success || c.c != '"') return false;
    buffer_add(ctx, c.c);
    while (true) {
        c = poll_char(ctx, stream);
        if (!c.success) break;
        buffer_add(ctx, c.c);
        if (c.c == '"') {
            return true;
        }
        if (c.c == '\\') {
            c = poll_char(ctx, stream);
            if (c.success) {
                buffer_add(ctx, c.c);
            } else {
                return false;
            }
        }
    }
    return false;
}

static inline token_t * parse_keyword(lexer_t * lexer, char_stream_t * stream, lexer_context_t *ctx) {
    matcher_context_t mc;
    matcher_reset_context(&lexer->keyword_matcher, &mc);
    for (int i = 0; ; ++i) {
        bool_char_t next = char_stream_peek(stream, i);
        if (!next.success || !is_identifier_letter(next.c)) {
            matcher_pattern_t * pattern = matcher_context_pattern(&mc);
            if (pattern != NULL) {
                poll_to_buffer(stream, ctx, i);
                return pattern->extra;
            } else {
                return NULL;
            }
        }
        if (!matcher_match(&lexer->keyword_matcher, &mc, next.c)) {
            return NULL;
        }
    }
}

token_t * lexer_poll(lexer_t * lexer, lexer_context_t * context, char_stream_t * stream) {
    buffer_reset(context);

    while (true) {
        bool_char_t c = char_stream_peek(stream, 0);
        if (!c.success) return &s_token_end;

        // Parse comments
        if (c.c == '/') {
            bool_char_t next = char_stream_peek(stream, 1);
            if (next.success && next.c == '/') {
                poll_char(context, stream);
                poll_char(context, stream);
                while (true) {
                    next = poll_char(context, stream);
                    if (!next.success || is_linebreak(next.c)) break;
                }
                continue;
            }
        }

        // Parse string literals.
        if (c.c == '"') {
            if (parse_string(stream, context)) {
                token_init(&context->token, TOKEN_STRING, (char *) context->buf, context->buf_len);
                return &context->token;
            }
            print_error("Invalid string", stream, context);
            return NULL;
        }

        // Parse number literals.
        if (c.c == '+' || c.c == '-' || is_digit(c.c)) {
            bool is_double;
            if (parse_number(stream, context, &is_double)) {
                if (is_double) {
                    token_init(&context->token, TOKEN_DOUBLE, (char *) context->buf, context->buf_len);
                } else {
                    token_init(&context->token, TOKEN_LONG, (char *) context->buf, context->buf_len);
                }
                return &context->token;
            }
        }

        if (!is_identifier_letter(c.c)) {
            // Parse punctuations
            token_t * token = parse_punctuation(lexer, c.c);
            poll_char(context, stream);
            if (token != NULL) {
                return token;
            }
            continue;
        }

        // Parse keywords
        token_t * keyword = parse_keyword(lexer, stream, context);
        if (keyword != NULL) {
            return keyword;
        }

        // Parse IDs.
        bool is_regular = is_regular_identifier_letter(c.c);
        while (true) {
            bool_char_t next = char_stream_peek(stream, 0);
            if (!next.success || !is_identifier_letter(next.c) || is_regular_identifier_letter(next.c) != is_regular) {
                token_init(&context->token, TOKEN_ID, (char *) context->buf, context->buf_len);
                return &context->token;
            }
            buffer_add(context, next.c);
            poll_char(context, stream);
        }
    }
}
