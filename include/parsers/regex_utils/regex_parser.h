#ifndef PARSER_H
#define PARSER_H

#include <utility>
#include <vector>

#include "automata/nfa.h"
#include "parsers/regex_utils/regex_scanner.h"

namespace compiler::regex {

    class RegexParser {
        std::set<char> any_char_;
        RegexScanner regex_scanner_;
        std::vector <automata::NFA> lexical_nfa_;

        void Machine();

        automata::NFA Rule();

        void Expr(automata::NFA &automata);

        void CatExpr(automata::NFA &automata);

        static bool isConcatenable(RegexToken);

        void Factor(automata::NFA &automata);

        void Term(automata::NFA &automata);

        void Dash(std::set<char> &automata);

    public:
        explicit RegexParser(RegexScanner scanner) : regex_scanner_(std::move(scanner)) {
            for (int i = ' '; i <= '}'; i++) any_char_.insert(i);
        }

        std::vector <automata::NFA> Parse() {
            Machine();
            return lexical_nfa_;
        }
    };
} // namespace compiler::regex

#endif
