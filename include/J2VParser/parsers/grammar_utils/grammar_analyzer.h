#ifndef J2VP_GRAMMAR_ANALYZER_H
#define J2VP_GRAMMAR_ANALYZER_H

#include "J2VParser/analyzers/lexical_analyzer_f.h"
#include "J2VParser/automata/nfa.h"
#include "J2VParser/buffer.h"

namespace compiler::grammar {
    class GrammarAnalyzer : public analyzers::LexicalAnalyzerF {
    private:
        static automata::DFA CreateAutomaton();

    public:
        explicit GrammarAnalyzer(io_buffer::TextSourceBuffer *input_file);
    };
} // namespace compiler::grammar

#endif //J2VP_GRAMMAR_ANALYZER_H
