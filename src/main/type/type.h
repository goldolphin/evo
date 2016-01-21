/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#ifndef EVO_TYPE_H
#define EVO_TYPE_H

#include <utils/string.h>

typedef enum {
    // Incomplete type
    TC_HOLDER,

    // Primitive types
    TC_UNIT,
    TC_BOOL,
    TC_INT,
    TC_LONG,
    TC_DOUBLE,
    TC_STRING,

    // Function types
    TC_FUN,

    // Product types
    TC_STRUCT,
} type_category_t;

/**
 * type_info_t
 */
typedef struct {
    type_category_t category;
    string_t * name;
} type_info_t;

static inline void type_info_init(type_info_t * type_info, type_category_t category, string_t * name) {
    type_info->category = category;
    type_info->name = name;
}

/**
 * type_t
 */
typedef struct {
    type_info_t * info;
} type_t;

static inline void type_init(type_t * type, type_info_t * info) {
    type->info = info;
}

static inline void type_copy(type_t * type, type_t * other) {
    type_init(type, other->info);
}

bool sbuilder_type(sbuilder_t * builder, type_t * type);

/**
 * Constant type infos.
 */
extern type_info_t * TYPE_INFO_HOLDER;
extern type_info_t * TYPE_INFO_UNIT;
extern type_info_t * TYPE_INFO_BOOL;
extern type_info_t * TYPE_INFO_INT;
extern type_info_t * TYPE_INFO_LONG;
extern type_info_t * TYPE_INFO_DOUBLE;
extern type_info_t * TYPE_INFO_STRING;
extern type_info_t * TYPE_INFO_FUN;

#endif //EVO_TYPE_H
