/**
 * @author caofuxiang
 *         2016-01-12 17:41:41.
 */

#ifndef EVO_STRUCT_H
#define EVO_STRUCT_H

#include "type.h"

typedef struct {
    string_t * name;
    type_t type;
} struct_field_t;

typedef struct {
    type_info_t super;
    struct_field_t * fields;
    int fields_num;
} struct_info_t;

void struct_info_init(struct_info_t * struct_info, struct_field_t * fields, int fields_num);

int struct_info_field_index(struct_info_t * struct_info, string_t * name);

struct_field_t * struct_info_field(struct_info_t * struct_info, int index);

bool sbuilder_struct_info(sbuilder_t * builder, struct_info_t * struct_info);

#endif //EVO_STRUCT_H
