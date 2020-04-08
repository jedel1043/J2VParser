#ifndef J2VPARSER_LR0_H
#define J2VPARSER_LR0_H

#include <string>
#include <utility>
#include <map>
#include <vector>
#include <set>

#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/parsers/items.h"
#include "J2VParser/parsers/botup_parser.h"
#include "J2VParser/parsers/conflict_man.h"
#include "J2VParser/parsers/grammar/grammar_parser.h"


namespace J2VParser::parsers {

    class LR0 : public BotUpParser<LR0Item> {
    public:
        LR0(io_buffer::TextSourceBuffer &grammar_f, analyzers::LexicalAnalyzer &lexical_analyzer,
            bool augment_grammar = true);

        LR0(const grammar::GrammarArray &grammar_array, analyzers::LexicalAnalyzer &lexicalAnalyzer,
            bool augment_grammar = true);

    private:

        ItemSet ItemsClosure(const Item &item_input, ItemSet &calculated);

        ItemSet ItemsClosure(const ItemSet &input_items);

        ItemSet goTo(const ItemSet &input_items, const std::string &input_symbol);

        void CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function);

        void ThrowConflictError(Conflict c, const std::pair<ItemSet, int> &print_obj,
                                const std::set<int> &rule_set, const std::string &symbol = "");
    };

} // namespace const compiler::parsers

#endif //J2VPARSER_LR0_H
