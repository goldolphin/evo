/**
 * @author goldolphin
 *         2015-08-21 14:05:05.
 */

#ifndef EVO_PAIR_H
#define EVO_PAIR_H

typedef struct {
    void * key;
    void * value;
} pair_t;

static inline void pair_set(pair_t * pair, void * key, void * value) {
    pair->key = key;
    pair->value = value;
}

static inline void pair_copy(pair_t * dst, pair_t * src) {
    pair_set(dst, src->key, src->value);
}

#endif //EVO_PAIR_H
