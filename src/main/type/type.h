/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#ifndef EVO_TYPE_H
#define EVO_TYPE_H

#include <utils/string.h>

typedef enum {
    TC_PRIMITIVE,
    TC_FUN,
    TC_STRUCT,
} type_category_t;

typedef enum {
    // Primitive types
    TI_UNIT,
    TI_BOOL,
    TI_INT,
    TI_LONG,
    TI_DOUBLE,
    TI_STRING,
    TI_FUN,

    TYPE_ID_PRIMITIVE_NUM = 16,
};

typedef struct {
    int id;
    string_t * name;
} type_t;

typedef struct {
    type_category_t category;
} type_info_t;

extern type_t * UNIT_TYPE;
extern type_t * BOOL_TYPE;
extern type_t * INT_TYPE;
extern type_t * LONG_TYPE;
extern type_t * DOUBLE_TYPE;
extern type_t * STRING_TYPE;
extern type_t * FUN_TYPE;

void type_init(type_t * type, int id, string_t * name);

void type_copy(type_t * type, type_t * other);

bool sbuilder_type(sbuilder_t * builder, type_t * type);

static inline void type_info_init(type_info_t * type_info, type_category_t category) {
    type_info->category = category;
}

#endif //EVO_TYPE_H
