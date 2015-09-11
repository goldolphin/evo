/**
 * @author goldolphin
 *         2015-08-31 15:58:58.
 */

#include <utils/memory.h>
#include "file_char_stream.h"

static inline void read(file_char_stream_t *s) {
    s->buffer_end += fread(s->buffer+s->buffer_end, 1, (size_t) s->buffer_capacity-s->buffer_end, s->file);
}

static bool_char_t peek(char_stream_t *stream, int n) {
    file_char_stream_t * s = container_of(stream, file_char_stream_t, super);
    bool_char_t res;
    ensure(n >= 0 && n < s->buffer_capacity);
    int left = s->buffer_end-s->buffer_start;
    if (n >= left) {
        if (left > 0) {
            memmove(s->buffer, s->buffer+s->buffer_start, (size_t) left);
        }
        s->buffer_start = 0;
        s->buffer_end = left;
        read(s);
        if (n >= s->buffer_end) {
            res.success = false;
            return res;
        }
    }
    res.success = true;
    res.c = s->buffer[s->buffer_start+n];
    return res;
}

static bool_char_t poll(char_stream_t *stream, int n) {
    bool_char_t res = peek(stream, n);
    if (!res.success) return res;
    file_char_stream_t * s = container_of(stream, file_char_stream_t, super);
    s->buffer_start += n+1;
    return res;
}

bool file_char_stream_init(file_char_stream_t * stream, const char * filename, int buffer_capacity) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) return false;

    char_stream_init(&stream->super, peek, poll);
    stream->file = file;
    stream->buffer = new_array(uint8_t, buffer_capacity);
    stream->buffer_capacity = buffer_capacity;
    stream->buffer_start = 0;
    stream->buffer_end = 0;
    return true;
}

bool file_char_stream_destroy(file_char_stream_t * stream) {
    free(stream->buffer);
    return fclose(stream->file) == 0;
}
