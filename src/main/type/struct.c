/**
 * @author caofuxiang
 *         2016-01-12 17:41:41.
 */

#include "struct.h"

void struct_info_init(struct_info_t * struct_info, string_t * name, struct_field_t * fields, int fields_num) {
    type_info_init(&struct_info->super, TC_STRUCT, name);
    struct_info->fields = fields;
    struct_info->fields_num = fields_num;
}

int struct_info_field_index(struct_info_t * struct_info, string_t * name) {
    for (int i = 0; i < struct_info->fields_num; ++i) {
        if (string_equals(name, struct_info->fields[i].name)) {
            return i;
        }
    }
    return -1;
}

struct_field_t * struct_info_field(struct_info_t * struct_info, int index) {
    if (index >= 0 && index < struct_info->fields_num) {
        return &struct_info->fields[index];
    }
    return NULL;
}

bool sbuilder_struct_info(sbuilder_t * builder, struct_info_t * struct_info) {
    sbuilder_str(builder, "struct ");
    sbuilder_string(builder, struct_info->super.name);
    sbuilder_str(builder, "(");
    for (int i = 0; i < struct_info->fields_num; ++i) {
        if (i > 0) {
            sbuilder_str(builder, ", ");
        }
        sbuilder_string(builder, struct_info->fields[i].name);
        sbuilder_str(builder, ":");
        sbuilder_type(builder, struct_info->fields[i].type);
    }
    return sbuilder_str(builder, ")");
}
