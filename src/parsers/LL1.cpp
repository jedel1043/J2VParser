#include "J2VParser/parsers/LL1.h"

namespace compiler::parsers {

    LL1::LL1(io_buffer::TextSourceBuffer *input_file, analyzers::LexicalAnalyzer &tokenizer) :
            LL1(grammar::GrammarParser(input_file), tokenizer) {}

    LL1::LL1(grammar::GrammarParser parser, analyzers::LexicalAnalyzer &tokenizer) :
            Parser(tokenizer, parser.ParseGrammar()) {
        auto terminals = grammar_.terminals();
        terminals.insert("$");

        for (int i = 0; i < grammar_.size(); ++i) {
            auto rule = grammar_.GetRuleFromIndex(i);
            auto ls = rule.first;
            auto rs = rule.second;
            auto first = grammar_.First(rs);
            auto first_cpy = first;
            first_cpy.erase("#");
            for (const auto &symbol : first_cpy) {
                if (function_.count({ls, symbol})) {
                    int rule_i = function_[{ls, symbol}];
                    ThrowConflictError(Conflict::kFirstFirstConflict,
                                       {rule_i, grammar_.GetRuleIndex(ls, rs)},
                                       symbol);
                    number_of_conflicts_++;
                    continue;
                }
                function_[{ls, symbol}] = grammar_.GetRuleIndex(ls, rs);
            }
            if (first.count("#")) {
                auto follow = grammar_.Follow(ls);

                for (const auto &symbol : follow) {
                    if (function_.count({ls, symbol})) {
                        ThrowConflictError(Conflict::kFirstFollowConflict,
                                           {grammar_.GetRuleIndex(ls, rs)},
                                           symbol);
                        number_of_conflicts_++;
                        continue;
                    }
                    function_[{ls, symbol}] = grammar_.GetRuleIndex(ls, rs);
                }
            }
        }

        if (number_of_conflicts_ > 0) {
            std::cerr << "Found " + std::to_string(number_of_conflicts_) + " conflicts when creating the parsing table."
                      << std::endl;
            std::cerr << "Stopping parsing..." << std::endl << std::endl;
            exit(-1);
        }

        for (const std::string &x : grammar_.terminals())
            function_.insert({{x, x}, -1});
        function_.insert({{"$", "$"}, -2});
    }

    void LL1::PrintParsingTable() {
        std::cout << grammar_ << std::endl;
        auto terminals = grammar_.terminals();
        terminals.insert("$");
        auto non_terminals = grammar_.non_terminals();
        std::cout << std::endl << std::endl << "Parsing table:" << std::endl << std::endl;
        std::string line = "\n+----------+";
        for (int i = 1; i <= (int) (22 * terminals.size()); i++) {
            if (i % 22 == 0)
                line += '+';
            else
                line += '-';
        }
        std::cout << line << std::endl;
        printf("|          |");
        for (auto it = terminals.rbegin(); it != terminals.rend(); it++)
            printf("      %5s          |", (*it).c_str());
        std::cout << line << std::endl;
        for (const std::string &non_terminal : non_terminals) {
            printf("| %-8s |", non_terminal.c_str());
            for (auto it = terminals.rbegin(); it != terminals.rend(); it++) {
                try {
                    int cell = function_.at(make_pair(non_terminal, *it));
                    std::string result;
                    for (const auto &expr : grammar_.GetRuleFromIndex(cell).second)
                        result += " " + expr;
                    printf("  (%-10s, %2d )  |", result.c_str(), cell);
                } catch (const std::out_of_range &oor) {
                    printf("                     |");
                }
            }
            std::cout << line << std::endl;
        }
        for (auto it = terminals.rbegin(); it != terminals.rend(); it++) {
            printf("| %-8s |", (*it).c_str());
            for (auto it2 = terminals.rbegin(); it2 != terminals.rend(); it2++) {
                try {
                    int cell = function_.at(make_pair(*it, *it2));
                    if (cell == -2)
                        printf("        ACCEPT       |");
                    else if (cell == -1)
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
        auto terminals = grammar_.terminals();
        terminals.insert("$");
        bool accept = false;
        std::vector<std::string> stack = {"$", grammar_.axiom()};
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
        if (!terminals.count(saver))
            saver = current_token.lexeme;

        while (true) {
            try {
                int action = function_.at({stack.back(), saver});

                if (action < 0) {
                    if (action == -2) {
                        if (verbose) {
                            std::string stack_state;
                            for (const auto &str : stack)
                                stack_state += str + " ";
                            printf(" %-70s | \t%-70s | (%s)\n", stack_state.c_str(), input.c_str(), "ACCEPT");
                        }
                        accept = true;
                        break;
                    } else {
                        if (verbose) {
                            std::string stack_state;
                            for (const auto &str : stack)
                                stack_state += str + " ";
                            printf(" %-70s | \t%-70s | (%s)\n", stack_state.c_str(), input.c_str(), "POP");
                            for (int i = 0; i < 187; i++)
                                std::cout << "-";
                            std::cout << std::endl;
                        }
                        current_token = tokenizer_.yylex();
                        saver = current_token.token_name;
                        input += current_token.lexeme + " ";
                        if (!terminals.count(saver))
                            saver = current_token.lexeme;
                        stack.pop_back();
                    }
                } else {
                    const auto rule = grammar_.GetRuleFromIndex(action);
                    if (verbose) {
                        std::string action_str;
                        for (const auto &str : rule.second)
                            action_str += str + " ";
                        action_str.pop_back();
                        action_str += ", " + std::to_string(action);
                        std::string stack_state;
                        for (const auto &str : stack)
                            stack_state += str + " ";
                        printf(" %-70s | \t%-70s | (%s)\n", stack_state.c_str(), input.c_str(), action_str.c_str());
                        for (int i = 0; i < 187; i++)
                            std::cout << "-";
                        std::cout << std::endl;
                    }
                    stack.pop_back();
                    if (rule.second.front() != "#")
                        stack.insert(stack.end(), rule.second.rbegin(), rule.second.rend());
                }
            } catch (const std::out_of_range &oor) {
                break;
            }
        }
        return accept;
    }

    void LL1::ThrowConflictError(Conflict c, const std::vector<int> &print_obj, const std::string &symbol) {
        if (c == Conflict::kFirstFirstConflict) {
            std::cerr << std::endl << "Found First/First conflict for symbol '" + symbol + "' caused by production(s): "
                      << std::endl;
            for (auto i : print_obj) {
                const auto rule = grammar_.GetRuleFromIndex(i);
                std::cerr << "\t" + std::to_string(i) + ".- " + rule.first << " -> ";
                for (const auto &str_symbol : rule.second)
                    std::cerr << str_symbol + " ";
                std::cerr << std::endl;
            }
            std::cerr << std::endl;
        } else if (c == Conflict::kFirstFollowConflict) {
            std::cerr << std::endl
                      << "Found First/Follow conflict for symbol '" + symbol + "' caused by production(s): "
                      << std::endl;
            for (auto i : print_obj) {
                const auto rule = grammar_.GetRuleFromIndex(i);
                std::cerr << "\t" + std::to_string(i) + ".- " + rule.first << " -> ";
                for (const auto &str_symbol : rule.second)
                    std::cerr << str_symbol + " ";
                std::cerr << std::endl;
            }
            std::cerr << std::endl;
        }
    }
} // namespace compiler::parsers
