#include <set>
#include <iostream>
#include <cstdio>
#include <tuple>
#include <string>
#include <compiler/LL1.h>
#include <compiler/grammar_parser/grammar_array.h>

using namespace std;

LL1::LL1(Grammar_Parser& parser): parser(parser){
  Grammar_Array rules = parser.parse();
  nonterminals = rules.get_nonterminals();
  terminals = rules.get_terminals();
  terminals.insert("$");

  cout << "Nonterminals: ";
  for(string s : nonterminals)
    cout << s << " ";
  cout << "\nTerminals: ";
  for(string s : terminals)
    cout << s << " ";
  cout << endl;

  for(string x : nonterminals){
    map<int, string> right_sides = rules[x];
    for(string y : terminals){
      for(const auto rs : right_sides){
        set<string> first = rules.first(string(1, rs.second[0]));

        if(first.find("#") == first.end()){
          if(first.find(y) != first.end()){
            function.insert(make_pair(make_pair(x, y), make_pair(rs.second, rs.first)));
            //cout << "(" << x << ", " << y << ") = " << "(" << rs.second << ", " << rs.first << ")" << endl;
          }
        }else{
          set<string> calculated;
          set<string> follow = rules.follow(x, calculated);
          if(follow.find(y) != follow.end()){
            function.insert(make_pair(make_pair(x, y), make_pair(rs.second, rs.first)));
            //cout << "(" << x << ", " << y << ") = " << "(" << rs.second << ", " << rs.first << ")" << endl;
          }
        }
      }
    }
  }

  for(string x : terminals){
    for(string y : terminals){
      if(x == y){
        if(x == "$")
          function.insert(make_pair(make_pair(x, x), make_pair("ACCEPT", -1)));
        else
          function.insert(make_pair(make_pair(x, x), make_pair("POP", -1)));
      }
    }
  }

  initial_variable = rules.initial();
}

void LL1::print_function(){
  string line = "\n+---+";
  for(int i=1; i<=(16*terminals.size()); i++){
    if(i%16 == 0)
      line += '+';
    else
      line += '-';
  }
  cout << line << endl;
  printf("|   |");
  for(string terminal : terminals)
    printf("       %c       |", terminal[0]);
  cout << line << endl;
  for(string nonterminal : nonterminals){
    printf("| %c |", nonterminal[0]);
    for(string terminal : terminals){
      try{
        pair<string, int> v = function.at(make_pair(nonterminal, terminal));
        printf("  (%5s, %2d)  |", v.first.c_str(), v.second);
      }catch (const out_of_range& oor){
        printf("               |");
      }
    }
    cout << line << endl;
  }
  for(string terminal1 : terminals){
    printf("| %c |", terminal1[0]);
    for(string terminal : terminals){
      try{
        pair<string, int> v = function.at(make_pair(terminal1, terminal));
        if(v.first == "ACCEPT")
          printf("    ACCEPT     |");
        else if(v.first == "POP")
          printf("      POP      |");
      }catch (const out_of_range& oor){
        printf("               |");
      }
    }
    cout << line << endl;
  }
}

bool LL1::parse(string& str, vector<tuple<string, string, string, string>>& record){
  bool accept = false;
  string matched = "";
  string todo = initial_variable + "$";
  string input = str + "$";

  while(true){
    try{
      pair<string, int> action = function.at(make_pair(todo.substr(0,1), input.substr(0,1)));
      record.push_back(make_tuple(matched, todo, input, action.first));
      if(action.second == -1){
        if(action.first == "ACCEPT"){
          accept = true;
          break;
        }else{
          matched += input[0];
          input = input.substr(1, input.length());
          todo = todo.substr(1, todo.length());
        }
      }else{
        if(action.first != "#")
          todo = action.first + todo.substr(1, todo.length());
        else
          todo = todo.substr(1, todo.length());
      }
    }catch(const out_of_range& oor){
      break;
    }
  }

  return accept;
}
