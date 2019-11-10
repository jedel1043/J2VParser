

#ifndef COMPILER_LEXICAL_ANALYZER_F_H
#define COMPILER_LEXICAL_ANALYZER_F_H


#include <io/buffer.h>
#include <automata/DFA.h>
#include "lexical_analyzer.h"

class lexical_analyzer_f : public lexical_analyzer {
    TextSourceBuffer *input_file;
public:
    lexical_analyzer_f(TextSourceBuffer *inputFile, DFA automata, bool skip_whitespace=true);

    Token yylex() override;

    bool is_in_end() override;

    char skip_ws() override;
    static bool is_eos(char c);
};


#endif //COMPILER_LEXICAL_ANALYZER_F_H
