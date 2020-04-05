#include <iostream>
#include <cstring>

#include "J2VParser/error.h"
#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer_f.h"
#include "J2VParser/parsers/LL1.h"
#include "J2VParser/analyzers/lexical_analyzer_s.h"

using namespace std;
using namespace J2VParser;

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "syntax: [input Grammar file_] [input regex file_] [input text_ file_ | \"input string\"]" << endl;
        cout << "Insert flags at the end of the inputs." << endl;
        cout << "-v/V = Verbose mode" << endl;
        return 0;
    } else {
        if (argc < 3)
            AbortTranslation(J2VParser::error::InvalidCommandLineArgs);
        else {
            if (argc < 4)
                AbortTranslation(J2VParser::error::InvalidCommandLineArgs);
            else {
                bool verbose = false;
                if (argc > 4 && (std::strcmp(argv[4], "-v") == 0 || std::strcmp(argv[4], "-V") == 0))
                    verbose = true;
                J2VParser::io_buffer::TextSourceBuffer input_grammar(argv[1]);
                J2VParser::io_buffer::TextSourceBuffer input_regex(argv[2]);
                if (ifstream(argv[3]).good()) {
                    J2VParser::io_buffer::TextSourceBuffer input_file(argv[3]);
                    J2VParser::analyzers::LexicalAnalyzerF tokenizer(input_file, input_regex);
                    J2VParser::parsers::LL1 yacc(input_grammar, tokenizer);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                } else {
                    J2VParser::analyzers::LexicalAnalyzerS tokenizer(argv[3], input_regex);
                    J2VParser::parsers::LL1 yacc(input_grammar, tokenizer);
                    bool accepted = yacc.Parse(verbose);
                    cout << endl << "Accepted string? " << (accepted ? "true" : "false") << endl << endl;
                }
            }
        }
    }
    return 0;
}

