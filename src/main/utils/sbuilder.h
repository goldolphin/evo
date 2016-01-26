/**
 * @author goldolphin
 *         2015-06-01 20:36:36.
 */

#ifndef EVO_SBUILDER_H
#define EVO_SBUILDER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct sbuilder_s {
    size_t len;
    size_t capacity;
    char * buf;
} sbuilder_t;

void sbuilder_init(sbuilder_t * builder, char * buf, size_t capacity);

static inline size_t sbuilder_len(sbuilder_t * builder) {
    return builder->len;
}

static inline void sbuilder_reset(sbuilder_t * builder) {
    builder->len = 0;
    builder->buf[0] = '\0';
}

bool sbuilder_format(sbuilder_t * builder, const char *message, ...);

bool sbuilder_char(sbuilder_t * builder, char c);

static inline bool sbuilder_int(sbuilder_t * builder, int i) {
    return sbuilder_format(builder, "%d", i);
}

static inline bool sbuilder_long(sbuilder_t * builder, long l) {
    return sbuilder_format(builder, "%ld", l);
}

static inline bool sbuilder_bool(sbuilder_t * builder, bool b) {
    return sbuilder_char(builder, (char) (b ? 't' : 'f'));
}

static inline bool sbuilder_binary(sbuilder_t * builder, uint64_t i, size_t len) {
    for (size_t j = 0; j < len; ++j) {
        uint64_t b = i&(1u << (len-1-j));
        bool res = sbuilder_char(builder, (char) (b == 0 ? '0' : '1'));
        if (!res) return res;
    }
    return true;
}

static inline bool sbuilder_str(sbuilder_t * builder, const char * s) {
    return sbuilder_format(builder, "%s", s);
}

bool sbuilder_nstr(sbuilder_t * builder, const char * s, size_t n);

#ifndef VNAME
#define CONCATE0(x, y) x##y
#define CONCATE(x, y) CONCATE0(x, y)
#define VNAME(x) CONCATE(x, __LINE__)
#endif

#define SBUILDER(v, n) \
sbuilder_t v; \
char VNAME(sbuf_)[n]; \
sbuilder_init(&v, VNAME(sbuf_), n)

#endif //EVO_SBUILDER_H
