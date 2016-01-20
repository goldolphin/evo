/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#include "type.h"

#define DEFINE_TYPE(var, category, name) \
static string_t s_##var##_name = STRING(name); \
static type_info_t s_##var = {category, &s_##var##_name}; \
type_info_t * var = &s_##var;

DEFINE_TYPE(TYPE_INFO_HOLDER, TC_HOLDER, "Holder")
DEFINE_TYPE(TYPE_INFO_UNIT, TC_UNIT, "Unit")
DEFINE_TYPE(TYPE_INFO_BOOL, TC_BOOL, "Bool")
DEFINE_TYPE(TYPE_INFO_INT, TC_INT, "Int")
DEFINE_TYPE(TYPE_INFO_LONG, TC_LONG, "Long")
DEFINE_TYPE(TYPE_INFO_DOUBLE, TC_DOUBLE, "Double")
DEFINE_TYPE(TYPE_INFO_STRING, TC_STRING, "String")
DEFINE_TYPE(TYPE_INFO_FUN, TC_FUN, "Fun")

bool sbuilder_type(sbuilder_t * builder, type_t * type) {
    if (type == NULL) {
        return sbuilder_str(builder, "TYPE_NULL");
    } else {
        sbuilder_format(builder, "type(%d, ", type->info->category);
        sbuilder_string(builder, type->info->name);
        return sbuilder_str(builder, ")");
    }
}
