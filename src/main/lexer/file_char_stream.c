/**
 * @author goldolphin
 *         2015-08-31 15:58:58.
 */

#include <utils/memory.h>
#include "file_char_stream.h"

static bool_char_t peek(char_stream_t *stream) {
    file_char_stream_t * s = container_of(stream, file_char_stream_t, super);
    bool_char_t res;
    if (s->buffer_start >= s->buffer_end) {
        s->buffer_start = 0;
        s->buffer_end = (int) fread(s->buffer, 1, (size_t) s->buffer_capacity, s->file);
        if (s->buffer_start >= s->buffer_end) {
            res.success = false;
            return res;
        }
    }
    res.success = true;
    res.c = s->buffer[s->buffer_start];
    return res;
}

static bool_char_t poll(char_stream_t *stream) {
    bool_char_t res = peek(stream);
    if (!res.success) return res;
    file_char_stream_t * s = container_of(stream, file_char_stream_t, super);
    ++ s->buffer_start;
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
