#include <iostream>

#include "analyzers/lexical_analyzer_s.h"
#include "parsers/parser_algorithms/SLR1.h"
#include "automata/nfa.h"
#include "automata/dfa.h"
#include "parsers/regex_utils/regex_scanner.h"
#include "parsers/regex_utils/regex_parser.h"

using namespace std;

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "syntax: [input Grammar file_] [input regex file_] [input text_ file_ | \"input string\"]" << endl;
        cout << "Insert flags at the end of the inputs." << endl;
        cout << "-v/V = Verbose mode" << endl;
        cout << "--augmented-grammar = Input grammar already in augmented grammar form." << endl;
        return 0;
    }
    else {
        if(argc < 3)
            AbortTranslation(compiler::error::InvalidCommandLineArgs);
        else{
            if (argc < 4)
                AbortTranslation(compiler::error::InvalidCommandLineArgs);
            else{
                bool verbose = false, augment_grammar = true;
                for (int i = 4; i < argc; ++i) {
                    if(std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "-V") == 0)
                        verbose = true;
                    else if(std::strcmp(argv[i], "--augmented-grammar") == 0)
                        augment_grammar = false;
                    else
                        AbortTranslation(compiler::error::InvalidCommandLineArgs);
                }
                compiler::io_buffer::TextSourceBuffer input_grammar(argv[1]);
                compiler::io_buffer::TextSourceBuffer input_regex(argv[2]);
                compiler::automata::DFA analyzer = compiler::automata::NFA::CalculateLexicalUnion(
                        compiler::regex::RegexParser(compiler::regex::RegexScanner(&input_regex)).Parse()).ToDFA();

                if(ifstream(argv[3]).good()) {
                    compiler::io_buffer::TextSourceBuffer input_file(argv[3]);
                    compiler::analyzers::LexicalAnalyzerF tokenizer(&input_file, analyzer);
                    compiler::parsers::SLR1 yacc(&input_grammar, tokenizer, augment_grammar);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                }
                else{
                    compiler::analyzers::LexicalAnalyzerS tokenizer(argv[3], analyzer);
                    compiler::parsers::SLR1 yacc(&input_grammar, tokenizer, augment_grammar);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                }
            }
        }
    }
    return 0;
}

