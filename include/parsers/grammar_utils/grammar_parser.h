#ifndef COMPILER_GRAMMAR_PARSER_H
#define COMPILER_GRAMMAR_PARSER_H

#include <utility>

#include "parsers/grammar_utils/grammar_analyzer.h"
#include "parsers/grammar_utils/grammar_array.h"

namespace compiler::grammar {
    class GrammarParser {
    private:
        GrammarAnalyzer scanner_;

        void Grammar(GrammarArray &new_grammar);

        void Rule(GrammarArray &new_grammar);

        void RightSide(const std::string &left_s, GrammarArray &new_grammar);

        std::string Symbol();

    public:
        explicit GrammarParser(io_buffer::TextSourceBuffer *input_file) : scanner_(input_file) {};

        GrammarArray ParseGrammar() {
            GrammarArray parsed_grammar;
            Grammar(parsed_grammar);
            return parsed_grammar;
        }

    };
} // namespace compiler::grammar

#endif //COMPILER_GRAMMAR_PARSER_H
