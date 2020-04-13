#include <iostream>
#include <cstring>

#include "J2VParser/error.h"
#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer_f.h"
#include "J2VParser/parsers/LR0.h"
#include "J2VParser/analyzers/lexical_analyzer_s.h"

using namespace std;
using namespace J2VParser;

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "syntax: [input Grammar file_] [input regex file_] [input text_ file_ | \"input string\"]" << endl;
        cout << "Insert flags at the end of the inputs." << endl;
        cout << "-v/V = Verbose mode" << endl;
        cout << "--augmented-grammar = Input grammar already in augmented grammar form." << endl;
        return 0;
    } else {
        if (argc < 3)
            AbortTranslation(error::InvalidCommandLineArgs);
        else {
            if (argc < 4)
                AbortTranslation(error::InvalidCommandLineArgs);
            else {
                bool verbose = false, augment_grammar = true;
                for (int i = 4; i < argc; ++i) {
                    if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "-V") == 0)
                        verbose = true;
                    else if (std::strcmp(argv[i], "--augmented-grammar") == 0)
                        augment_grammar = false;
                    else
                        AbortTranslation(error::InvalidCommandLineArgs);
                }
                io_buffer::TextSourceBuffer input_grammar(argv[1]);
                io_buffer::TextSourceBuffer input_regex(argv[2]);
                std::string input{argv[3]};
                if (ifstream(argv[3]).good()) {
                    io_buffer::TextSourceBuffer input_file(input);
                    analyzers::LexicalAnalyzerF tokenizer(input_file, input_regex);
                    parsers::LR0 yacc(input_grammar, tokenizer, augment_grammar);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                } else {

                    analyzers::LexicalAnalyzerS tokenizer(input, input_regex);
                    parsers::LR0 yacc(input_grammar, tokenizer, augment_grammar);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                }
            }
        }
    }
    return 0;
}

