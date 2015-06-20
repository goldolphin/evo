/**
 * 
 * @author goldolphin
 *         2015-05-11 23:31
 */
#ifndef EVO_BITMAP_H
#define EVO_BITMAP_H

#include <stddef.h>
#include <stdbool.h>

struct bitmap_s;
typedef struct bitmap_s bitmap_t;

bitmap_t * make_bitmap(size_t size);

void destroy_bitmap(bitmap_t * bitmap);

bool bitmap_set(bitmap_t *bitmap, size_t key);

bool bitmap_unset(bitmap_t *bitmap, size_t key);

bool bitmap_union(bitmap_t * bitmap, bitmap_t * src);

bool bitmap_isset(bitmap_t * bitmap, size_t key);

#endif //EVO_BITMAP_H
