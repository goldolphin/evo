/**
 * 
 * @author goldolphin
 *         2015-05-02 14:10
 */
#include "source_info.h"
#include "../memory/memory.h"

source_info_t * make_source_info(char * name) {
    source_info_t * info = new_data(source_info_t);
    info->name = name;
    return info;
}

