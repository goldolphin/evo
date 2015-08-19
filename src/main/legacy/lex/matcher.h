/**
 * 
 * @author goldolphin
 *         2015-06-21 21:54
 */
#ifndef EVO_MATCHER_H
#define EVO_MATCHER_H

#include "character.h"

#define META_NULL ((char_t)0)

typedef unsigned char state_id_t;
const state_id_t STATE_REJECT = 255;
const state_id_t ACCEPT_BASE = 254;

typedef struct {
    char_t * value;
    void * data;
} pattern_t;

typedef struct {
    char_t * char2meta;
    state_id_t * transitions;
    int meta_num;
    int state_num;
    pattern_t * patterns;
    int pattern_num;
} matcher_t;

static inline void pattern_init(pattern_t * pattern, char_t * value, void * data) {
    pattern->value = value;
    pattern->data = data;
}

void matcher_init(matcher_t * matcher, char_t * char2meta, state_id_t * transitions, int meta_num, int state_num,
                  pattern_t * patterns, int pattern_num);

void matcher_init_char2meta(char_t * char2meta /* out */, int * meta_num /* out */, int * max_state_num /* out */,
                            pattern_t * patterns, int pattern_num);

bool matcher_init_transitions(state_id_t * transitions /* out */, int * state_num /* inout */,
                              char_t * char2meta, int meta_num,
                              pattern_t * patterns, int pattern_num);

pattern_t * matcher_match(matcher_t * matcher, state_id_t * state /* inout */, char_t c);

#endif //EVO_MATCHER_H
