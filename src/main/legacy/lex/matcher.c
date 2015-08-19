/**
 * 
 * @author goldolphin
 *         2015-06-21 21:54
 */
#include <string.h>
#include "matcher.h"

void matcher_init(matcher_t * matcher, char_t * char2meta, state_id_t * transitions, int meta_num, int state_num,
                  pattern_t * patterns, int pattern_num) {
    matcher->char2meta = char2meta;
    matcher->transitions = transitions;
    matcher->meta_num = meta_num;
    matcher->state_num = state_num;
    matcher->patterns = patterns;
    matcher->pattern_num = pattern_num;
}

void matcher_init_char2meta(char_t * char2meta /* out */, int * meta_num /* out */, int * max_state_num /* out */,
                            pattern_t * patterns, int pattern_num) {
    memset(char2meta, META_NULL, CHAR_COUNT);
    int meta_count = 1;
    int total_length = 0;
    for (int i = 0; i < pattern_num; ++i) {
        for (int j = 0; ; ++j) {
            char_t c = patterns[i].value[j];
            if (c == CHAR_NULL) {
                break;
            }
            total_length ++;
            if (char2meta[c] == META_NULL) {
                char2meta[c] = meta_count++;
            }
        }
    }
    *meta_num = meta_count;
    *max_state_num = total_length+1;
}

bool matcher_init_transitions(state_id_t * transitions /* out */, int * state_num /* inout */,
                              char_t * char2meta, int meta_num,
                              pattern_t * patterns, int pattern_num) {

    memset(transitions, STATE_REJECT, meta_num*(*state_num)*sizeof(state_id_t));
    state_id_t start_state = 0;
    state_id_t state_count = 1;
    for (int i = 0; i < pattern_num; ++i) {
        state_id_t state = start_state;
        for (int j = 0; ; ++j) {
            char_t c = patterns[i].value[j];
            char_t m = char2meta[c];
            state_id_t * next = &transitions[state*meta_num+m];
            if (m == META_NULL) {
                *next = ACCEPT_BASE-i;
                break;
            } else if (*next == STATE_REJECT) {
                if (state_count > ACCEPT_BASE-pattern_num) {
                    return false;
                }
                *next = state_count ++;
            }
            state = *next;
        }
    }
    *state_num = state_count;
    return true;
}

pattern_t * matcher_match(matcher_t * matcher, state_id_t * state /* inout */, char_t c) {
    char_t m = matcher->char2meta[c];
    *state = matcher->transitions[(*state)*matcher->meta_num+m];
    int i = ACCEPT_BASE - *state;
    if (i < matcher->pattern_num) {
        return matcher->patterns[i];
    }
    return NULL;
}
