#include "J2VParser/parsers/LR0.h"

#include <stack>
#include <algorithm>

namespace J2VParser::parsers {

    LR0::ItemSet LR0::ItemsClosure(const Item &item_input, ItemSet &calculated) {
        if (calculated.count(item_input))
            return {};
        calculated.insert(item_input);
        ItemSet closure_result = {item_input};
        if (item_input.point_ < item_input.rule_.size() && grammar_.non_terminals().count(item_input.PointSymbol())) {
            auto rules = grammar_[item_input.PointSymbol()];
            for (const auto &rule : rules) {
                Item item_saver(item_input.PointSymbol(), rule);
                auto closure_saver = ItemsClosure(item_saver, calculated);
                if (!closure_saver.empty())
                    closure_result.insert(closure_saver.begin(), closure_saver.end());
            }
        }
        return closure_result;
    }

    LR0::ItemSet LR0::ItemsClosure(const ItemSet &input_items) {
        ItemSet closure_result = {}, calculated = {};
        for (const auto &item : input_items) {
            auto closure_saver = ItemsClosure(item, calculated);
            if (!closure_saver.empty())
                closure_result.insert(closure_saver.begin(), closure_saver.end());
        }
        return closure_result;
    }

    LR0::ItemSet LR0::goTo(const ItemSet &input_items, const std::string &input_symbol) {
        if (input_symbol == "$")
            return {};
        ItemSet goto_result = {};
        for (const Item &item : input_items) {
            if (item.point_ < item.rule_.size() && item.PointSymbol() == input_symbol)
                goto_result.insert(Item(item.variable_, item.rule_, item.point_ + 1));
        }
        goto_result = ItemsClosure(goto_result);
        return goto_result;
    }

    LR0::LR0(io_buffer::TextSourceBuffer &grammar_f, analyzers::LexicalAnalyzer &lexical_analyzer,
             bool augment_grammar) :
            BotUpParser(grammar_f, lexical_analyzer, augment_grammar) {}

    LR0::LR0(const grammar::GrammarArray &grammar_array, analyzers::LexicalAnalyzer &lexicalAnalyzer,
             bool augment_grammar) :
            BotUpParser(grammar_array, lexicalAnalyzer, augment_grammar) {
        grammar_.InsertTerminal("$");
        std::vector<std::tuple<std::string, ItemSet, ItemSet>> states_function = {};

        std::string ax_saver = grammar_.axiom();
        auto rule_saver = *grammar_[ax_saver].begin();

        Item axiom(ax_saver, rule_saver);

        auto first_state = ItemsClosure({axiom});
        std::stack<ItemSet> pending;
        pending.push(first_state);
        std::set<ItemSet> states_set = {first_state};

        while (!pending.empty()) {
            ItemSet actual_state = pending.top();
            pending.pop();

            std::set<std::string> symbol_set;
            for (const auto &item : actual_state) {
                if (item.point_ < item.rule_.size())
                    symbol_set.insert(item.PointSymbol());
            }

            for (const auto &symbol : symbol_set) {
                ItemSet new_state = ItemsClosure(goTo(actual_state, symbol));
                if (!new_state.empty()) {
                    if (!states_set.count(new_state)) {
                        pending.push(new_state);
                        states_set.insert(new_state);
                    }
                    states_function.emplace_back(symbol, actual_state, new_state);
                }
            }
        }
        CreateParsingTable(states_function);
    }

