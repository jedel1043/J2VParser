#include "J2VParser/parsers/LR1.h"

#include <stack>
#include <algorithm>

namespace J2VParser::parsers {

    LR1::ItemSet LR1::ItemsClosure(const Item &item_input, ItemSet &calculated) {
        if (calculated.count(item_input))
            return {};
        calculated.insert(item_input);
        ItemSet closure_result = {item_input};
        if (item_input.lr0_item_.point_ < item_input.lr0_item_.rule_.size() &&
            grammar_.non_terminals().count(item_input.lr0_item_.PointSymbol())) {
            auto rules = grammar_[item_input.lr0_item_.PointSymbol()];
            for (const auto &rule : rules) {

                std::vector<std::string> saver;
                saver.insert(saver.end(), item_input.lr0_item_.rule_.begin() + item_input.lr0_item_.point_ + 1,
                             item_input.lr0_item_.rule_.end());
                saver.push_back(item_input.token_);

                for (const auto &token : grammar_.First(saver)) {
                    Item item_saver = {{item_input.lr0_item_.PointSymbol(), rule, 0}, token};
                    auto closure_saver = ItemsClosure(item_saver, calculated);
                    if (!closure_saver.empty())
                        closure_result.insert(closure_saver.begin(), closure_saver.end());
                }
            }
        }
        return closure_result;
    }

    LR1::ItemSet LR1::ItemsClosure(const ItemSet &input_items) {
        ItemSet closure_result = {}, calculated = {};
        for (const auto &item : input_items) {
            auto closure_saver = ItemsClosure(item, calculated);
            if (!closure_saver.empty())
                closure_result.insert(closure_saver.begin(), closure_saver.end());
        }
        return closure_result;
    }

    LR1::ItemSet LR1::goTo(const ItemSet &input_items, const std::string &input_symbol) {
        if (input_symbol == "$")
            return {};
        ItemSet goto_result = {};
        for (const Item &item : input_items) {
            if (item.lr0_item_.point_ < item.lr0_item_.rule_.size() && item.lr0_item_.PointSymbol() == input_symbol)
                goto_result.insert(
                        {{item.lr0_item_.variable_, item.lr0_item_.rule_, item.lr0_item_.point_ + 1}, item.token_});
        }
        goto_result = ItemsClosure(goto_result);
        return goto_result;
    }

    LR1::LR1(io_buffer::TextSourceBuffer &input_file, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar) :
            LR1(grammar::ParseGrammar(input_file), tokenizer, augment_grammar) {}

    LR1::LR1(const grammar::GrammarArray &grammar_array, analyzers::LexicalAnalyzer &lexical_array,
             bool augment_grammar) :
            BotUpParser(grammar_array, lexical_array, augment_grammar) {
        grammar_.InsertTerminal("$");
        std::vector<std::tuple<std::string, ItemSet, ItemSet>> states_function = {};

        std::string ax_saver = grammar_.axiom();
        auto rule_saver = *grammar_[ax_saver].begin();

        Item axiom = {{ax_saver, rule_saver, 0}, "$"};

        auto first_state = ItemsClosure({axiom});
        std::stack<ItemSet> pending;
        pending.push(first_state);
        std::set<ItemSet> states_set = {first_state};

        while (!pending.empty()) {
            ItemSet actual_state = pending.top();
            pending.pop();

            std::set<std::string> symbol_set;
            for (const auto &item : actual_state) {
                if (item.lr0_item_.point_ < item.lr0_item_.rule_.size())
                    symbol_set.insert(item.lr0_item_.PointSymbol());
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

    void LR1::CreateParsingTable(const std::vector<std::tuple<std::string, ItemSet, ItemSet>> &states_function) {
        std::map<ItemSet, int> states_index;
        std::set<int> analyzed;

        std::vector<std::string> axiom_rule = *grammar_[grammar_.axiom()].begin();
        Item accept_item = {{grammar_.axiom(), axiom_rule, 1}, "$"};

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

                std::map<std::string, int> symbol_r;
                std::set<int> scanned_rules;
                std::set<std::string> symbol_s;
                for (const auto &item : next_state) {
                    if (item.lr0_item_.point_ == item.lr0_item_.rule_.size()) {
                        if (symbol_r.count(item.token_) &&
                            symbol_r[item.token_] !=
                            grammar_.GetRuleIndex(item.lr0_item_.variable_, item.lr0_item_.rule_)) {
                            ThrowConflictError(Conflict::kReduceReduceConflict,
                                               {next_state, states_index[next_state]},
                                               scanned_rules);
                            number_of_conflicts_++;
                        } else {
                            symbol_r[item.token_] = grammar_.GetRuleIndex(item.lr0_item_.variable_,
                                                                          item.lr0_item_.rule_);
                            scanned_rules.insert(symbol_r[item.token_]);
                        }
                    } else if (grammar_.terminals().count(item.lr0_item_.PointSymbol()))
                        symbol_s.insert(item.lr0_item_.PointSymbol());
                    if (item.lr0_item_.point_ == item.lr0_item_.rule_.size())
                        function_[{states_index[next_state], item.token_}] =
                                {'r', grammar_.GetRuleIndex(item.lr0_item_.variable_, item.lr0_item_.rule_)};
                }
                for (const auto &item : symbol_s) {
                    if (symbol_r.count(item)) {
                        ThrowConflictError(Conflict::kShiftReduceConflict,
                                           {next_state, states_index[next_state]},
                                           {symbol_r[item]},
                                           item);
                        number_of_conflicts_++;
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

    void LR1::ThrowConflictError(Conflict c, const std::pair<ItemSet, int> &print_obj,
                                 const std::set<int> &rule_set, const std::string &symbol) {
        if (c == Conflict::kShiftReduceConflict) {
            std::cerr << std::endl << "In state " << std::to_string(print_obj.second) << ":" << std::endl;
            PrintItemSet(print_obj.first);
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
            PrintItemSet(print_obj.first);
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

    void LR1::PrintItemSet(const LR1::ItemSet &set) {
        std::vector<std::pair<LR0Item, std::set<std::string>>> items;
        for (const auto &it : set) {
            if (!items.empty() && items.back().first == it.lr0_item_)
                items.back().second.insert(it.token_);
            else
                items.push_back({it.lr0_item_, {it.token_}});
        }
        std::string result;
        for (const auto &it : items) {
            result = "\t\t" + std::to_string(grammar_.GetRuleIndex(it.first.variable_, it.first.rule_)) +
                     it.first.to_string() + "\t\t{";
            for (const auto &it2 : it.second)
                result += it2 + ", ";
            result.pop_back();
            result.pop_back();
            result += "}\n";
            std::cout << result << std::endl;
        }
    }
} // namespace compiler::parsers

