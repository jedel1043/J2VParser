#include <iostream>
#include <cstring>

#include "J2VParser/error.h"
#include "J2VParser/buffer.h"
#include "J2VParser/automata/dfa.h"
#include "J2VParser/automata/nfa.h"
#include "J2VParser/parsers/regex_utils/regex_parser.h"
#include "J2VParser/analyzers/lexical_analyzer_f.h"
#include "J2VParser/parsers/LR0.h"
#include "J2VParser/analyzers/lexical_analyzer_s.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
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
                    compiler::parsers::LR0 yacc(&input_grammar, tokenizer, augment_grammar);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                }
                else{
                    compiler::analyzers::LexicalAnalyzerS tokenizer(argv[3], analyzer);
                    compiler::parsers::LR0 yacc(&input_grammar, tokenizer, augment_grammar);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                }
            }
        }
    }
    return 0;
}
