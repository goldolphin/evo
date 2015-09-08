/**
 * @author goldolphin
 *         2015-09-08 16:09:09.
 */

#ifndef EVO_INFIX_PARSER_H
#define EVO_INFIX_PARSER_H

#include <utils/matcher.h>
#include <utils/string.h>

typedef struct {
    string_t name;
    bool left2right;
    int precedence;
} infix_def_t;

typedef struct {
    matcher_t infix_def_map;
} infix_parser_t;

#endif //EVO_INFIX_PARSER_H
