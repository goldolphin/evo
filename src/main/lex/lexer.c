/**
 * @author caofuxiang
 *         2015-08-19 20:44:44.
 */

#include <stdio.h>
#include <utils/matcher.h>
#include "lexer.h"
#include "character.h"

static char * keywords[] = {
        "let",
        "import",
        "struct",
        "extends",

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
};

void lexer_init(lexer_t * lexer) {
    int keyword_num = array_size(keywords);
    matcher_pattern_t patterns[keyword_num];
    for (int i = 0; i < keyword_num; ++i) {
        matcher_pattern_init(&patterns[i], (uint8_t *) keywords[i], (int) strlen(keywords[i]), int2ptr(i, void));
    }
    matcher_init(&lexer->keyword_matcher, patterns, keyword_num);
}

void lexer_destroy(lexer_t * lexer) {
    matcher_destroy(&lexer->keyword_matcher);
}

void lexer_reset_context(lexer_t * lexer, lexer_context_t * context) {
    matcher_reset_context(&lexer->keyword_matcher, &context->keyword_context);
    context->buffer_len = 0;
}

static inline void buffer_add(lexer_context_t * context, uint8_t c) {
    context->buffer[context->buffer_len++] = c;
}

static inline void print_token(uint8_t * type, uint8_t * value) {
    printf("type=%s, value=%s\n", type, value);
}

bool lexer_match(lexer_t * lexer, lexer_context_t * context, uint8_t c) {
    if (matcher_match(&lexer->keyword_matcher, &context->keyword_context, c)) {
        buffer_add(context, c);
        return true;
    }

    matcher_pattern_t * pattern = matcher_context_pattern(&context->keyword_context);
    if (pattern != NULL) {
        if (is_delimiter(c)) {
            print_token((uint8_t *) "KEYWORD", pattern->value);
            lexer_reset_context(lexer, context);
            return lexer_match(lexer, context, c);
        }
    }
    return false;
}