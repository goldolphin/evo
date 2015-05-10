/**
 * 
 * @author goldolphin
 *         2015-05-02 12:49
 */
#ifndef EVO_TOKEN_H
#define EVO_TOKEN_H

#include "source_info.h"
#include "character.h"
#include "../memory/memory.h"

typedef enum {
    PREDEFINED,
    LITERAL,
    IDENTIFIER,
} token_type;

typedef struct {
    token_type type;
    char_t * value;
} token_t;

typedef struct {
    token_t * token;
    source_info_t * source;
    int line;
    int column;
} token_info_t;

static inline token_info_t * make_token_info(token_t * token, source_info_t * source, int line, int column) {
    token_info_t *info = new_data(token_info_t);
    info->token = token;
    info->source = source;
    info->line = line;
    info->column = column;
    return info;
}

extern token_info_t token_end;
#define TOKEN_END (&token_end)
#endif //EVO_TOKEN_H
