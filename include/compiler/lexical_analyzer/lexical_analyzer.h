#ifndef COMPILER_LEXICAL_ANALYZER_H
#define COMPILER_LEXICAL_ANALYZER_H

#include <automata/DFA.h>
#include <string>
#include <utility>

class Token{
    string token_name;
    string lexeme;

public:
    Token() = default;

    Token(string tokenName, string lexeme) : token_name(std::move(tokenName)), lexeme(std::move(lexeme)) {}

    [[nodiscard]] const string &get_token_name() const {return token_name;}

    [[nodiscard]] const string &get_lexeme() const {return lexeme;}

    bool operator==(const string& token){return token_name == token;}
    bool operator!=(const string& token){return token_name != token;}
};

class lexical_analyzer {
protected:
    DFA automata;
    bool skip_whitespace;
    Token current_token;
public:
    lexical_analyzer(DFA automata, bool skip_whitespace):
    automata(std::move(automata)),
    skip_whitespace(skip_whitespace) {};

    virtual Token yylex() = 0;
    virtual bool is_in_end() = 0;
    virtual char skip_ws() = 0;

    Token get_current_token(){ return current_token;};
};


#endif //COMPILER_LEXICAL_ANALYZER_H
