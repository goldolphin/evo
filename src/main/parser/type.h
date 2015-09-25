/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#ifndef EVO_TYPE_H
#define EVO_TYPE_H

typedef enum {
    TC_BOOL,
    TC_LONG,
    TC_DOUBLE,
    TC_STRUCT,
    TC_FUN,
} type_category_t;

typedef struct {
    type_category_t category;
} type_t;

typedef struct {
    int num;
    type_t ** types;
} type_list_t;

typedef struct {
    type_t super;
    type_list_t members;
} type_struct_t;

typedef struct {
    type_t super;
    type_list_t params;
    type_t * return_type;
} type_fun_t;

#endif //EVO_TYPE_H
