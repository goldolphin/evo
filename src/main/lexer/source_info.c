/**
 * @author goldolphin
 *         2015-09-08 11:25:25.
 */

#include "source_info.h"
#include "character.h"
#include <utils/memory.h>

void source_info_init(source_info_t * info, int buffer_size) {
    info->line = 1;
    info->column = 0;
    sbuilder_init(&info->buffer, new_array(char, buffer_size), (size_t) buffer_size);
}

void source_info_destroy(source_info_t * info) {
    free(info->buffer.buf);
}

void source_info_update(source_info_t * info, uint8_t c) {
    if (is_linebreak(c)) {
        info->line ++;
        info->column = 0;
        sbuilder_reset(&info->buffer);
    } else {
        info->column ++;
        sbuilder_char(&info->buffer, c);
    }
}
