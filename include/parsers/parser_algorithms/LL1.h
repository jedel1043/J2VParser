#ifndef LL1_H
#define LL1_H

#include <map>
#include <tuple>
#include <vector>
#include <variant>

#include "parsers/parser_algorithms/parser.h"
#include "parsers/grammar_utils/grammar_parser.h"
#include "analyzers/lexical_analyzer.h"
#include "buffer.h"
#include "parsers/parser_algorithms/conflict_man.h"

namespace compiler::parsers {

    class LL1 : public Parser<std::map<std::pair<std::string, std::string>, int>>,
                public ConflictManager{
    private:
        void PrintParsingTable() override;
        void ThrowConflictError(Conflict c, const std::vector<int> &print_obj, const std::string &symbol);

    public:
        LL1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer);

        LL1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer);
        bool Parse(bool verbose) override;
    };
} // namespace compiler::parsers

#endif
