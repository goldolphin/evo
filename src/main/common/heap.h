/**
 * 
 * @author goldolphin
 *         2015-05-16 12:43
 */
#ifndef EVO_HEAP_H
#define EVO_HEAP_H

#include <stddef.h>
#include <stdbool.h>

struct heap_s;
typedef struct heap_s heap_t;

heap_t * make_heap(size_t capacity);

void destroy_heap(heap_t * heap);

size_t heap_capacity(heap_t * heap);

size_t heap_size(heap_t * heap);

bool heap_head(heap_t * heap, long *key, void ** value);

bool heap_push(heap_t * heap, long key, void * value);

bool heap_pop(heap_t * heap, long *key /* Could be NULL */, void ** value /* Could be NULL */);

#endif //EVO_HEAP_H
