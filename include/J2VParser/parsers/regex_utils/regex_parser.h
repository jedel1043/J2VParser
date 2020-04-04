#ifndef PARSER_H
#define PARSER_H

#include <utility>
#include <set>
#include <vector>

#include "J2VParser/automata/nfa.h"
#include "J2VParser/parsers/regex_utils/regex_scanner.h"

namespace compiler::regex {

    class RegexParser {
        std::set<char> any_char_;
        RegexScanner regex_scanner_;
        std::vector<automata::NFA> lexical_nfa_;

        void Machine();

        automata::NFA Rule();

        void Expr(automata::NFA &automaton);

        void CatExpr(automata::NFA &automaton);

        bool isConcatenable(RegexToken);

        void Factor(automata::NFA &automaton);

        void Term(automata::NFA &automaton);

        void Dash(std::set<char> &automaton);

    public:
        explicit RegexParser(RegexScanner scanner) : regex_scanner_(std::move(scanner)) {
            for (char i = ' '; i <= '}'; i++) any_char_.insert(i);
        }

        std::vector <automata::NFA> Parse() {
            Machine();
            return lexical_nfa_;
        }
    };
} // namespace compiler::regex

#endif
