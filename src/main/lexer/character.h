/**
 * @author goldolphin
 *         2015-08-19 17:35:35.
 */

#ifndef EVO_CHARACTER_H
#define EVO_CHARACTER_H

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

static inline bool is_digit(uint8_t c) {
    return c >= '0' && c <= '9';
}

static inline bool is_alpha(uint8_t c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static inline bool is_visible(uint8_t c) {
    return c >= 32 && c != 127;
}

static inline bool is_linebreak(uint8_t c) {
    return c == '\n' || c == '\r';
}

static inline bool is_space(uint8_t c) {
    return isspace(c) != 0;
}

static inline bool is_punctuation(uint8_t c) {
    return strchr("'\"()\\,.:;[]{}`", c) != NULL;
}

static inline bool is_delimiter(uint8_t c) {
    return is_space(c) || is_punctuation(c);
}

static inline bool is_regular_identifier_letter(uint8_t c) {
    return is_alpha(c) || is_digit(c) || c > 127 || c == '_';
}

static inline bool is_identifier_letter(uint8_t c) {
    return is_visible(c) && !is_delimiter(c);
}

static inline int escape(uint8_t * str, int len, uint8_t *c) {
    if (len < 1) return len;
    if (str[0] != '\\') {
        *c = str[0];
        return 1;
    }


}

#endif //EVO_CHARACTER_H
