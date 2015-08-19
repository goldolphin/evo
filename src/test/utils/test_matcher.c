/**
 * @author goldolphin
 *         2015-08-18 17:32:32.
 */
#include <utils/matcher.h>
#include <stdio.h>

int main() {
    char * keywords[] = {
            "auto",
            "break",
            "case",
            "char",
            "const",
            "continue",
            "default",
            "do",
            "double",
            "else",
            "enum",
            "extern",
            "float",
            "for",
            "goto",
            "if",
            "inline",
            "int",
            "long",
            "register",
            "restrict",
            "return",
            "short",
            "signed",
            "sizeof",
            "static",
            "struct",
            "switch",
            "typedef",
            "union",
            "unsigned",
            "void",
            "volatile",
            "while",
    };
    int keyword_num = array_size(keywords);

    matcher_pattern_t patterns[keyword_num];
    for (int i = 0; i < keyword_num; ++i) {
        matcher_pattern_init(&patterns[i], (uint8_t *) keywords[i], (int) strlen(keywords[i]), int2ptr(i, void));
    }

    matcher_t matcher;
    matcher_init(&matcher, patterns, keyword_num);

    for (int i = 0; i < keyword_num; ++i) {
        matcher_pattern_t *pattern = matcher_match_seq(&matcher, (uint8_t *) keywords[i], (int) strlen(keywords[i]));
        ensure(pattern != NULL);
        ensure(pattern->value == (uint8_t *)keywords[i]);
        ensure(ptr2int(pattern->extra, int) == i);
        printf("pattern[%d]=%s\n", ptr2int(pattern->extra, int), pattern->value);
    }

    char * inputs[] = {
            "hi",
            "hello",
            "world",
            "unio"
    };
    int input_num = array_size(inputs);

    for (int i = 0; i < input_num; ++i) {
        matcher_pattern_t *pattern = matcher_match_seq(&matcher, (uint8_t *) inputs[i], (int) strlen(inputs[i]));
        ensure(pattern == NULL);
    }

    return 0;
}