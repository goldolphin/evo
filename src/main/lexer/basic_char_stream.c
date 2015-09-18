/**
 * @author caofuxiang
 *         2015-09-15 10:39:39.
 */

#include <utils/memory.h>
#include "basic_char_stream.h"

static bool_char_t peek(char_stream_t *stream, int n) {
    basic_char_stream_t * s = container_of(stream, basic_char_stream_t, super);
    ensure(n >= 0);
    bool_char_t res;
    int left = s->buffer_end-s->buffer_start;
    if (n >= left) {
        res.success = false;
        return res;
    }
    res.success = true;
    res.c = s->buffer[s->buffer_start+n];
    return res;
}

static bool_char_t poll(char_stream_t *stream, int n) {
    bool_char_t res = peek(stream, n);
    if (!res.success) return res;
    basic_char_stream_t * s = container_of(stream, basic_char_stream_t, super);
    s->buffer_start += n+1;
    return res;
}

void basic_char_stream_init(basic_char_stream_t * stream, uint8_t * buffer, int buffer_size) {
    char_stream_init(&stream->super, peek, poll);
    stream->buffer = buffer;
    stream->buffer_start = 0;
    stream->buffer_end = buffer_size;
}
