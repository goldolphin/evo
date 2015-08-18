/**
 * @author goldolphin
 *         2015-08-17 11:40:40.
 */

#ifndef EVO_MATCHER_H
#define EVO_MATCHER_H

#include <stdbool.h>
#include "memory.h"

typedef struct {
    uint8_t *value;
    int value_len;
    void *extra;
} matcher_pattern_t;

struct matcher_state_s;
typedef struct matcher_state_s matcher_state_t;

typedef struct {
    matcher_state_t * state;
} matcher_context_t;

typedef struct {
    uint8_t * char2meta;
    int meta_count;
    matcher_state_t * start_state;
} matcher_t;

void matcher_pattern_init(matcher_pattern_t * pattern, uint8_t *value, int value_len, void *extra);

void matcher_init(matcher_t * matcher, matcher_pattern_t * patterns, int pattern_num);

void matcher_destroy(matcher_t * matcher);

void matcher_reset_context(matcher_t * matcher, matcher_context_t * context);

matcher_pattern_t * matcher_context_pattern(matcher_context_t * context);

bool matcher_match(matcher_t * matcher, matcher_context_t * context, uint8_t c);

matcher_pattern_t * matcher_match_seq(matcher_t * matcher, uint8_t * seq, int len);

#endif //EVO_MATCHER_H
