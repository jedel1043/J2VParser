#ifndef J2VP_PARSER_H
#define J2VP_PARSER_H

#include <utility>

#include "J2VParser/parsers/grammar_utils/grammar_array.h"
#include "J2VParser/analyzers/lexical_analyzer.h"

namespace compiler::parsers {
    template<typename T>
    class Parser {
    protected:
        T function_;
        grammar::GrammarArray grammar_;
        analyzers::LexicalAnalyzer &tokenizer_;
        int number_of_conflicts_ = 0;
    public:
        explicit Parser(analyzers::LexicalAnalyzer &tokenizer, grammar::GrammarArray grammar) :
        tokenizer_(tokenizer), grammar_(std::move(grammar)) {}
        virtual bool Parse(bool verbose) = 0;
        bool Parse(){ return Parse(false); }

        virtual void PrintParsingTable() = 0;
    };
} // namespace compiler::parsers
#endif //J2VP_PARSER_H
