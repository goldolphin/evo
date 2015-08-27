/**
 * @author caofuxiang
 *         2015-08-19 20:44:44.
 */

#include <stdio.h>
#include <utils/matcher.h>
#include <utils/sbuilder.h>
#include "lexer.h"
#include "character.h"
#include "token.h"

typedef struct {
    token_type_t type;
    char * value;
} token_define_t;

static token_define_t keywords[] = {
        {TOKEN_IMPORT, "import"},
        {TOKEN_LET, "let"},
        {TOKEN_STRUCT, "struct"},
        {TOKEN_EXTENDS, "extends"},
        {TOKEN_FUN, "fun"},
/*
        "'",
        "\"",
        "(",
        ")",
        "\\",
        ",",
        ".",
        ":",
        ";",
        "[",
        "]",
        "{",
        "}",
        */
};

static token_define_t punctuations[] = {
        {TOKEN_LPAREN, "("},
        {TOKEN_RPAREN, ")"},
        {TOKEN_COMMA, ","},
        {TOKEN_PERIOD, "."},
        {TOKEN_COLON, ":"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_LBRACKET, "["},
        {TOKEN_RBRACKET, "]"},
        {TOKEN_LBRACE, "{"},
        {TOKEN_RBRACE, "}"},
        {TOKEN_GRAVE, "`"},
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
    int keyword_num = array_size(keywords);
    lexer->keywords = new_array(matcher_pattern_t, keyword_num);
    for (int i = 0; i < keyword_num; ++i) {
        token_t * token = new_data(token_t);
        token_init(token, keywords[i].type, (uint8_t *) keywords[i].value, (int) strlen(keywords[i].value));
        matcher_pattern_init(&lexer->keywords[i], token->value, token->value_len, token);
    }
    matcher_init(&lexer->keyword_matcher, lexer->keywords, keyword_num);

    int punctuation_num = array_size(punctuations);
    lexer->punctuations = new_array(token_t, punctuation_num);
    for (int i = 0; i < punctuation_num; ++i) {
        token_init(&lexer->punctuations[i], punctuations[i].type, (uint8_t *)punctuations[i].value, (int)strlen(punctuations[i].value));
    }
}

void lexer_destroy(lexer_t * lexer) {
    int keyword_num = array_size(keywords);
    for (int i = 0; i < keyword_num; ++i) {
        free(lexer->keywords[i].extra);
    }
    matcher_destroy(&lexer->keyword_matcher);
    free(lexer->keywords);
    free(lexer->punctuations);
}

static inline token_t * parse_punctuation(lexer_t * lexer, uint8_t c) {
    int punctuation_num = array_size(punctuations);
    for (int i = 0; i < punctuation_num; ++i) {
        if (lexer->punctuations[i].value[0] == c) {
            return &lexer->punctuations[i];
        }
    }
    return NULL;
}

static inline void print_error(const char * msg, uint8_t * str, int len, int pos) {
    SBUILDER(builder, 4096);
    sbuilder_nstr(&builder, (const char *) str, pos);
    sbuilder_str(&builder, "_###_");
    sbuilder_nstr(&builder, (const char *) (str+pos), len-pos);
    printf("%s: %s\n", msg, builder.buf);
}

static inline int parse_integer(uint8_t * str, int len, buffer_t * buf) {
    for (int i = 0; ; ++i) {
        if (i >= len || !is_digit(str[i])) {
            return i;
        }
        buffer_add(buf, str[i]);
    }
}

static inline int parse_float(uint8_t * str, int len, buffer_t * buf) {
    int r = parse_integer(str, len, buf);
    if (r < len && str[r] == '.') {
        ++ r;
        r += parse_integer(str+r, len-r, buf);
        if (r == 1) return 0;
    }
    if (r < len && str[r] == 'e') {
        ++ r;
        int e = parse_integer(str+r, len-r, buf);
        if (e == 0) return 0;
    }
    return r;
}

static inline int parse_string(uint8_t * str, int len, buffer_t * buf) {
    if (len < 2) return 0;
    buffer_add(buf, str[0]);
    int r = 1;
    while (r < len) {
        uint8_t c = str[r];
        buffer_add(buf, c);
        if (c == '"') {
            return r+1;
        }
        if (c == '\\') {
            ++ r;
            if (r < len) {
                buffer_add(buf, str[r]);
            } else {
                return 0;
            }
        }
        ++ r;
    }
    return 0;
}

static inline token_t * new_token(token_type_t type, uint8_t *value, int value_len) {
    token_t * token = new_data(token_t);
    token_init(token, type, value, value_len);
    return token;
}

bool lexer_match(lexer_t * lexer, uint8_t * str, int len, lexer_callback_t callback, void * extra) {
    matcher_context_t ctx;
    buffer_t buf;
    matcher_reset_context(&lexer->keyword_matcher, &ctx);
    buffer_reset(&buf);
    for (int i = 0; i < len; ) {
        uint8_t c = str[i];
        // Parse comments
        if (c == '/') {
            if (str[i+1] == '/') {
                i += 2;
                while (i < len && !is_linebreak(str[i])) {
                    ++ i;
                }
                continue;
            }
        }

        // Parse string literals.
        if (c == '"') {
            int l = parse_string(str+i, len-i, &buf);
            if (l > 0) {
                callback(new_token(TOKEN_STRING, buf.buffer, buf.length), extra);
                buffer_reset(&buf);
                i += l;
                continue;
            }
        }

        // Parse number literals.
        if (is_digit(c)) {
            int l = parse_float(str+i, len-i, &buf);
            if (l > 0) {
                callback(new_token(TOKEN_NUMBER, buf.buffer, buf.length), extra);
                buffer_reset(&buf);
                i += l;
                continue;
            }
        }

        if (!is_identifier_letter(c)) {
            matcher_pattern_t * pattern = matcher_context_pattern(&ctx);
            if (pattern != NULL) {
                callback(pattern->extra, extra);
                matcher_reset_context(&lexer->keyword_matcher, &ctx);
                buffer_reset(&buf);
            } else if (buf.length > 0) {
                callback(new_token(TOKEN_ID, buf.buffer, buf.length), extra);
                matcher_reset_context(&lexer->keyword_matcher, &ctx);
                buffer_reset(&buf);
            }
            // Parse punctuations
            token_t * token = parse_punctuation(lexer, c);
            if (token != NULL) {
                callback(token, extra);
            }
            ++ i;
            continue;
        }

        // Parse keywords
        if (matcher_match(&lexer->keyword_matcher, &ctx, c)) {
            buffer_add(&buf, c);
            ++i;
            continue;
        }
        matcher_reset_context(&lexer->keyword_matcher, &ctx);

        // Parse IDs.
        while (i < len && is_identifier_letter(str[i])) {
            buffer_add(&buf, str[i]);
            ++i;
        }
    }
    return true;
}