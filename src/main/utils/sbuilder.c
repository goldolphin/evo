/**
 * @author goldolphin
 *         2015-06-01 20:36:36.
 */

#include <string.h>
#include "sbuilder.h"
void sbuilder_init(sbuilder_t * builder, char * buf, size_t capacity) {
    builder->buf = buf;
    builder->capacity = capacity;
    sbuilder_reset(builder);
}

bool sbuilder_format(sbuilder_t * builder, const char *format, ...) {
    size_t left = builder->capacity - builder->len;
    if (left <= 1) {
        return false;
    }
    va_list args;
    va_start(args, format);
    int count = vsnprintf(builder->buf+builder->len, left, format, args);
    va_end(args);
    if (count >= left) {
        builder->len = builder->capacity;
    } else {
        builder->len += count;
    }
    return true;
}

bool sbuilder_char(sbuilder_t * builder, char c) {
    if (builder->capacity - builder->len <= 1) {
        return false;
    }
    builder->buf[builder->len++] = c;
    builder->buf[builder->len] = '\0';
    return true;
}

bool sbuilder_nstr(sbuilder_t * builder, const char * s, size_t n) {
    size_t left = builder->capacity - builder->len;
    if (n > left-1) {
        return false;
    }
    for (int i = 0; i < n && s[i] != '\0' ; ++i) {
        builder->buf[builder->len++] = s[i];
    }
    builder->buf[builder->len] = '\0';
    return true;
}
