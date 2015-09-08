/**
 * @author goldolphin
 *         2015-09-08 11:25:25.
 */

#ifndef EVO_SOURCE_INFO_H
#define EVO_SOURCE_INFO_H

#include <utils/sbuilder.h>

typedef struct {
    int line;
    int column;
    sbuilder_t buffer;
}source_info_t;

void source_info_init(source_info_t * info, int buffer_size);

void source_info_destroy(source_info_t * info);

void source_info_update(source_info_t * info, uint8_t c);

#endif //EVO_SOURCE_INFO_H
