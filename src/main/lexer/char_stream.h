/**
 * @author goldolphin
 *         2015-08-31 15:43:43.
 */

#ifndef EVO_CHAR_STREAM_H
#define EVO_CHAR_STREAM_H

#include <stdint.h>
#include <stdbool.h>

struct char_stream_s;
typedef struct char_stream_s char_stream_t;

typedef struct {
    bool success;
    uint8_t c;
} bool_char_t;

typedef bool_char_t (* char_stream_peek_t) (char_stream_t *stream, int n);
typedef bool_char_t (* char_stream_poll_t) (char_stream_t *stream, int n);

struct char_stream_s {
    char_stream_peek_t peek;
    char_stream_poll_t poll;
};

/**
 * peek the n-th character, where n starts from 0
 */
static inline bool_char_t char_stream_peek(char_stream_t *stream, int n) {
    return stream->peek(stream, n);
}

/**
 * poll the n-th character, where n starts from 0
 */
static inline bool_char_t char_stream_poll(char_stream_t *stream, int n) {
    return stream->poll(stream, n);
}

void char_stream_init(char_stream_t *stream, char_stream_peek_t peek, char_stream_poll_t poll);

#endif //EVO_CHAR_STREAM_H
