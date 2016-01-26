/**
 * @author goldolphin
 *         2015-09-18 15:48:48.
 */

#ifndef EVO_ENV_H
#define EVO_ENV_H

#include <utils/memory.h>

typedef struct {
    size_t stack_size;
    char * stack;
    size_t top;
    size_t frame_base;
    int level;
} env_t;

void env_init(env_t * env, size_t stack_size);

void env_destroy(env_t * env);

void env_push_frame(env_t * env);

void env_pop_frame(env_t * env);

#endif //EVO_ENV_H
