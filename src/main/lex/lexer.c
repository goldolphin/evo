/**
 * 
 * @author goldolphin
 *         2015-05-02 14:08
 */
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "matcher.h"

#define MAX_TOKEN_LEN 1024

typedef struct {
    char_t buffer[MAX_TOKEN_LEN];
    int length;
} buffer_t;

typedef struct {
    pattern_t * predefined;
    int predefined_len;
    matcher_t predefined_matcher;
} lexer_t;
