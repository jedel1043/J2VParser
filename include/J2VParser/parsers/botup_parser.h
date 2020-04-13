#ifndef J2VPARSER_BOTUP_PARSER_H
#define J2VPARSER_BOTUP_PARSER_H

#include "J2VParser/buffer.h"
#include "J2VParser/parsers/parser.h"
#include "J2VParser/parsers/grammar/grammar_parser.h"
#include "J2VParser/parsers/grammar/grammar_array.h"

#include <utility>
#include <string>

namespace J2VParser::parsers {
    template<typename U>
    class BotUpParser : public Parser {
    protected:
        using cell = std::pair<char, int>;
        using Item = U;
        using ItemSet = std::set<Item>;

        std::map<std::pair<int, std::string>, std::pair<char, int>> function_;
        int states_number_ = 0;

    public:
        BotUpParser(const grammar::GrammarArray &grammarArray, analyzers::LexicalAnalyzer &lexicalAnalyzer,
                    bool augment_grammar) :
                Parser(augment_grammar ?
                       grammarArray.GetAugmentedGrammar() :
                       grammarArray, lexicalAnalyzer) {};

        BotUpParser(io_buffer::TextSourceBuffer &grammar_f, analyzers::LexicalAnalyzer &lexicalAnalyzer,
                    bool augment_grammar) :
                Parser(augment_grammar ?
                       grammar::ParseGrammar(grammar_f).GetAugmentedGrammar() :
                       grammar::ParseGrammar(grammar_f),
                       lexicalAnalyzer) {};

        void PrintParsingTable() override {
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

        bool Parse(bool verbose) override {
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

            auto current_token = tokenizer_.jvly();
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
                            std::cout << std::string(187, '-');
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
                            std::cout << std::string(187, '-');
                            std::cout << std::endl;
                        }
                        stack.push_back(saver);
                        stack.push_back(std::to_string(action.second));
                        current_token = tokenizer_.jvly();
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
                            std::cout << std::string(187, '-');
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
                                printf(" %-70s | \t%-70s | %s\n", stack_state.c_str(), input.c_str(),
                                       action_str.c_str());
                                std::cout << std::string(187, '-');
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
    };
} // namespace J2VParser::parsers
#endif //J2VPARSER_BOTUP_PARSER_H
