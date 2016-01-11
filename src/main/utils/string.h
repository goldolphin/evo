/**
 * @author goldolphin
 *         2015-09-01 19:58:58.
 */

#ifndef EVO_STRING_H
#define EVO_STRING_H

#include <stdint.h>
#include "sbuilder.h"

typedef struct {
    uint8_t * value;
    int len;
} string_t;

#define STRING_LITERAL(str) {(uint8_t *) str, sizeof(str)-1}

void string_init(string_t *str, uint8_t *value, int len);

bool string_equals_u(string_t * str, uint8_t *value, int len);

bool string_equals_c(string_t * str, const char * s);

bool string_equals(string_t * str1, string_t * str2);

bool sbuilder_string(sbuilder_t * builder, string_t * str);

size_t string_hash_func (void * key);

bool string_equal_func (void * key1, void * key2);

#endif //EVO_STRING_H
