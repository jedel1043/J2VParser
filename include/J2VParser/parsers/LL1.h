#ifndef J2VP_LL1_H
#define J2VP_LL1_H

#include <string>
#include <utility>
#include <map>
#include <vector>

#include "J2VParser/parsers/parser.h"
#include "J2VParser/parsers/conflict_man.h"
#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/parsers/grammar_utils/grammar_parser.h"

namespace compiler::parsers {

    class LL1 : public Parser<std::map<std::pair<std::string, std::string>, int>> {
    private:
        void PrintParsingTable() override;

        void ThrowConflictError(Conflict c, const std::vector<int> &print_obj, const std::string &symbol);

    public:
        LL1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer);

        LL1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer);

        bool Parse(bool verbose);
    };
} // namespace compiler::parsers
#endif //J2VP_LL1_H
