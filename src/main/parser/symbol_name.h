/**
 * @author caofuxiang
 *         2016-02-01 16:38:38.
 */

#ifndef EVO_SYMBOL_NAME_H
#define EVO_SYMBOL_NAME_H

#include <utils/string.h>

typedef struct {
    string_t * module_name;
    string_t * name;
} symbol_name_t;

void symbol_name_init(symbol_name_t * symbol_name, string_t * module_name, string_t * name);

void symbol_name_destroy(symbol_name_t * symbol_name);

symbol_name_t * symbol_name_new(string_t * module_name, string_t * name);

void symbol_name_free(symbol_name_t * symbol_name);

bool symbol_name_equals(symbol_name_t * name1, symbol_name_t * name2);

size_t symbol_name_hash_func(void * key);

bool symbol_name_equal_func (void * key1, void * key2);

#endif //EVO_SYMBOL_NAME_H
