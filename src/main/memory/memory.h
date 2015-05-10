/**
 * 
 * @author goldolphin
 *         2015-05-02 23:16
 */
#ifndef EVO_MEMORY_H
#define EVO_MEMORY_H

#include <stdlib.h>

#define new_data(type) ((type *)malloc(sizeof type))

#define new_array(type, size) ((type *)malloc(sizeof(type) * (size)))

#define delete(p) free(p)

#endif //EVO_MEMORY_H
