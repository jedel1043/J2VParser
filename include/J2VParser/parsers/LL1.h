#ifndef J2VP_LL1_H
#define J2VP_LL1_H

#include <string>
#include <utility>
#include <map>
#include <vector>

#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/parsers/parser.h"
#include "J2VParser/parsers/conflict_man.h"
#include "J2VParser/parsers/grammar/grammar_array.h"
#include "J2VParser/parsers/grammar/grammar_parser.h"

namespace J2VParser::parsers {

    class LL1 : public Parser<std::map<std::pair<std::string, std::string>, int>> {
    private:
        void CreateParsingTable();

        void PrintParsingTable() override;

        void ThrowConflictError(Conflict c, const std::vector<int> &print_obj, const std::string &symbol);

    public:
        LL1(const grammar::GrammarArray &grammarArray, analyzers::LexicalAnalyzer &lexicalAnalyzer) :
                Parser(grammarArray, lexicalAnalyzer) {
            CreateParsingTable();
        }

        LL1(io_buffer::TextSourceBuffer &grammar_f, analyzers::LexicalAnalyzer &lexicalAnalyzer) :
                LL1(grammar::ParseGrammar(grammar_f), lexicalAnalyzer) {}

        bool Parse(bool verbose) override;
    };
} // namespace compiler::parsers
#endif //J2VP_LL1_H
