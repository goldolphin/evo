/**
 * @author goldolphin
 *         2015-08-28 16:36:36.
 */
#include <parser/parser.h>
#include <lexer/file_char_stream.h>

int main() {
    file_char_stream_t cs;
    file_char_stream_init(&cs, "src/test/evo/list.evo", 4096);
    lexer_t lexer;
    lexer_init(&lexer);
    token_stream_t ts;
    token_stream_init(&ts, &lexer, &cs.super);

    while (true) {
        ast_statement_t *statement = parse_statement(&ts);
        if (statement == NULL) break;
        print_statement(0, statement);
    }
    return 0;
}
