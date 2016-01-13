/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#include "type.h"

#define DEFINE_TYPE(var, id, name) \
static string_t s_##var##_name = STRING_LITERAL(name); \
static type_t s_##var = {id, &s_##var##_name}; \
type_t * var = &s_##var;

DEFINE_TYPE(UNIT_TYPE, TI_UNIT, "Unit")
DEFINE_TYPE(BOOL_TYPE, TI_BOOL, "Bool")
DEFINE_TYPE(INT_TYPE, TI_INT, "Int")
DEFINE_TYPE(LONG_TYPE, TI_LONG, "Long")
DEFINE_TYPE(DOUBLE_TYPE, TI_DOUBLE, "Double")
DEFINE_TYPE(STRING_TYPE, TI_STRING, "String")
DEFINE_TYPE(FUN_TYPE, TI_FUN, "Fun")

void type_init(type_t * type, int id, string_t * name) {
    type->id = id;
    type->name = name;
}

void type_copy(type_t * type, type_t * other) {
    type_init(type, other->id, other->name);
}

bool sbuilder_type(sbuilder_t * builder, type_t * type) {
    if (type == NULL) {
        return sbuilder_str(builder, "TYPE_NULL");
    } else {
        sbuilder_format(builder, "type(%d, ", type->id);
        sbuilder_string(builder, type->name);
        return sbuilder_str(builder, ")");
    }
}
