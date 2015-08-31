/**
 * @author goldolphin
 *         2015-08-31 15:43:43.
 */

#include "char_stream.h"

void char_stream_init(char_stream_t *stream, char_stream_peek_t peek, char_stream_poll_t poll) {
    stream->peek = peek;
    stream->poll = poll;
}