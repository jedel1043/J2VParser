#ifndef LL1_H
#define LL1_H

#include <map>
#include <tuple>
#include <vector>
#include <variant>

#include "parsers/grammar_utils/grammar_parser.h"
#include "analyzers/lexical_analyzer.h"
#include "buffer.h"

namespace compiler::parsers {

    class LL1 {
    private:
        std::map<std::pair<std::string, std::string>, std::variant<std::string, std::pair<std::vector<std::string>, int>>> function_;
        std::set<std::string> terminals_;
        std::set<std::string> non_terminals_;
        std::string axiom_;
        analyzers::LexicalAnalyzer &tokenizer_;

        void PrintParsingTable();

    public:
        LL1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer);

        LL1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer);

        bool Parse(bool verbose = false);
    };
} // namespace compiler::parsers

#endif
