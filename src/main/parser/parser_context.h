/**
 * @author caofuxiang
 *         2016-01-19 10:57:57.
 */

#ifndef EVO_PARSER_CONTEXT_H
#define EVO_PARSER_CONTEXT_H

#include "module/module.h"

typedef struct {
    hashmap_t module_map; // name -> module
    module_t * current_module;
} parser_context_t;

void parser_context_init(parser_context_t * context, size_t initial_capacity);

void parser_context_destroy(parser_context_t * context);

bool parser_context_add_module(parser_context_t * context, string_t * name);

module_t * parser_context_get_module(parser_context_t * context, string_t * name);

static inline module_t * parser_context_current_module(parser_context_t * context) {
    return context->current_module;
}

static inline void parser_context_enter_scope(parser_context_t * context) {
    module_enter_scope(context->current_module);
}

static inline void parser_context_exit_scope(parser_context_t * context) {
    module_exit_scope(context->current_module);
}

#endif //EVO_PARSER_CONTEXT_H
