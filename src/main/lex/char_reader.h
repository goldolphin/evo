/**
 * 
 * @author goldolphin
 *         2015-06-20 13:19
 */
#ifndef EVO_CHAR_READER_H
#define EVO_CHAR_READER_H

#include <stdio.h>
#include "source_info.h"
#include "character.h"

struct char_reader_s;
typedef struct char_reader_s char_reader_t;

typedef error_t (* char_reader_read_t) (char_reader_t * reader, char_t *buffer, size_t *size);
typedef source_info_t * (* char_reader_source_info_t) (char_reader_t * reader);

struct char_reader_s {
    char_reader_read_t read;
    char_reader_source_info_t source_info;
};

static inline error_t char_reader_read(char_reader_t * reader, char_t *buffer, size_t *size) {
    return reader->read(reader, buffer, size);
}

static inline source_info_t * char_reader_source_info(char_reader_t * reader) {
    return reader->source_info(reader);
}

#endif //EVO_CHAR_READER_H
