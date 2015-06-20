/**
 * 
 * @author goldolphin
 *         2015-05-02 12:47
 */
#ifndef EVO_CHARACTER_H
#define EVO_CHARACTER_H

#include <errno.h>
#include <stdbool.h>

typedef int error_t;
#define EOK 0

typedef unsigned char char_t;
#define CHAR_NULL ((char_t)0)
#define CHAR_COUNT 256

static inline bool is_digit(char_t c) {
    return c >= '0' && c <= '9';
}

static inline bool is_visible(char_t c) {
    return c >= 33 && c != 127;
}

bool is_identifier_letter(char_t c);

#endif //EVO_CHARACTER_H
