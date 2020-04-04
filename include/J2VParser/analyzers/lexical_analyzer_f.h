#ifndef J2VP_LEXICAL_ANALYZER_F_H
#define J2VP_LEXICAL_ANALYZER_F_H

#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/automata/dfa.h"

namespace compiler::analyzers {

    class LexicalAnalyzerF : public LexicalAnalyzer {
    protected:
        io_buffer::TextSourceBuffer *input_file_;
    public:
        LexicalAnalyzerF(io_buffer::TextSourceBuffer *inputFile, automata::DFA automaton, bool skip_whitespace = true);

        Token yylex() override;

        bool isInEnd() override;

        char SkipWS() override;

        static bool isEOS(char c);

        [[nodiscard]] io_buffer::TextSourceBuffer *getInputFile() const;
    };
} //namespace compiler::analyzers


#endif //J2VP_LEXICAL_ANALYZER_F_H
