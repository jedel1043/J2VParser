#include "parsers/parser_algorithms/LL1.h"

#include <set>
#include <iostream>
#include <cstdio>
#include <tuple>
#include <string>

namespace compiler::parsers {

    LL1::LL1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer) :
            LL1(grammar::GrammarParser(input_file), tokenizer) {}

    LL1::LL1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer) :
    tokenizer_(tokenizer) {
        grammar::GrammarArray rules_array = parser.ParseGrammar();
        axiom_ = rules_array.axiom();
        non_terminals_ = rules_array.non_terminals();
        terminals_ = rules_array.terminals();
        terminals_.insert("$");

        for (const std::string &variable : non_terminals_) {
            std::set <std::vector<std::string>> variable_rules = rules_array[variable];
            for (const std::string &terminal_symbol : terminals_) {
                for (const auto &rule : variable_rules) {
                    std::set <std::string> first = rules_array.First(rule);

                    if (!first.count("#")) {
                        if (first.count(terminal_symbol))
                            function_.insert({{variable, terminal_symbol},
                                              make_pair(rule, rules_array.GetRuleIndex(variable, rule))});
                    } else {
                        std::set <std::string> follow = rules_array.Follow(variable);
                        if (follow.count(terminal_symbol))
                            function_.insert({{variable, terminal_symbol},
                                              make_pair(rule, rules_array.GetRuleIndex(variable, rule))});
                    }
                }
            }
        }

        for (const std::string &x : terminals_) {
            for (const std::string &y : terminals_) {
                if (x == y) {
                    if (x == "$")
                        function_.insert({{x, x}, "ACCEPT"});
                    else
                        function_.insert({{x, x}, "POP"});
                }
            }
        }
    }

    void LL1::PrintParsingTable() {
        std::cout << std::endl << std::endl << "Parsing table:" << std::endl << std::endl;
        std::string line = "\n+----------+";
        for (int i = 1; i <= (int) (22 * terminals_.size()); i++) {
            if (i % 22 == 0)
                line += '+';
            else
                line += '-';
        }
        std::cout << line << std::endl;
        printf("|          |");
        for (auto it = terminals_.rbegin(); it != terminals_.rend(); it++)
            printf("      %5s          |", (*it).c_str());
        std::cout << line << std::endl;
        for (const std::string &non_terminal : non_terminals_) {
            printf("| %-8s |", non_terminal.c_str());
            for (auto it = terminals_.rbegin(); it != terminals_.rend(); it++) {
                try {
                    auto cell = function_.at(make_pair(non_terminal, *it));
                    const auto pairPtr(std::get_if<std::pair < std::vector < std::string>, int >> (&cell));
                    std::string result;
                    for (const auto &expr : (*pairPtr).first)
                        result += " " + expr;
                    printf("  (%-10s, %2d )  |", result.c_str(), (*pairPtr).second);
                } catch (const std::out_of_range &oor) {
                    printf("                     |");
                }
            }
            std::cout << line << std::endl;
        }
        for (auto it = terminals_.rbegin(); it != terminals_.rend(); it++) {
            printf("| %-8s |", (*it).c_str());
            for (auto it2 = terminals_.rbegin(); it2 != terminals_.rend(); it2++) {
                try {
                    auto cell = function_.at(make_pair(*it, *it2));
                    const auto pairStr(std::get_if<std::string>(&cell));
                    if (*pairStr == "ACCEPT")
                        printf("        ACCEPT       |");
                    else if (*pairStr == "POP")
                        printf("         POP         |");
                } catch (const std::out_of_range &oor) {
                    printf("                     |");
                }
            }
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }

     bool LL1::Parse(bool verbose) {
        bool accept = false;
        std::vector <std::string> stack = {"$", axiom_};
        std::string input;

        if (verbose){
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
        if (!terminals_.count(saver))
            saver = current_token.lexeme;

        while (true) {
            try {
                auto action = function_.at({stack.back(), saver});

                if (const auto strPtr(std::get_if<std::string>(&action)); strPtr) {
                    if(verbose) {
                        std::string stack_state;
                        for(const auto& str : stack)
                            stack_state += str + " ";
                        printf(" %-70s | \t%-70s | (%s)\n", stack_state.c_str(), input.c_str(), (*strPtr).c_str());
                    }
                    if (*strPtr == "ACCEPT") {
                        accept = true;
                        break;
                    } else {
                        current_token = tokenizer_.yylex();
                        saver = current_token.token_name;
                        input += current_token.lexeme + " ";
                        if (!terminals_.count(saver))
                            saver = current_token.lexeme;
                        stack.pop_back();
                    }
                } else {
                    const auto pairPtr(std::get_if<std::pair < std::vector < std::string>, int >> (&action));
                    std::string action_str;
                    for (const auto &str : (*pairPtr).first)
                        action_str += str + " ";
                    action_str.pop_back();
                    action_str += ", " + std::to_string((*pairPtr).second);
                    if(verbose) {
                        std::string stack_state;
                        for(const auto& str : stack)
                            stack_state += str + " ";
                        printf(" %-70s | \t%-70s | (%s)\n", stack_state.c_str(), input.c_str(), action_str.c_str());
                    }
                    stack.pop_back();
                    if ((*pairPtr).first.front() != "#")
                        stack.insert(stack.end(), (*pairPtr).first.rbegin(), (*pairPtr).first.rend());
                }
            } catch (const std::out_of_range &oor) {
                break;
            }
        }

        return accept;
    }
} // namespace compiler::parsers
