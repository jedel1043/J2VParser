//
// Created by Jedel on 08/11/2019.
//

#ifndef COMPILER_GRAMMAR_PARSER_H
#define COMPILER_GRAMMAR_PARSER_H


#include <utility>
#include<error.h>

#include "grammar_analyzer.h"
#include "grammar_array.h"

class grammar_parser {
private:
    grammar_analyzer scanner;
    grammar_array parsed_grammar;

    void grammar();
    void rule();
    void right_side(const string& left_s);
    string symbol();

public:
    explicit grammar_parser(grammar_analyzer scanner) : scanner(std::move(scanner)) {};
    explicit grammar_parser(TextSourceBuffer *input_file) : scanner(input_file) {};

    grammar_array parse_grammar() {
        parsed_grammar = grammar_array();
        grammar();
        return parsed_grammar;
    }

};


#endif //COMPILER_GRAMMAR_PARSER_H
