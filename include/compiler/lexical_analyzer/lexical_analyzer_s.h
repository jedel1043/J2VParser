//
// Created by Jedel on 08/11/2019.
//

#ifndef COMPILER_LEXICAL_ANALYZER_S_H
#define COMPILER_LEXICAL_ANALYZER_S_H

#include <automata/DFA.h>
#include <string>
#include <utility>
#include "lexical_analyzer.h"


class lexical_analyzer_s : public lexical_analyzer{
private:
    std::string str_input;
    std::string::iterator str_pos;
public:
    lexical_analyzer_s(DFA automata, std::string strInput, bool skip_whitespace = true);
    explicit lexical_analyzer_s(DFA automata, bool skip_whitespace = true) :
    lexical_analyzer(std::move(automata), skip_whitespace){};

    void set_str_input(const string & new_str);

    Token yylex() override;

    bool is_in_end() override;

    char skip_ws() override;
};


#endif //COMPILER_LEXICAL_ANALYZER_S_H
