/**
 * @author goldolphin
 *         2015-09-01 19:58:58.
 */

#ifndef EVO_STRING_H
#define EVO_STRING_H

#include <stdint.h>
#include "sbuilder.h"
#include "memory.h"

typedef struct {
    uint8_t * value;
    int len;
} string_t;

#define STRING(str) (&(string_t){(uint8_t *) str, sizeof(str)-1})

void string_init(string_t *str, uint8_t *value, int len);

bool string_equals_u(string_t * str, uint8_t *value, int len);

bool string_equals_c(string_t * str, const char * s);

bool string_equals(string_t * str1, string_t * str2);

bool sbuilder_string(sbuilder_t * builder, string_t * str);

size_t string_hash_func (void * key);

bool string_equal_func (void * key1, void * key2);

static inline string_t * string_new(int len) {
    uint8_t * s = new_array(uint8_t, len);
    string_t * str = new_data(string_t);
    string_init(str, s, len);
    return str;
}

static inline string_t * string_dup(string_t * from) {
    string_t * to = string_new(from->len);
    memcpy(to->value, from->value, (size_t) from->len);
    return to;
}

static inline string_t * string_concat2(string_t * a, string_t * b) {
    string_t * to = string_new(a->len + b->len);
    memcpy(to->value, a->value, (size_t) a->len);
    memcpy(&to->value[a->len], b->value, (size_t) b->len);
    return to;
}

static inline string_t * string_concat3(string_t * a, string_t * b, string_t * c) {
    string_t * to = string_new(a->len + b->len + c->len);
    memcpy(to->value, a->value, (size_t) a->len);
    memcpy(&to->value[a->len], b->value, (size_t) b->len);
    memcpy(&to->value[a->len+b->len], c->value, (size_t) c->len);
    return to;
}

static inline void string_free(string_t * s) {
    free(s->value);
    free(s);
}

#endif //EVO_STRING_H
