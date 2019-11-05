#ifndef LL1_H
#define LL1_H

#include <compiler/grammar_parser/grammar_parser.h>
#include <compiler/grammar_parser/grammar_array.h>
#include <map>
#include <tuple>
#include <vector>

using namespace std;

class LL1 {
private:
  Grammar_Parser parser;
  map<pair<string, string>, pair<string, int>> function;
  set<string> terminals;
  set<string> nonterminals;
  string initial_variable;

public:
  LL1(Grammar_Parser& parser);
  void print_function();
  bool parse(string& str, vector<tuple<string, string, string, string>>& record);
};

#endif
