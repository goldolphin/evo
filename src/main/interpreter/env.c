/**
 * @author goldolphin
 *         2015-09-18 15:48:48.
 */

#include "env.h"

typedef struct {
    size_t last_frame_base;
} frame_t;

void env_init(env_t * env, size_t stack_size) {
    env->stack_size = stack_size;
    env->stack = new_array(char, stack_size);
    env->top = 0;
    env->frame_base = 0;
    env->level = 0;
    env_push_frame(env);
}

void env_destroy(env_t * env) {
    free(env->stack);
}

void env_push_frame(env_t * env) {
    ensure(sizeof(frame_t) + env->top < env->stack_size);
    frame_t * frame = (frame_t *) &env->stack[env->top];
    frame->last_frame_base = env->frame_base;
    env->frame_base = env->top;
    env->top += sizeof(frame_t);
    ++ env->level;
}

void env_pop_frame(env_t * env) {
    ensure(env->frame_base > 0);
    frame_t * frame = (frame_t *) &env->stack[env->frame_base];
    env->frame_base = frame->last_frame_base;
    env->top = env->frame_base;
    -- env->level;
}
