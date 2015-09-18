/**
 * @author goldolphin
 *         2015-08-28 16:36:36.
 */
#include <parser/parser.h>
#include <lexer/file_char_stream.h>
#include <lexer/basic_char_stream.h>

void test_str(const char * str) {
    basic_char_stream_t cs;
    basic_char_stream_init(&cs, (uint8_t *) str, (int) strlen(str));
    lexer_t lexer;
    lexer_init(&lexer);
    token_stream_t ts;
    token_stream_init(&ts, &lexer, &cs.super);
    parser_t parser;
    parser_init(&parser);

    while (true) {
        ast_statement_t *statement = parse_statement(&parser, &ts);
        if (statement == NULL) break;
        print_statement(0, statement);
    }
}

void test_file(const char * filename) {
    file_char_stream_t cs;
    file_char_stream_init(&cs, filename, 4096);
    lexer_t lexer;
    lexer_init(&lexer);
    token_stream_t ts;
    token_stream_init(&ts, &lexer, &cs.super);
    parser_t parser;
    parser_init(&parser);

    while (true) {
        ast_statement_t *statement = parse_statement(&parser, &ts);
        if (statement == NULL) break;
        print_statement(0, statement);
    }
}

int main() {
    test_str("1- - -1");
    test_file("src/test/evo/list.evo");
    return 0;
}
