/**
 * 
 * @author goldolphin
 *         2015-06-20 13:19
 */
#include "char_reader.h"

typedef struct {
    char_reader_t super;
    source_info_t source_info;
    FILE * file;
} file_reader_t;

error_t file_reader_read(file_reader_t * reader, char_t *buffer, size_t *size) {
    size_t read = fread(buffer, sizeof(char_t), *size, reader->file);
    if (read == 0 && ferror(reader->file)) {
        return errno;
    }
    *size = read;
    return EOK;
}

source_info_t * file_reader_source_info(file_reader_t * reader) {
    return &reader->source_info;
}

error_t file_reader_init(file_reader_t * reader, char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        return errno;
    }
    reader->super.read = (char_reader_read_t) file_reader_read;
    reader->super.source_info = (char_reader_source_info_t) file_reader_source_info;
    source_info_set(&reader->source_info, filename);
    reader->file = file;
    return EOK;
}