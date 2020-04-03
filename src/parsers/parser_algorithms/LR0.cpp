#include "parsers/parser_algorithms/LR0.h"

#include <algorithm>
#include <stack>
#include <iterator>


namespace compiler::parsers {

    std::string LR0::Item::to_string() const {
        std::string output;
        output += ".- " + variable + " -> ";
        int i = 0;
        for (const auto &str : rule) {
            if (point == i)
                output += ". ";
            output += str + " ";
            i++;
        }
        output.pop_back();
        if (point == rule.size())
            output += " .";
        return output;
    }

    std::string LR0::Item::GetFullString() const {
        auto result = variable;
        int i = 0;
        for (const auto &symbol : rule) {
            if (point == i++)
                result += ".";
            result += symbol;
        }
        if (point == rule.size())
            result += ".";
        return result;
    }

    LR0::ItemSet LR0::ItemsClosure(const Item &item_input, ItemSet &calculated) {
        if (calculated.count(item_input))
            return {};
        calculated.insert(item_input);
        ItemSet closure_result = {item_input};
        if (item_input.point < item_input.rule.size() && grammar_.non_terminals().count(item_input.PointSymbol())) {
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
            if (item.point < item.rule.size() && item.PointSymbol() == input_symbol)
                goto_result.insert(Item(item.variable, item.rule, item.point + 1));
        }
        goto_result = ItemsClosure(goto_result);
        return goto_result;
    }

