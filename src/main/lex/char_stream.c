/**
 * 
 * @author goldolphin
 *         2015-05-02 22:07
 */
#include "char_stream.h"
#include "source_info.h"
#include "../memory/memory.h"

typedef struct {
    stream_t parent;
    source_info_t * source_info;
    FILE * file;
    char_t head;
    bool cached;
} file_stream_t;

error_t file_stream_peek(stream_t * s, char_t *c) {
    file_stream_t * fs = (file_stream_t *)s;
    if (!fs->cached) {
        int i = fgetc(fs->file);
        if (i == EOF) {
            if (ferror(fs->file)) {
                return errno;
            }
            fs->head = CHAR_END;
        } else {
            fs->head = (char_t)i;
        }
        fs->cached = true;
    }
    *c = fs->head;
    return EOK;
}

error_t file_stream_poll(stream_t * s, char_t *c) {
    error_t error = file_stream_peek(s, c);
    if (error != EOK) {
        return error;
    }
    file_stream_t * fs = (file_stream_t *)s;
    fs->cached = false;
    return EOK;
}

source_info_t * file_stream_source_info(stream_t * stream) {
    file_stream_t * fs = (file_stream_t *)stream;
    return fs->source_info;
}

stream_t * make_file_stream(char * filename) {
    file_stream_t * fs = new_data(file_stream_t);
    fs->parent.poll = file_stream_poll;
    fs->parent.source_info = file_stream_source_info;
    fs->source_info = make_source_info(filename);
    fs->file = fopen(filename, "r");
    fs->head = CHAR_END;
    fs->cached = false;
    return (stream_t *)fs;
}

