#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <automata/NFA.h>
#include <error.h>
#include <compiler/scanner.h>

class Parser{
  set<char> any;
  Scanner scanner;
  vector<NFA> machines;

  void machine();
  NFA rule();
  void expr(NFA&);
  void cat_expr(NFA&);
  bool isConcatenable(Token);
  void factor(NFA&);
  void term(NFA&);
  void discard(NFA&);
  void dash(set<char>&);

public:
  Parser(Scanner scanner): scanner(scanner){
    for(int i=' '; i<='}'; i++) any.insert(i);
  }
  vector<NFA> parse(){
    machine();
    return machines;
  }
};

#endif