    LR0::LR0(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer, bool augment_grammar) :
            Parser(tokenizer, augment_grammar ? parser.ParseGrammar().GetAugmentedGrammar() : parser.ParseGrammar()) {
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
                if (item.point < item.rule.size())
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
                    if (item.point == item.rule.size()) {
                        for (const auto &terminal : grammar_.terminals()) {
                            function_[{states_index[next_state], terminal}] =
                                    {'r', grammar_.GetRuleIndex(item.variable, item.rule)};
                        }
                    }
                } else {
                    std::set<int> symbol_r;
                    std::set<std::string> symbol_s;
                    for (const auto &item : next_state) {
                        if (item.point == item.rule.size())
                            symbol_r.insert(grammar_.GetRuleIndex(item.variable, item.rule));
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

    void LR0::PrintParsingTable() {
        std::vector<std::string> symbol_set;
        std::set<std::string> terminals = grammar_.terminals();
        std::set<std::string> non_terminals = grammar_.non_terminals();
        symbol_set.insert(symbol_set.end(), terminals.rbegin(), terminals.rend());
        std::copy_if(non_terminals.begin(), non_terminals.end(),
                     std::back_inserter(symbol_set), [this](auto val) { return val != grammar_.axiom(); });

        std::cout << std::endl << grammar_ << std::endl << std::endl;

        std::string separator = ".:*~*:._";
        for (int i = 1; i <= 25; i++)
            std::cout << separator;

        std::cout << std::endl;

        std::cout << std::endl << std::endl << "Parsing table:" << std::endl << std::endl;
        std::string line = "\n+----------+";
        for (int i = 1; i <= (int) (22 * symbol_set.size()); i++) {
            if (i % 22 == 0)
                line += '+';
            else
                line += '-';
        }
        std::cout << line << std::endl;
        printf("|          |");
        for (const auto &symbol : symbol_set)
            printf("      %5s          |", symbol.c_str());
        std::cout << line << std::endl;
        for (int i = 0; i < states_number_; i++) {
            printf("| %-8d |", i);
            for (const std::string &symbol : symbol_set) {
                try {
                    cell action = function_.at({i, symbol});
                    if (action.first == 'a')
                        printf("        ACCEPT       |");
                    else
                        printf("          %c%-3d       |", action.first, action.second);
                } catch (const std::out_of_range &oor) {
                    printf("                     |");
                }
            }
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }

    bool LR0::Parse(bool verbose) {
        bool accept = false;
        std::vector<std::string> stack = {"0"};
        std::string input;
        if (verbose) {
            PrintParsingTable();
            std::cout << "Parsing process:" << std::endl << std::endl;
            std::cout << " STACK";
            for (int i = 0; i < 66; i++)
                std::cout << " ";
            std::cout << "| \tINPUT STRING";
            for (int i = 0; i < 59; i++)
                std::cout << " ";
            std::cout << "| ACTION" << std::endl;
            for (int i = 0; i < 187; i++)
                std::cout << "-";
            std::cout << std::endl;
        }

        auto current_token = tokenizer_.yylex();
        std::string saver = current_token.token_name;
        input += current_token.lexeme + " ";
        if (!grammar_.terminals().count(saver))
            saver = current_token.lexeme;

        while (true) {
            try {
                auto action = function_.at({stoi(stack.back()), saver});
                if (action.first == 'a') {
                    if (verbose) {
                        std::string stack_state;
                        std::string action_str = "ACCEPT";
                        for (const auto &str : stack)
                            stack_state += str + " ";
                        printf(" %-70s | \t%-70s | %s\n", stack_state.c_str(), input.c_str(), action_str.c_str());
                        for (int i = 0; i < 187; i++)
                            std::cout << "-";
                        std::cout << std::endl;
                    }
                    accept = true;
                    break;
                } else if (action.first == 's') {
                    if (verbose) {
                        std::string stack_state;
                        std::string action_str;
                        action_str += action.first;
                        action_str += std::to_string(action.second);
                        for (const auto &str : stack)
                            stack_state += str + " ";
                        printf(" %-70s | \t%-70s | %s\n", stack_state.c_str(), input.c_str(), action_str.c_str());
                        for (int i = 0; i < 187; i++)
                            std::cout << "-";
                        std::cout << std::endl;
                    }
                    stack.push_back(saver);
                    stack.push_back(std::to_string(action.second));
                    current_token = tokenizer_.yylex();
                    saver = current_token.token_name;
                    input += current_token.lexeme + " ";
                    if (!grammar_.terminals().count(saver))
                        saver = current_token.lexeme;
                } else if (action.first == 'r') {
                    auto rule = grammar_.GetRuleFromIndex(action.second);

                    int rule_size = 2 * (int) rule.second.size();
                    std::string var_symbol = rule.first;

                    if (verbose) {
                        std::string stack_state;
                        std::string action_str;
                        action_str += action.first;
                        action_str += std::to_string(action.second);
                        action_str += ": " + rule.first + " -> ";
                        for (const auto &symbol : rule.second)
                            action_str += symbol + " ";
                        for (const auto &str : stack)
                            stack_state += str + " ";
                        printf(" %-70s | \t%-70s | %s\n", stack_state.c_str(), input.c_str(), action_str.c_str());
                        for (int i = 0; i < 187; i++)
                            std::cout << "-";
                        std::cout << std::endl;
                    }

                    for (int i = 0; i < rule_size; ++i)
                        stack.pop_back();
                    int last_state = stoi(stack.back());
                    stack.push_back(var_symbol);
                    action = function_.at({last_state, var_symbol});
                    if (action.first == 'g') {
                        if (verbose) {
                            std::string stack_state;
                            std::string action_str;
                            action_str += 'g';
                            action_str += std::to_string(action.second);
                            for (const auto &str : stack)
                                stack_state += str + " ";
                            printf(" %-70s | \t%-70s | %s\n", stack_state.c_str(), input.c_str(), action_str.c_str());
                            for (int i = 0; i < 187; i++)
                                std::cout << "-";
                            std::cout << std::endl;
                        }
                        stack.push_back(std::to_string(action.second));
                    }
                }
            } catch (const std::out_of_range &oor) {
                break;
            }
        }
        return accept;
    }

    void LR0::ThrowConflictError(ConflictManager::Conflict c, const std::pair<ItemSet, int> &print_obj,
                                 const std::set<int> &rule_set, const std::string &symbol) {
        if (c == Conflict::kShiftReduceConflict) {
            std::cerr << std::endl << "In state " << std::to_string(print_obj.second) << ":" << std::endl;
            for (const auto &item : print_obj.first) {
                std::cerr << "\t" << std::to_string(grammar_.GetRuleIndex(item.variable, item.rule))
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
                std::cerr << "\t" << std::to_string(grammar_.GetRuleIndex(item.variable, item.rule))
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

