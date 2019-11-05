//
// Created by asusi7 on 10/5/2019.
//

#include <algorithm>
#include <iostream>
#include <set>
#include "compiler/grammar_parser/grammar_array.h"


// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable (para usarse en el atributo collection.
set<pair<int, string>> Grammar_Array::get_right_sides(const string &variable) {
    return right_sides[variable];
}

// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable y donde la variable no es la regla (para usarse en el atributo collection.
set<pair<int, string>> Grammar_Array::get_right_sides_e(const string &variable) {
    set<pair<int, string>> t = right_sides[variable];
    set<pair<int, string>> ret;

    for(const pair<int, string>& temp : t){
        if(temp.second != variable){
            ret.insert(temp);
        }
    }

    return ret;
}

map<int, string> Grammar_Array::operator[](string index){
  map<int, string> result;
  set<int> n_rules = collection_index.at(index);
  for(int n_rule : n_rules)
    result.insert(make_pair(n_rule, collection.at(make_pair(n_rule, index))));
  return result;
}

//insercion a la coleccion de reglas.
void Grammar_Array::insert(const string &variable, const string &r_side) {
    if(initial_variable == "")
      initial_variable = variable;
    bool isn_in_variables = true;

    for(string t : nonterminals){
        if(t == variable)
            isn_in_variables = false;
    }

    if(isn_in_variables){
        nonterminals.insert(variable);
        set<string>::iterator it = find(terminals.begin(), terminals.end(), variable);
        if(it != terminals.end())
          terminals.erase(it);
    }

    for(string t : nonterminals){
        if(r_side.find(t) != string::npos){
            right_sides[t].insert(make_pair(collection.size(), variable));
        }
    }
    for(char c : r_side){
      if(c == '#') continue;
      string t = string(1, c);
      set<string>::iterator it = find(terminals.begin(), terminals.end(), t);
      set<string>::iterator it2 = find(nonterminals.begin(), nonterminals.end(), t);
      if(it == terminals.end() && it2 == nonterminals.end())
        terminals.insert(t);
    }
    get_missing_rules(variable);
    collection_index[variable].insert(collection.size());
    pair<int, string> key(collection.size(), variable);
    collection[key] = r_side;
}

void Grammar_Array::get_missing_rules(const string &variable) {
    map<pair<int, string>, string>::iterator it;

    for(it = collection.begin(); it != collection.end(); ++it){
        pair<int, string> key = it->first;
        string r_side = it->second;

        set<pair<int, string>> r_sides_variable = right_sides[variable];

        if( (r_side.find(variable) != string::npos) && (find(r_sides_variable.begin(), r_sides_variable.end(), key) == r_sides_variable.end())){
            right_sides[variable].insert(key);
        }
    }
}

map<string, set<int>> Grammar_Array::get_collection_index() {
    return collection_index;
}

void Grammar_Array::print_collection_index(const string &variable) {
    set<int> indexes = collection_index[variable];
    cout << variable;
    cout << " : ";
    for(int t : indexes){
        cout << t;
        cout << " ";
    }
}

void Grammar_Array::print_collection() {
    map<pair<int, string>, string>::iterator it;

    for(it = collection.begin(); it != collection.end(); ++it){
        pair<int, string> var = it->first;
        cout << ("(" + var.second);
        cout << ", " << var.first << ") -> ";
        cout << it->second << "\n";
    }
}

void Grammar_Array::print_right_sides(const string &variable) {
    cout << variable << " : ";
    auto r_sides = right_sides[variable];

    for(pair<int, string> t : r_sides){
        cout << "(" << t.first << ", " << t.second << ") ";
    }
}



bool Grammar_Array::is_nullable(const string& variable){
  for(auto rule : collection){
    if(rule.first.second == variable){
      if(rule.second.length() == 1 && rule.second[0] == '#')
        return true;
    }
  }
  return false;
}

set<string> Grammar_Array::first(const string& expression){
    set<string> first_result;
    if(count(nonterminals.begin(), nonterminals.end(), expression) == 0){
        first_result.insert(expression);
    }else{
        for(const auto& rule : collection){
            if(rule.first.second == expression){
                bool epsilon = true;
                for(char c : rule.second){
                  string first_symbol(1, c);
                  if(!is_nullable(first_symbol)){
                    if(count(nonterminals.begin(), nonterminals.end(), first_symbol) == 0){
                        first_result.insert(first_symbol);
                    }else{
                        set<string> first_return = first(first_symbol);
                        first_result.insert(first_return.begin(), first_return.end());
                    }
                    epsilon = false;
                    break;
                  }
                }
                if(epsilon) first_result.insert("#");
            }
        }
    }
    return first_result;
}

set<string> Grammar_Array::follow(const string& expression, set<string>& calculated){
    set<string> follow_result;

    if(calculated.find(expression) != calculated.end())
      return follow_result;
    else
      calculated.insert(expression);

    if(expression == initial_variable){
        follow_result.insert("$");
    }
    set<pair<int, string>> right_result = get_right_sides(expression);
    for(auto rule_f : right_result){
      string rule = collection[rule_f];

      for(int i=0; i<rule.length(); i++){
        if(string(1, rule[i]) == expression){
          set<pair<int, string>> e_result;
          if(i != (rule.length()-1)){
            set<string> first_result;
            first_result = first(string(1, rule[i+1]));
            for(string first_elem : first_result){
              if(first_elem != "#"){
                follow_result.insert(first_elem);
              }else{
                e_result = get_right_sides_e(expression);
              }
            }
          }else{
            e_result = get_right_sides_e(expression);
          }

          for(auto e_result_elem : e_result){
            set<string> follow_e = follow(e_result_elem.second, calculated);
            for(string e_r : follow_e)
              follow_result.insert(e_r);
          }
        }
      }
    }
    return follow_result;
}
