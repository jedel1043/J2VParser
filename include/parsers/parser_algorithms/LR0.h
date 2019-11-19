#ifndef COMPILER_LR0_H
#define COMPILER_LR0_H

#include <utility>
#include <unordered_set>

#include "buffer.h"
#include "analyzers/lexical_analyzer.h"
#include "parsers/grammar_utils/grammar_array.h"
#include "parsers/grammar_utils/grammar_parser.h"

namespace compiler::parsers {
    class LR0 {
    public:
        struct Item {
            std::string variable;
            std::vector<std::string> rule = {};
            int point{};

            Item() = default;

            Item(std::string variable, std::vector<std::string> rule,
                 int point):
                    variable(std::move(variable)), rule(std::move(rule)), point(point) {}

            Item(std::string var_input, std::vector<std::string> rule_input):
                    variable(std::move(var_input)), rule(std::move(rule_input)), point(0){
            };

            [[nodiscard]] const std::string& PointSymbol() const {return rule.at(point);}

            [[nodiscard]] std::string GetFullString() const {
                auto result = variable;
                int i = 0;
                for(const auto& symbol : rule) {
                    if(point== i++)
                        result+= ".";
                    result += symbol;
                }
                if (point == rule.size())
                    result += ".";
                return result;
            }

            bool operator<(const Item& obj) const{
                return GetFullString() < obj.GetFullString();
            }

        };

        LR0(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar=true) :
                LR0(grammar::GrammarParser(input_file), tokenizer, augment_grammar) {}

        LR0(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar=true);
        bool Parse(bool verbose = false);

    private:
        using ItemSet = std::set<Item>;
        using cell = std::pair<char, int>;

        std::map<std::pair<int, std::string>, cell> function_;
        grammar::GrammarArray grammar_;
        analyzers::LexicalAnalyzer &tokenizer_;
        int states_number_=0;

        void PrintParsingTable();

        ItemSet ItemsClosure(const Item &item_input, ItemSet &calculated);

        ItemSet ItemsClosure(const ItemSet &input_items);

        ItemSet goTo(const ItemSet &input_items, const std::string &input_symbol);

        void CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function);
    };

    std::ostream &operator<<(std::ostream &stream_in, const LR0::Item& obj);

} // namespace compiler::parsers

#endif //COMPILER_LR0_H
