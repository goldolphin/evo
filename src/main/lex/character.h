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

static inline bool is_visible(uint8_t c) {
    return c >= 33 && c != 127;
}

static inline bool is_space(uint8_t c) {
    return isspace(c);
}

static inline bool is_punctuation(uint8_t c) {
    return strchr("'\"()\\,.:;[]{}`", c) != NULL;
}

static inline bool is_delimiter(uint8_t c) {
    return is_space(c) || is_punctuation(c);
}

static inline bool is_identifier_letter(uint8_t c) {
    return is_visible(c) && !is_delimiter(c);
}

#endif //EVO_CHARACTER_H
