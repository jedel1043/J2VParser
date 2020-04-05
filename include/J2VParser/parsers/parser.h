#ifndef J2VP_PARSER_H
#define J2VP_PARSER_H

#include <utility>

#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/analyzers/regex/regex_scanner.h"
#include "J2VParser/analyzers/regex/regex_parser.h"
#include "J2VParser/parsers/grammar/grammar_parser.h"
#include "J2VParser/parsers/grammar/grammar_array.h"

namespace J2VParser::parsers {
    template<typename T>
    class Parser {
    protected:
        T function_;
        grammar::GrammarArray grammar_;
        analyzers::LexicalAnalyzer &tokenizer_;
        int number_of_conflicts_ = 0;
    public:
        /*
         * Base constructor for all parsers
         */
        Parser(grammar::GrammarArray grammarArray, analyzers::LexicalAnalyzer &lexicalAnalyzer) :
                grammar_(std::move(grammarArray)), tokenizer_(lexicalAnalyzer) {};

        virtual bool Parse(bool verbose) = 0;

        bool Parse() { return Parse(false); }

        virtual void PrintParsingTable() = 0;
    };
} // namespace compiler::parsers
#endif //J2VP_PARSER_H
