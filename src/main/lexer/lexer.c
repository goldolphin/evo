/**
 * @author caofuxiang
 *         2015-08-19 20:44:44.
 */

#include <stdio.h>
#include <utils/matcher.h>
#include <utils/sbuilder.h>
#include "lexer.h"
#include "character.h"

static char * keywords[] = {
        "import",
        "let",
        "struct",
        "extends",
        "fun",
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
        matcher_pattern_init(&lexer->keywords[i], (uint8_t *) keywords[i], (int) strlen(keywords[i]), int2ptr(i, void));
    }
    matcher_init(&lexer->keyword_matcher, lexer->keywords, keyword_num);
}

void lexer_destroy(lexer_t * lexer) {
    matcher_destroy(&lexer->keyword_matcher);
}

static inline void print_token(const char * type, uint8_t * value, int len) {
    SBUILDER(builder, 1024);
    sbuilder_nstr(&builder, (const char *) value, len);
    printf("type=%s, value=`%s`\n", type, builder.buf);
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

bool lexer_match(lexer_t * lexer, uint8_t * str, int len) {
    matcher_context_t ctx;
    buffer_t buf;
    matcher_reset_context(&lexer->keyword_matcher, &ctx);
    buffer_reset(&buf);
    for (int i = 0; i < len; ) {
        uint8_t c = str[i];
        if (!is_visible(c)) {
            ++i;
            continue;
        }

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
                print_token("STR", buf.buffer, buf.length);
                buffer_reset(&buf);
                i += l;
                continue;
            }
        }

        // Parse number literals.
        if (is_digit(c)) {
            int l = parse_float(str+i, len-i, &buf);
            if (l > 0) {
                print_token("NUM", buf.buffer, buf.length);
                buffer_reset(&buf);
                i += l;
                continue;
            }
        }

        if (!is_identifier_letter(c)) {
            // Parse punctuations
            if (is_punctuation(c)) {
                print_token("PUNC", &c, 1);
                buffer_reset(&buf);
            }
            matcher_pattern_t * pattern = matcher_context_pattern(&ctx);
            if (pattern != NULL) {
                print_token("KEYWORD", pattern->value, pattern->value_len);
                matcher_reset_context(&lexer->keyword_matcher, &ctx);
                buffer_reset(&buf);
            } else if (buf.length > 0) {
                print_token("ID", buf.buffer, buf.length);
                matcher_reset_context(&lexer->keyword_matcher, &ctx);
                buffer_reset(&buf);
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