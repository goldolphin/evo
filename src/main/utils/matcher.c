/**
 * @author goldolphin
 *         2015-08-17 11:40:40.
 */

#include "matcher.h"

#define CHAR_COUNT 256
#define META_STOP ((uint8_t)0xFF)

struct matcher_state_s {
    matcher_pattern_t * pattern;
    matcher_state_t ** nexts;
};

static int char2meta_init(uint8_t * char2meta, matcher_pattern_t * patterns, int pattern_num) {
    for (int i = 0; i < CHAR_COUNT; ++i) {
        char2meta[i] = META_STOP;
    }
    int meta_count = 0;
    for (int i = 0; i < pattern_num; ++i) {
        matcher_pattern_t * pattern = &patterns[i];
        for (int j = 0; j < pattern->value_len; ++j) {
            uint8_t c = pattern->value[j];
            if (char2meta[c] == META_STOP) {
                char2meta[c] = (uint8_t) meta_count++;
            }
        }
    }
    return meta_count;
}

static matcher_state_t * matcher_new_state(matcher_t *matcher) {
    matcher_state_t * state = new_data(matcher_state_t);
    state->pattern = NULL;
    state->nexts = new_array(matcher_state_t*, matcher->meta_count);
    zero_array(state->nexts, matcher->meta_count);
    return state;
}

static void matcher_free_state(matcher_t *matcher, matcher_state_t *state) {
    if (state == NULL) {
        return;
    }
    for (int i = 0; i < matcher->meta_count; ++i) {
        matcher_free_state(matcher, state->nexts[i]);
    }
    free(state->nexts);
    free(state);
}

#define CHAR2META(c) (matcher->char2meta[c])

void matcher_pattern_init(matcher_pattern_t * pattern, uint8_t *value, int value_len, void *extra) {
    pattern->value = value;
    pattern->value_len = value_len;
    pattern->extra = extra;
}

void matcher_init(matcher_t * matcher, matcher_pattern_t * patterns, int pattern_num) {
    matcher->char2meta = new_array(uint8_t, CHAR_COUNT);
    matcher->meta_count = char2meta_init(matcher->char2meta, patterns, pattern_num);
    matcher->start_state = matcher_new_state(matcher);

    for (int i = 0; i < pattern_num; ++i) {
        matcher_state_t * state = matcher->start_state;
        matcher_pattern_t * pattern = &patterns[i];
        for (int j = 0; j < pattern->value_len; ++j) {
            uint8_t m = CHAR2META(pattern->value[j]);
            if (state->nexts[m] == NULL) {
                state->nexts[m] = matcher_new_state(matcher);
            }
            state = state->nexts[m];
        }
        state->pattern = pattern;
    }
}

void matcher_destroy(matcher_t * matcher) {
    free(matcher->char2meta);
    matcher_free_state(matcher, matcher->start_state);
}

void matcher_reset_context(matcher_t * matcher, matcher_context_t * context) {
    context->state = matcher->start_state;
}

matcher_pattern_t * matcher_context_pattern(matcher_context_t * context) {
    return context->state->pattern;
}

bool matcher_match(matcher_t * matcher, matcher_context_t * context, uint8_t c) {
    uint8_t m = CHAR2META(c);
    if (m == META_STOP) {
        return false;
    }
    matcher_state_t * next = context->state->nexts[m];
    if (next == NULL) {
        return false;
    }
    context->state = next;
    return true;
}

matcher_pattern_t * matcher_match_seq(matcher_t * matcher, uint8_t * seq, int len) {
    matcher_context_t context;
    matcher_reset_context(matcher, &context);
    for (int i = 0; i < len; ++i) {
        if (!matcher_match(matcher, &context, seq[i])) {
            return NULL;
        }
    }
    return matcher_context_pattern(&context);
}
