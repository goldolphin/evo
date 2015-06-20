/**
 * 
 * @author goldolphin
 *         2015-05-11 23:31
 */
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

typedef uint64_t unit_t;

#define BITS 64
#define ONE ((unit_t)1UL)

struct bitmap_s {
    size_t size;
    size_t n_units;
    unit_t buffer[0];
};

bitmap_t * make_bitmap(size_t size) {
    size_t n_units = (size+BITS-1)/BITS;
    bitmap_t * bitmap = (bitmap_t *)malloc(sizeof(bitmap_t) + n_units);
    bitmap->size = size;
    bitmap->n_units = n_units;
    memset(bitmap->buffer, 0, n_units);
    return bitmap;
}

void destroy_bitmap(bitmap_t * bitmap) {
    free(bitmap);
}

bool bitmap_set(bitmap_t *bitmap, size_t key) {
    if (key >= bitmap->size) return false;
    size_t n = key / BITS;
    size_t m = key % BITS;
    bitmap->buffer[n] |= (ONE << m);
    return true;
}

bool bitmap_unset(bitmap_t *bitmap, size_t key) {
    if (key >= bitmap->size) return false;
    size_t n = key / BITS;
    size_t m = key % BITS;
    bitmap->buffer[n] &= ~(ONE << m);
    return true;
}

bool bitmap_union(bitmap_t * bitmap, bitmap_t * src) {
    size_t n = bitmap->n_units < src->n_units ? bitmap->n_units : src->n_units;
    for (size_t i = 0; i < n; ++i) {
        bitmap->buffer[i] |= src->buffer[i];
    }
    size_t m = bitmap->size % BITS;
    bitmap->buffer[bitmap->n_units-1] &= (ONE << m) -1;
}

bool bitmap_isset(bitmap_t * bitmap, size_t key) {
    if (key >= bitmap->size) return false;
    size_t n = key / BITS;
    size_t m = key % BITS;
    return (bitmap->buffer[n] & (ONE << m)) != 0;
}
