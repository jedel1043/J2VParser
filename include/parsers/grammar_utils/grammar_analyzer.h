#ifndef COMPILER_GRAMMAR_ANALYZER_H
#define COMPILER_GRAMMAR_ANALYZER_H

#include "analyzers/lexical_analyzer_f.h"
#include "buffer.h"

namespace compiler::grammar {
    class GrammarAnalyzer : public analyzers::LexicalAnalyzerF {
    private:
        static io_buffer::TextSourceBuffer *CreateFileBuffer();

        static automata::DFA CreateAutomaton();

    public:
        explicit GrammarAnalyzer(io_buffer::TextSourceBuffer *input_file);
    };
} // namespace compiler::grammar

#endif //COMPILER_GRAMMAR_ANALYZER_H
