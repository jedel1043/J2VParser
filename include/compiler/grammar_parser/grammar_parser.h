//
// Created by Jedel on 06/10/2019.
//

#ifndef COMPILER_GRAMMAR_PARSER_H
#define COMPILER_GRAMMAR_PARSER_H

#include <set>
#include "grammar_array.h"
#include "compiler/Grammar_Scanner.h"


class Grammar_Parser {
private:
    std::set<char> any;
    Grammar_Scanner scanner;
    Grammar_Array parsed_grammar;

    bool grammar();
    bool rule_list();
    bool rule();
    bool left(string & left_symbol);
    bool right_sides(string & left_symbol);
    bool symbols(string & right);
    bool symbols_p(string & right);

public:
    explicit Grammar_Parser(Grammar_Scanner scanner) : scanner(std::move(scanner)) {
        for (int i = ' '; i <= '}'; i++) any.insert(i);
    }

    Grammar_Array parse() {
        grammar();
        return parsed_grammar;
    }


    bool rule_list_p();

    bool right_sides_p(string &left_symbol);
};


#endif //COMPILER_GRAMMAR_PARSER_H
