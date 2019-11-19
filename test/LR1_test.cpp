#include "parsers/parser_algorithms/LR1.h"
#include "automata/nfa.h"
#include "parsers/regex_utils/regex_parser.h"
#include "parsers/regex_utils/regex_scanner.h"

using namespace std;

int main(){
    compiler::io_buffer::TextSourceBuffer input_grammar("../LR1_grammar.txt");
    compiler::io_buffer::TextSourceBuffer input_regex("../regex.txt");
    compiler::automata::DFA analyzer = compiler::automata::NFA::CalculateLexicalUnion(
            compiler::regex::RegexParser(compiler::regex::RegexScanner(&input_regex)).Parse()).ToDFA().Minimize();
    compiler::io_buffer::TextSourceBuffer input_file("../input_string.txt");
    compiler::analyzers::LexicalAnalyzerF tokenizer(&input_file, analyzer);
    compiler::parsers::LR1 yacc(&input_grammar, tokenizer);

    yacc.PrintParsingTable();

    return 0;
}
