#include <set>
#include <iostream>
#include <cstdio>
#include <tuple>
#include <string>
#include "compiler/parser_algorithms/LL1.h"
#include <compiler/grammar_parser/grammar_array.h>

using namespace std;

LL1::LL1(TextSourceBuffer *input_file, lexical_analyzer& tokenizer) :
LL1(grammar_parser(input_file), tokenizer){}

LL1::LL1(grammar_parser parser, lexical_analyzer& tokenizer): tokenizer(tokenizer) {
    grammar_array rules_array = parser.parse_grammar();
    axiom = rules_array.get_axiom();
    non_terminals = rules_array.get_non_terminals();
    terminals = rules_array.get_terminals();
    terminals.insert("$");

    cout << "Non-terminals: ";
    for (const string& s : non_terminals)
        cout << s << " ";
    cout << "\nTerminals: ";
    for (const string& s : terminals)
        cout << s << " ";
    cout << endl;

    for (const string& variable : non_terminals) {
        set<vector<string>> variable_rules = rules_array[variable];
        for (const string& terminal_symbol : terminals) {
            for (const auto& rule : variable_rules) {
                set<string> first = rules_array.first(rule);

                if (!first.count("#")) {
                    if (first.count(terminal_symbol))
                        function.insert({{variable, terminal_symbol}, make_pair(rule, rules_array.get_rule_index(variable, rule))});
                } else {
                    set<string> follow = rules_array.follow(variable);
                    if (follow.count(terminal_symbol))
                        function.insert({{variable, terminal_symbol}, make_pair(rule, rules_array.get_rule_index(variable, rule))});
                }
            }
        }
    }

    for (const string& x : terminals) {
        for (const string& y : terminals) {
            if (x == y) {
                if (x == "$")
                    function.insert({{x, x}, "ACCEPT"});
                else
                    function.insert({{x, x}, "POP"});
            }
        }
    }
}

void LL1::print_function() {
    string line = "\n+----------+";
    for (int i = 1; i <= (int)(22 * terminals.size()); i++) {
        if (i % 22 == 0)
            line += '+';
        else
            line += '-';
    }
    cout << line << endl;
    printf("|          |");
    for (auto it = terminals.rbegin(); it != terminals.rend() ; it++)
        printf("      %5s          |", (*it).c_str());
    cout << line << endl;
    for (const string& non_terminal : non_terminals) {
        printf("| %-8s |", non_terminal.c_str());
        for (auto it = terminals.rbegin(); it != terminals.rend() ; it++) {
            try {
                auto cell = function.at(make_pair(non_terminal, *it));
                const auto pairPtr (std::get_if<pair<vector<string>, int>>(&cell));
                string result;
                for (const auto& expr : (*pairPtr).first)
                    result += " " + expr;
                printf("  (%-10s, %2d )  |", result.c_str(), (*pairPtr).second);
            } catch (const out_of_range &oor) {
                printf("                     |");
            }
        }
        cout << line << endl;
    }
    for (auto it = terminals.rbegin(); it != terminals.rend() ; it++) {
        printf("| %-8s |", (*it).c_str());
        for (auto it2 = terminals.rbegin(); it2 != terminals.rend() ; it2++) {
            try {
                auto cell = function.at(make_pair(*it, *it2));
                const auto pairStr (std::get_if<string>(&cell));
                if (*pairStr == "ACCEPT")
                    printf("        ACCEPT       |");
                else if (*pairStr == "POP")
                    printf("         POP         |");
            } catch (const out_of_range &oor) {
                printf("                     |");
            }
        }
        cout << line << endl;
    }
}

pair<vector<tuple<vector<string>, string, string>>, bool> LL1::parse() {
    bool accept = false;
    vector<tuple<vector<string>, string, string>> record;
    vector<string> stack = {"$", axiom};
    string input;

    auto current_token = tokenizer.yylex();
    string saver = current_token.get_token_name();
    input += current_token.get_lexeme() + " ";
    if(!terminals.count(saver))
        saver = current_token.get_lexeme();

    while (true) {
        try {
            auto action = function.at({stack.back(), saver});

            if (const auto strPtr (std::get_if<string>(&action)); strPtr) {
                record.emplace_back(stack, input, *strPtr);
                if (*strPtr == "ACCEPT") {
                    accept = true;
                    break;
                } else {
                    current_token = tokenizer.yylex();
                    saver = current_token.get_token_name();
                    input += current_token.get_lexeme() + " ";
                    if(!terminals.count(saver))
                        saver = current_token.get_lexeme();
                    stack.pop_back();
                }
            } else {
                const auto pairPtr (std::get_if<pair<vector<string>, int>>(&action));
                string action_str;
                for(const auto& str : (*pairPtr).first)
                    action_str += str + " ";
                action_str.pop_back();
                action_str += ", " + to_string((*pairPtr).second);

                record.emplace_back(stack, input, action_str);
                stack.pop_back();
                if ((*pairPtr).first.front() != "#")
                    stack.insert(stack.end(), (*pairPtr).first.rbegin(), (*pairPtr).first.rend());
            }
        } catch (const out_of_range &oor) {
            break;
        }
    }

    return {record, accept};
}
