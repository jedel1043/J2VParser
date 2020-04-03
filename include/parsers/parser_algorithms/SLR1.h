#ifndef COMPILER_SLR1_H
#define COMPILER_SLR1_H
#include <utility>

#include "buffer.h"
#include "parsers/parser_algorithms/parser.h"
#include "parsers/parser_algorithms/conflict_man.h"
#include "analyzers/lexical_analyzer.h"
#include "parsers/grammar_utils/grammar_array.h"
#include "parsers/grammar_utils/grammar_parser.h"

namespace compiler::parsers {

    class SLR1 : public Parser<std::map<std::pair<int, std::string>, std::pair<char, int>>>,
                 public ConflictManager{
    public:
        struct Item {
            std::string variable;
            std::vector<std::string> rule = {};
            size_t point{};

            Item() = default;

            Item(std::string variable, std::vector<std::string> rule,
                 int point):
                    variable(std::move(variable)), rule(std::move(rule)), point(point) {}

            Item(std::string var_input, std::vector<std::string> rule_input):
                    variable(std::move(var_input)), rule(std::move(rule_input)), point(0){
            };

            bool operator==(const Item& obj) const{
                return GetFullString() == obj.GetFullString();
            }

            bool operator<(const Item& obj) const{
                return GetFullString() < obj.GetFullString();
            }

            [[nodiscard]] const std::string& PointSymbol() const {return rule.at(point);}

            [[nodiscard]] std::string GetFullString() const;

            [[nodiscard]] std::string to_string() const;

        };

        SLR1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar=true) :
                SLR1(grammar::GrammarParser(input_file), tokenizer, augment_grammar) {}

        SLR1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar=true);
        bool Parse(bool verbose) override;

    protected:
        using ItemSet = std::set<Item>;
        using cell = std::pair<char, int>;

        int states_number_=0;

        void PrintParsingTable() override;

        ItemSet ItemsClosure(const Item &item_input, ItemSet &calculated);

        ItemSet ItemsClosure(const ItemSet &input_items);

        ItemSet goTo(const ItemSet &input_items, const std::string &input_symbol);

        void CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function);

        void ThrowConflictError(Conflict c, const std::pair<ItemSet, int> &print_obj, const std::set<int> &rule_set,
                                const std::string &symbol = "");
    };

} // namespace const compiler::parsers
#endif //COMPILER_SLR1_H
