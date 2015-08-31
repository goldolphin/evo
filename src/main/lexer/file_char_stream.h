/**
 * @author goldolphin
 *         2015-08-31 15:58:58.
 */

#ifndef EVO_FILE_CHAR_STREAM_H
#define EVO_FILE_CHAR_STREAM_H

#include <stdio.h>
#include "char_stream.h"

typedef struct {
    char_stream_t super;
    FILE * file;
    uint8_t * buffer;
    int buffer_capacity;
    int buffer_start;
    int buffer_end;
} file_char_stream_t;

bool file_char_stream_init(file_char_stream_t * stream, const char * filename, int buffer_capacity);

bool file_char_stream_destroy(file_char_stream_t * stream);

#endif //EVO_FILE_CHAR_STREAM_H
