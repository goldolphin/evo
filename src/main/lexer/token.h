/**
 * @author goldolphin
 *         2015-08-27 10:00:00.
 */

#ifndef EVO_TOKEN_H
#define EVO_TOKEN_H

#include <stdint.h>
#include <utils/sbuilder.h>
#include <utils/string.h>

typedef enum {
#define TOKEN_DEF(a, b) a,
#include "token_define.def"
#undef TOKEN_DEF
} token_type_t;

typedef struct {
    token_type_t type;
    string_t value;
} token_t;

const char *token_type_name(token_type_t type);

const char *token_type_value(token_type_t type);

void token_init(token_t * token, token_type_t type, char *value, int value_len);

bool sbuilder_token(sbuilder_t * builder, token_t * token);

#endif //EVO_TOKEN_H
