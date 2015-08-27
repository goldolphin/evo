/**
 * @author caofuxiang
 *         2015-08-27 10:00:00.
 */

#ifndef EVO_TOKEN_H
#define EVO_TOKEN_H

#include <stdint.h>

typedef enum {
    TOKEN_ID,

    // Literals
    TOKEN_NUMBER,
    TOKEN_STRING,

    // Keywords
    TOKEN_IMPORT,
    TOKEN_LET,
    TOKEN_STRUCT,
    TOKEN_EXTENDS,
    TOKEN_FUN,

    // Punctuations
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_PERIOD,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_GRAVE,
} token_type_t;

typedef struct {
    token_type_t type;
    uint8_t *value;
    int value_len;
} token_t;

void token_init(token_t * token, token_type_t type, uint8_t *value, int value_len);

#endif //EVO_TOKEN_H
