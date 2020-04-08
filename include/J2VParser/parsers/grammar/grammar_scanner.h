#ifndef J2VPARSER_GRAMMAR_ANALYZER_H
#define J2VPARSER_GRAMMAR_ANALYZER_H

#include "J2VParser/analyzers/lexical_analyzer_f.h"
#include "J2VParser/analyzers/automata/nfa.h"
#include "J2VParser/buffer.h"

namespace J2VParser::grammar {
    class GrammarScanner : public analyzers::LexicalAnalyzerF {
    private:
        static automata::DFA CreateAutomaton();

    public:
        explicit GrammarScanner(io_buffer::TextSourceBuffer &sourceBuffer);
    };
} // namespace compiler::grammar

#endif //J2VPARSER_GRAMMAR_ANALYZER_H