    void LR0::CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function) {
        std::map<ItemSet, int> states_index;
        std::set<int> analyzed;

        std::vector<std::string> axiom_rule = *grammar_[grammar_.axiom()].begin();
        auto accept_item = Item(grammar_.axiom(), axiom_rule, 1);

        for (const auto &action : states_function) {
            const auto&[symbol, initial_state, next_state] = action;
            if (!states_index.count(initial_state)) {
                states_number_++;
                states_index[initial_state] = states_index.size();
            }
            if (!states_index.count(next_state)) {
                states_number_++;
                states_index[next_state] = states_index.size();
            }
            if (grammar_.terminals().count(symbol))
                function_[{states_index[initial_state], symbol}] = {'s', states_index[next_state]};
            else
                function_[{states_index[initial_state], symbol}] = {'g', states_index[next_state]};
            if (next_state.count(accept_item)) {
                function_[{states_index[next_state], "$"}] = {'a', -1};
                continue;
            }
            if (!analyzed.count(states_index[next_state])) {
                analyzed.insert(states_index[next_state]);
                if (next_state.size() == 1) {
                    const auto &item = *next_state.begin();
                    if (item.point_ == item.rule_.size()) {
                        for (const auto &terminal : grammar_.terminals()) {
                            function_[{states_index[next_state], terminal}] =
                                    {'r', grammar_.GetRuleIndex(item.variable_, item.rule_)};
                        }
                    }
                } else {
                    std::set<int> symbol_r;
                    std::set<std::string> symbol_s;
                    for (const auto &item : next_state) {
                        if (item.point_ == item.rule_.size())
                            symbol_r.insert(grammar_.GetRuleIndex(item.variable_, item.rule_));
                        else if (grammar_.terminals().count(item.PointSymbol()))
                            symbol_s.insert(item.PointSymbol());
                    }
                    if (symbol_r.size() > 1) {
                        ThrowConflictError(Conflict::kReduceReduceConflict,
                                           {next_state, states_index[next_state]}, symbol_r);
                        number_of_conflicts_++;
                    }
                    if (!symbol_s.empty() && !symbol_r.empty()) {
                        for (const auto &sym : symbol_s) {
                            ThrowConflictError(Conflict::kShiftReduceConflict,
                                               {next_state, states_index[next_state]},
                                               symbol_r, sym);
                            number_of_conflicts_++;
                        }
                    }
                }
            }
        }
        if (number_of_conflicts_ > 0) {
            std::cerr << "Found " + std::to_string(number_of_conflicts_) + " conflicts when creating the parsing table."
                      << std::endl;
            std::cerr << "Stopping parsing..." << std::endl << std::endl;
            exit(-1);
        }
    }

    void LR0::ThrowConflictError(Conflict c, const std::pair<ItemSet, int> &print_obj,
                                 const std::set<int> &rule_set, const std::string &symbol) {
        if (c == Conflict::kShiftReduceConflict) {
            std::cerr << std::endl << "In state " << std::to_string(print_obj.second) << ":" << std::endl;
            for (const auto &item : print_obj.first) {
                std::cerr << "\t" << std::to_string(grammar_.GetRuleIndex(item.variable_, item.rule_))
                          << item.to_string() << std::endl;
            }
            std::cerr << std::endl;
            std::cerr << std::endl
                      << "Found Shift/Reduce conflict for symbol '" + symbol + "' caused by production(s): "
                      << std::endl;
            for (int i : rule_set) {
                auto rule_saver = grammar_.GetRuleFromIndex(i);
                std::cerr << "\t" << std::to_string(i) + ".- " << rule_saver.first << +" -> ";
                for (const auto &sym: rule_saver.second)
                    std::cerr << sym + " ";
                std::cerr << std::endl;
            }
            std::cerr << std::endl;
        } else if (c == Conflict::kReduceReduceConflict) {
            std::cerr << std::endl << "In state " << std::to_string(print_obj.second) << ":" << std::endl;
            for (const auto &item : print_obj.first) {
                std::cerr << "\t" << std::to_string(grammar_.GetRuleIndex(item.variable_, item.rule_))
                          << item.to_string() << std::endl;
            }
            std::cerr << std::endl;
            std::cerr << std::endl
                      << "Found Reduce/Reduce conflict caused by production(s): "
                      << std::endl;
            for (int i : rule_set) {
                auto rule_saver = grammar_.GetRuleFromIndex(i);
                std::cerr << "\t" << std::to_string(i) + ".- " << rule_saver.first << +" -> ";
                for (const auto &sym: rule_saver.second)
                    std::cerr << sym + " ";
                std::cerr << std::endl;
            }
            std::cerr << std::endl;
        }
    }

} // namespace compiler::parsers

