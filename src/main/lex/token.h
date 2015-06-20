/**
 * 
 * @author goldolphin
 *         2015-05-02 12:49
 */
#ifndef EVO_TOKEN_H
#define EVO_TOKEN_H

#include "source_info.h"
#include "character.h"

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

void token_init(token_t * token, token_type type, char_t * value);

void token_init_predefined(token_t * token, char_t * value);

static inline void token_info_init(token_info_t * info, token_t * token, source_info_t * source, int line, int column) {
    info->token = token;
    info->source = source;
    info->line = line;
    info->column = column;
}

extern token_info_t token_end;
#define TOKEN_END (&token_end)
#endif //EVO_TOKEN_H
