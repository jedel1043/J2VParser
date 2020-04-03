#ifndef COMPILER_LEXICAL_ANALYZER_F_H
#define COMPILER_LEXICAL_ANALYZER_F_H

#include "buffer.h"
#include "analyzers/lexical_analyzer.h"
#include "automata/dfa.h"

namespace compiler::analyzers {

    class LexicalAnalyzerF : public LexicalAnalyzer {
    protected:
        io_buffer::TextSourceBuffer *input_file_;
    public:
        LexicalAnalyzerF(io_buffer::TextSourceBuffer *inputFile, automata::DFA automata, bool skip_whitespace = true);

        Token yylex() override;

        bool isInEnd() override;

        char SkipWS() override;

        static bool isEOS(char c);

        io_buffer::TextSourceBuffer *getInputFile() const;
    };
} //namespace compiler::analyzers


#endif //COMPILER_LEXICAL_ANALYZER_F_H
