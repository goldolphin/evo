/**
 * 
 * @author goldolphin
 *         2015-05-02 14:08
 */
#include <string.h>
#include "lexer.h"
#include "token.h"
#include "char_stream.h"

#define CHAR_COUNT 256

#define META_NULL ((char_t)255)
#define REJECT -1

#define MAX_TOKEN_LEN 1024

typedef struct state_s {
    struct state_s ** nexts;
    int accept;
} state_t;

state_t * make_state(int next_num, int accept) {
    state_t * s = new_data(state_t);
    s->nexts = new_array(state_t*, next_num);
    memset(s->nexts, NULL, next_num);
    s->accept = accept;
    return s;
}

typedef struct {
    token_t * predefined;
    int predefined_len;
    char_t char_to_meta[CHAR_COUNT];
    char_t meta_num;
    state_t * start_state;
    int state_num;
} lexer_t;

typedef struct {
    lexer_t * lexer;
    stream_t * stream;
    state_t * current;
    char_t buffer[MAX_TOKEN_LEN];
    int buffer_pos;
} tokenizer_t;

lexer_t * make_lexer(token_t * predefined, int predefined_len) {
    lexer_t * lexer = new_data(lexer_t);
    lexer->predefined = predefined;
    lexer->predefined_len = predefined_len;

    memset(lexer->char_to_meta, META_NULL, CHAR_COUNT);
    char_t meta_num = 0;
    int max_len = 0;
    for (int i = 0; i < predefined_len; ++i) {
        for (int j = 0; ; ++j) {
            char_t c = predefined[i].value[j];
            if (c == CHAR_END) {
                if (max_len < j) max_len = j;
                break;
            }
            if (lexer->char_to_meta[c] == META_NULL) {
                lexer->char_to_meta[c] = meta_num++;
            }
        }
    }
    lexer->meta_num = meta_num;

    lexer->start_state = make_state(meta_num, REJECT);
    int state_num = 0;
    for (int i = 0; i < predefined_len; ++i) {
        state_t * current = lexer->start_state;
        for (int j = 0; ; ++j) {
            char_t c = predefined[i].value[j];
            if (c == CHAR_END) {
                current->accept = i;
                break;
            }
            char_t m = lexer->char_to_meta[c];
            if (current->nexts[m] == NULL) {
                current->nexts[m] = make_state(meta_num, REJECT);
                state_num ++;
            }
            current = current->nexts[m];
        }
    }
    lexer->state_num = state_num;
}

token_info_t * lexer_poll(lexer_t * lexer, stream_t * stream, char_t * buffer, int buffer_len) {
    state_t * current = lexer->start_state;
    char_t c;
    int pos = 0;
    while(true) {
        if (current->accept != REJECT) {
            return make_token_info(
                    &lexer->predefined[current->accept],
                    stream_source_info(stream),
                    0,
                    0);
        }
        if (stream_peek(stream, &c) != EOK) {
            return NULL;
        }
        char_t m = lexer->char_to_meta[c];
        if (m == META_NULL) {

        }
    }
}

tokenizer_t * make_tokenizer(lexer_t * lexer, stream_t * stream) {
    tokenizer_t * tokenizer = new_data(tokenizer_t);
    tokenizer->lexer = lexer;
    tokenizer->stream = stream;
    tokenizer->buffer_pos = 0;
}

