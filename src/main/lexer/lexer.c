/**
 * @author goldolphin
 *         2015-08-19 20:44:44.
 */

#include <stdio.h>
#include <utils/matcher.h>
#include <utils/sbuilder.h>
#include "lexer.h"
#include "character.h"

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

typedef struct {
    uint8_t buffer[MAX_TOKEN_LEN];
    int length;
} buffer_t;

static inline void buffer_reset(buffer_t * buffer) {
    buffer->length = 0;
}

static inline void buffer_add(buffer_t * buffer, uint8_t c) {
    buffer->buffer[buffer->length++] = c;
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

static inline token_t * parse_punctuation(lexer_t * lexer, uint8_t c) {
    int punctuation_num = (int) array_size(punctuations);
    for (int i = 0; i < punctuation_num; ++i) {
        if (lexer->punctuations[i].value[0] == c) {
            return &lexer->punctuations[i];
        }
    }
    return NULL;
}

static inline void print_error(const char * msg, char_stream_t * stream, buffer_t * buf) {
    SBUILDER(builder, 4096);
    sbuilder_nstr(&builder, (const char *) buf->buffer, buf->length);
    sbuilder_str(&builder, "_###_");
    while (true) {
        bool_char_t c = char_stream_poll(stream);
        if (!c.success || is_linebreak(c.c)) break;
        sbuilder_char(&builder, c.c);
    }
    printf("%s: %s\n", msg, builder.buf);
}

static inline bool parse_integer(char_stream_t * stream, buffer_t * buf) {
    bool success = false;
    while (true) {
        bool_char_t c = char_stream_peek(stream);
        if (!c.success || !is_digit(c.c)) break;
        buffer_add(buf, c.c);
        success = true;
        char_stream_poll(stream);
    }
    return success;
}

static inline bool parse_float(char_stream_t * stream, buffer_t * buf) {
    parse_integer(stream, buf);
    bool_char_t c = char_stream_peek(stream);
    if (c.success && c.c == '.') {
        char_stream_poll(stream);
        if (!parse_integer(stream, buf)) {
            return false;
        }
    }

    c = char_stream_peek(stream);
    if (c.success && c.c == 'e') {
        char_stream_poll(stream);
        if (!parse_integer(stream, buf)) {
            return false;
        }
    }
    return true;
}

static inline bool parse_string(char_stream_t * stream, buffer_t * buf) {
    while (true) {
        bool_char_t c = char_stream_poll(stream);
        if (!c.success) break;
        buffer_add(buf, c.c);
        if (c.c == '"') {
            return true;
        }
        if (c.c == '\\') {
            c = char_stream_poll(stream);
            if (c.success) {
                buffer_add(buf, c.c);
            } else {
                return false;
            }
        }
    }
    return false;
}

bool lexer_match(lexer_t * lexer, char_stream_t * stream, lexer_callback_t callback, void * extra) {
    matcher_context_t ctx;
    buffer_t buf;
    matcher_reset_context(&lexer->keyword_matcher, &ctx);
    buffer_reset(&buf);
    while (true) {
        bool_char_t c = char_stream_poll(stream);
        if (!c.success) break;

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
            buffer_add(&buf, c.c);
            if (parse_string(stream, &buf)) {
                token_t token;
                token_init(&token, TOKEN_STRING, buf.buffer, buf.length);
                callback(&token, extra);
                buffer_reset(&buf);
                continue;
            }
            print_error("Invalid string", stream, &buf);
            return false;
        }

        // Parse number literals.
        if (is_digit(c.c)) {
            buffer_add(&buf, c.c);
            if (parse_float(stream, &buf)) {
                token_t token;
                token_init(&token, TOKEN_NUMBER, buf.buffer, buf.length);
                callback(&token, extra);
                buffer_reset(&buf);
                continue;
            }
            print_error("Invalid number", stream, &buf);
            return false;
        }

        if (!is_identifier_letter(c.c)) {
            matcher_pattern_t * pattern = matcher_context_pattern(&ctx);
            if (pattern != NULL) {
                callback(pattern->extra, extra);
                matcher_reset_context(&lexer->keyword_matcher, &ctx);
                buffer_reset(&buf);
            } else if (buf.length > 0) {
                token_t token;
                token_init(&token, TOKEN_ID, buf.buffer, buf.length);
                callback(&token, extra);
                matcher_reset_context(&lexer->keyword_matcher, &ctx);
                buffer_reset(&buf);
            }
            // Parse punctuations
            token_t * token = parse_punctuation(lexer, c.c);
            if (token != NULL) {
                callback(token, extra);
            }
            continue;
        }

        // Parse keywords
        if (matcher_match(&lexer->keyword_matcher, &ctx, c.c)) {
            buffer_add(&buf, c.c);
            continue;
        }
        matcher_reset_context(&lexer->keyword_matcher, &ctx);

        // Parse IDs.
        buffer_add(&buf, c.c);
        while (true) {
            bool_char_t next = char_stream_peek(stream);
            if (!next.success || !is_identifier_letter(next.c)) break;
            buffer_add(&buf, next.c);
            char_stream_poll(stream);
        }
    }
    return true;
}

int lexer_read_line(uint8_t * line, int len, FILE * file) {
    for (int i = 0; i < len; ++i) {
        int c = fgetc(file);
        if (is_linebreak((uint8_t) c)) {
            line[i] = (uint8_t) c;
            return i+1;
        }
        if (c == EOF) {
            if (i == 0) {
                return -1;
            }
            line[i] = '\n';
            return i+1;
        }
        line[i] = (uint8_t) c;
    }
    return len;
}

