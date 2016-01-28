/**
 * @author caofuxiang
 *         2016-01-28 11:28:28.
 */

#ifndef EVO_PS_SCOPE_H
#define EVO_PS_SCOPE_H

#include <utils/hashmap.h>

typedef struct {
    hashmap_t local_map;    // name -> index
} ps_scope_t;

#endif //EVO_PS_SCOPE_H
