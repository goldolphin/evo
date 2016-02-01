/**
 * @author caofuxiang
 *         2016-01-29 16:47:47.
 */

#ifndef EVO_PS_CONTEXT_H
#define EVO_PS_CONTEXT_H

#include <utils/list.h>
#include "ps_module.h"

typedef struct {
    hashmap_t module_map; // name -> module
    list_t imported_modules;
    ps_module_t * current_module;
} ps_context_t;

#endif //EVO_PS_CONTEXT_H
