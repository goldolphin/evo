/**
 * @author caofuxiang
 *         2015-08-19 20:44:44.
 */

#ifndef EVO_LEXER_H
#define EVO_LEXER_H

#include <utils/matcher.h>

#define MAX_TOKEN_LEN 1024

typedef struct {
    matcher_t keyword_matcher;
} lexer_t;

#endif //EVO_LEXER_H
