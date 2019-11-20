#ifndef COMPILER_LR1_H
#define COMPILER_LR1_H

#include "parsers/parser_algorithms/parser.h"
#include "parsers/parser_algorithms/LR0.h"
#include "parsers/parser_algorithms/conflict_man.h"

namespace compiler::parsers {
    class LR1 : public Parser<std::map<std::pair<int, std::string>, std::pair<char, int>>>,
                public ConflictManager{
    public:
        LR1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar=true) :
                LR1(grammar::GrammarParser(input_file), tokenizer, augment_grammar) {}

        LR1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar=true);

        bool Parse(bool verbose) override;

    private:
        struct Item {
            LR0::Item lr0_item;
            std::string token;

            bool operator<(const Item& obj) const{
                return lr0_item.GetFullString() + token < obj.lr0_item.GetFullString() + obj.token;
            }


        };

        using ItemSet = std::set<Item>;
        using cell = std::pair<char, int>;

        int states_number_=0;

        ItemSet ItemsClosure(const Item &item_input, ItemSet &calculated);

        ItemSet ItemsClosure(const ItemSet &input_items);

        ItemSet goTo(const ItemSet &input_items, const std::string &input_symbol);

        void CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function);

        void PrintParsingTable() override;

        void PrintItemSet(const ItemSet &set);

        void ThrowConflictError(Conflict c, const std::pair<ItemSet, int> &print_obj, const std::set<int> &rule_set,
                                const std::string &symbol = "");
    };
}

#endif //COMPILER_LR1_H
