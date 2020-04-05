#ifndef COMPILER_LR1_H
#define COMPILER_LR1_H

#include <string>
#include <utility>
#include <map>
#include <vector>
#include <set>

#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/parsers/grammar/grammar_array.h"
#include "J2VParser/parsers/grammar/grammar_parser.h"
#include "J2VParser/parsers/conflict_man.h"
#include "J2VParser/parsers/items.h"
#include "J2VParser/parsers/botup_parser.h"

namespace J2VParser::parsers {

    class LR1 : public BotUpParser<LR1Item> {
    public:
        LR1(const grammar::GrammarArray &grammar_array, analyzers::LexicalAnalyzer &lexical_array,
            bool augment_grammar = true);

        LR1(io_buffer::TextSourceBuffer &input_file, analyzers::LexicalAnalyzer &tokenizer,
            bool augment_grammar = true);

    private:

        ItemSet ItemsClosure(const Item &item_input, ItemSet &calculated);

        ItemSet ItemsClosure(const ItemSet &input_items);

        ItemSet goTo(const ItemSet &input_items, const std::string &input_symbol);

        void CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function);

        void PrintItemSet(const ItemSet &set);

        void ThrowConflictError(Conflict c, const std::pair<ItemSet, int> &print_obj, const std::set<int> &rule_set,
                                const std::string &symbol = "");
    };
}

#endif //COMPILER_LR1_H
