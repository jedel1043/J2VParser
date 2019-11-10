#ifndef PARSER_H
#define PARSER_H

#include <utility>
#include <vector>
#include <automata/NFA.h>
#include <error.h>
#include <compiler/regex_scanner.h>

class Regex_Parser {
    set<char> any;
    Regex_Scanner scanner;
    vector<NFA> machines;

    void machine();

    NFA rule();

    void expr(NFA &);

    void cat_expr(NFA &);

    static bool isConcatenable(Regex_Token);

    void factor(NFA &);

    void term(NFA &);

    void discard(NFA &);

    void dash(set<char> &);

public:
    explicit Regex_Parser(Regex_Scanner scanner) : scanner(std::move(scanner)) {
        for (int i = ' '; i <= '}'; i++) any.insert(i);
    }

    vector<NFA> parse() {
        machine();
        return machines;
    }
};

#endif
