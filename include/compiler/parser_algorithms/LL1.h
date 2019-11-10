#ifndef LL1_H
#define LL1_H

#include <compiler/grammar_parser/grammar_array.h>
#include <compiler/grammar_parser/grammar_parser.h>
#include <map>
#include <tuple>
#include <vector>
#include <variant>

using namespace std;

class LL1 {
private:
    map<pair<string, string>, variant<string, pair<vector<string>, int>>> function;
    set<string> terminals;
    set<string> non_terminals;
    string axiom;
    lexical_analyzer &tokenizer;

public:
    explicit LL1(grammar_parser parser, lexical_analyzer &tokenizer);

    explicit LL1(TextSourceBuffer *input_file, lexical_analyzer &tokenizer);

    void print_function();

    pair<vector<tuple<vector<string>, string, string>>, bool> parse();
};

#endif
