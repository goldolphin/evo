/**
 * @author caofuxiang
 *         2016-01-27 20:06:06.
 */

#ifndef EVO_PS_FRAME_H
#define EVO_PS_FRAME_H

#include <utils/hashmap.h>

typedef struct {
    hashmap_t captured_map;  // name -> (l/c, outer_index, capture_index)
} ps_frame_t;
#endif //EVO_PS_FRAME_H
