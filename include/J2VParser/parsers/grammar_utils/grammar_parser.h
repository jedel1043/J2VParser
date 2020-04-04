#ifndef J2VP_GRAMMAR_PARSER_H
#define J2VP_GRAMMAR_PARSER_H

#include <string>

#include "J2VParser/parsers/grammar_utils/grammar_analyzer.h"
#include "J2VParser/buffer.h"
#include "J2VParser/parsers/grammar_utils/grammar_array.h"

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

#endif //J2VP_GRAMMAR_PARSER_H
