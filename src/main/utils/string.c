/**
 * @author goldolphin
 *         2015-09-01 19:58:58.
 */

#include <string.h>
#include "string.h"

void string_init(string_t *str, char *value, size_t len) {
    str->value = value;
    str->len = len;
}

bool string_equals_s(string_t *str, char *value, size_t len) {
    return str->len == len && memcmp(str->value, value, (size_t) len) == 0;
}

bool string_equals_c(string_t * str, const char * s) {
    for (int i = 0; i < str->len; ++i) {
        if (s[i] == '\0' || s[i] != str->value[i]) {
            return false;
        }
    }
    return s[str->len] == '\0';
}

bool string_equals(string_t * str1, string_t * str2) {
    if (str1 == str2) return true;
    if (str1 == NULL || str2 == NULL) return false;
    return string_equals_s(str1, str2->value, str2->len);
}

bool sbuilder_string(sbuilder_t * builder, string_t * str) {
    return sbuilder_nstr(builder, (const char *) str->value, str->len);
}

size_t string_hash_func (void * key) {
    string_t * s = key;
    size_t h = 0;
    for (int i = 0; i < s->len; i ++) {
        h = 31*h + s->value[i];
    }
    return h;
}

bool string_equal_func (void * key1, void * key2) {
    return string_equals(key1, key2);
}
