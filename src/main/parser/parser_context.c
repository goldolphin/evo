/**
 * @author caofuxiang
 *         2016-01-19 10:57:57.
 */

#include <utils/pair.h>
#include "parser_context.h"

static void init_prelude(module_t * module) {
    module_define_type(module, TYPE_INFO_UNIT);
    module_define_type(module, TYPE_INFO_BOOL);
    module_define_type(module, TYPE_INFO_INT);
    module_define_type(module, TYPE_INFO_LONG);
    module_define_type(module, TYPE_INFO_DOUBLE);
    module_define_type(module, TYPE_INFO_STRING);
    module_define_type(module, TYPE_INFO_STRING);

    module_define_var(module, STRING("__ne"), NULL);
    module_define_binary(module, STRING("!="), true, 5, STRING("__ne"));

    module_define_var(module, STRING("__eq"), NULL);
    module_define_binary(module, STRING("=="), true, 5, STRING("__eq"));

    module_define_var(module, STRING("__or"), NULL);
    module_define_binary(module, STRING("||"), true, 5, STRING("__or"));

    module_define_var(module, STRING("__lt"), NULL);
    module_define_binary(module, STRING("<"), true, 5, STRING("__lt"));

    module_define_var(module, STRING("__ge"), NULL);
    module_define_binary(module, STRING(">="), true, 5, STRING("__ge"));

    module_define_var(module, STRING("__re"), NULL);
    module_define_binary(module, STRING("%"), true, 5, STRING("__re"));

    module_define_var(module, STRING("__add"), NULL);
    module_define_binary(module, STRING("+"), true, 5, STRING("__add"));

    module_define_var(module, STRING("__sub"), NULL);
    module_define_binary(module, STRING("-"), true, 5, STRING("__sub"));

    module_define_var(module, STRING("__mul"), NULL);
    module_define_binary(module, STRING("*"), true, 4, STRING("__mul"));

    module_define_var(module, STRING("__neg"), NULL);
    module_define_prefix(module, STRING("-"), 2, STRING("__neg"));

    module_define_var(module, STRING("printf"), NULL);
    module_define_var(module, STRING("printfn"), NULL);
    module_define_var(module, STRING("if"), NULL);
    module_define_var(module, STRING("rand"), NULL);
}

void parser_context_init(parser_context_t * context, size_t initial_capacity) {
    hashmap_init1(&context->module_map, initial_capacity, string_hash_func, string_equal_func);
    context->current_module = NULL;
    ensure(parser_context_add_module(context, STRING("prelude")));
    init_prelude(context->current_module);
}

void parser_context_destroy(parser_context_t * context) {
    hashmap_destroy(&context->module_map);
}

bool parser_context_add_module(parser_context_t * context, string_t * name) {
    pair_t kv;
    if (hashmap_get(&context->module_map, name, &kv)) {
        return false;
    }
    module_t * module = new_data(module_t);
    module_init(module);
    ensure(hashmap_put(&context->module_map, string_dup(name), module) != hashmap_end(&context->module_map));
    context->current_module = module;
    return true;
}

module_t * parser_context_get_module(parser_context_t * context, string_t * name) {
    pair_t kv;
    if (hashmap_get(&context->module_map, name, &kv)) {
        return kv.value;
    }
    return NULL;
}
