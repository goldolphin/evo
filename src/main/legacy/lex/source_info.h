/**
 * 
 * @author goldolphin
 *         2015-05-02 14:10
 */
#ifndef EVO_SOURCE_INFO_H
#define EVO_SOURCE_INFO_H

typedef struct {
    char * name;
} source_info_t;


void source_info_set(source_info_t * info, char * name);

#endif //EVO_SOURCE_INFO_H
