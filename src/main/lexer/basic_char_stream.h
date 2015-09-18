/**
 * @author caofuxiang
 *         2015-09-15 10:39:39.
 */

#ifndef EVO_BASIC_CHAR_STREAM_H
#define EVO_BASIC_CHAR_STREAM_H

#include "char_stream.h"

typedef struct {
    char_stream_t super;
    uint8_t * buffer;
    int buffer_start;
    int buffer_end;
} basic_char_stream_t;

void basic_char_stream_init(basic_char_stream_t * stream, uint8_t * buffer, int buffer_size);

#endif //EVO_BASIC_CHAR_STREAM_H
