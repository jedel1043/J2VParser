#ifndef J2VP_LEXICAL_ANALYZER_H
#define J2VP_LEXICAL_ANALYZER_H

#include <string>
#include <utility>

#include "J2VParser/automata/dfa.h"

namespace compiler::analyzers {

    struct Token {
        std::string token_name;
        std::string lexeme;

        bool operator==(const std::string &token) const { return token_name == token; }
        bool operator!=(const std::string &token) const { return token_name != token; }
    };

    class LexicalAnalyzer {
    protected:
        automata::DFA automaton_;
        bool skip_whitespace_;
        Token current_token_;
    public:
        LexicalAnalyzer(automata::DFA automaton, bool skip_whitespace) :
                automaton_(std::move(automaton)),
                skip_whitespace_(skip_whitespace) {};

        virtual Token yylex() = 0;

        virtual bool isInEnd() = 0;

        virtual char SkipWS() = 0;

        Token current_token() { return current_token_; };
    };
} //namespace compiler::analyzers


#endif //J2VP_LEXICAL_ANALYZER_H
