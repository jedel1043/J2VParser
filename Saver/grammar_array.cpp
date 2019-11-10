#include <algorithm>
#include <iostream>
#include <set>
#include "compiler/grammar_parser/grammar_array.h"


// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable (para usarse en el atributo collection.
set<pair<int, string>> grammar_array::get_right_sides(const string &variable) {
    return right_sides[variable];
}

// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable y donde la variable no es la regla (para usarse en el atributo collection.
set<pair<int, string>> grammar_array::get_right_sides_e(const string &variable) {
    set<pair<int, string>> t = right_sides[variable];
    set<pair<int, string>> ret;

    for(const pair<int, string>& temp : t){
        if(temp.second != variable){
            ret.insert(temp);
        }
    }

    return ret;
}

map<int, string> grammar_array::operator[](const string& index){
  map<int, string> result;
  set<int> n_rules = collection_index.at(index);
  for(int n_rule : n_rules)
    result.insert(make_pair(n_rule, collection.at(make_pair(n_rule, index))));
  return result;
}

//insercion a la coleccion de reglas.
void grammar_array::insert(const string &variable, const string &r_side) {
    if(initial_variable.empty())
      initial_variable = variable;
    bool not_in_variables = true;

    for(const string& t : non_terminals){
        if(t == variable)
            not_in_variables = false;
    }

    if(not_in_variables){
        non_terminals.insert(variable);
        auto it = find(terminals.begin(), terminals.end(), variable);
        if(it != terminals.end())
          terminals.erase(it);
    }

    for(const string& t : non_terminals){
        if(r_side.find(t) != string::npos){
            right_sides[t].insert(make_pair(collection.size(), variable));
        }
    }
    for(char c : r_side){
      if(c == '#') continue;
      string t = string(1, c);
      auto it = find(terminals.begin(), terminals.end(), t);
      auto it2 = find(non_terminals.begin(), non_terminals.end(), t);
      if(it == terminals.end() && it2 == non_terminals.end())
        terminals.insert(t);
    }
    get_missing_rules(variable);
    collection_index[variable].insert(collection.size());
    pair<int, string> key(collection.size(), variable);
    collection[key] = r_side;
}

void grammar_array::get_missing_rules(const string &variable) {
    map<pair<int, string>, string>::iterator it;

    for(it = collection.begin(); it != collection.end(); ++it){
        pair<int, string> key = it->first;
        string r_side = it->second;

        set<pair<int, string>> r_sides_variable = right_sides[variable];

        if( (r_side.find(variable) != string::npos) && (r_sides_variable.find(key) == r_sides_variable.end())){
            right_sides[variable].insert(key);
        }
    }
}

map<string, set<int>> grammar_array::get_collection_index() {
    return collection_index;
}

void grammar_array::print_collection_index(const string &variable) {
    set<int> indexes = collection_index[variable];
    cout << variable;
    cout << " : ";
    for(int t : indexes){
        cout << t;
        cout << " ";
    }
}

void grammar_array::print_collection() {
    map<pair<int, string>, string>::iterator it;

    for(it = collection.begin(); it != collection.end(); ++it){
        pair<int, string> var = it->first;
        cout << ("(" + var.second);
        cout << ", " << var.first << ") -> ";
        cout << it->second << "\n";
    }
}

void grammar_array::print_right_sides(const string &variable) {
    cout << variable << " : ";
    auto r_sides = right_sides[variable];

    for(const pair<int, string>& t : r_sides){
        cout << "(" << t.first << ", " << t.second << ") ";
    }
}



bool grammar_array::is_nullable(const string& variable){
  for(auto rule : collection){
    if(rule.first.second == variable){
      if(rule.second.length() == 1 && rule.second[0] == '#')
        return true;
    }
  }
  return false;
}

set<string> grammar_array::first(const string& expression){
    set<string> first_result;
    if(count(non_terminals.begin(), non_terminals.end(), expression) == 0){
        first_result.insert(expression);
    }else{
        for(const auto& rule : collection){
            if(rule.first.second == expression){
                bool epsilon = true;
                for(char c : rule.second){
                  string first_symbol(1, c);
                  if(!is_nullable(first_symbol)){
                    if(count(non_terminals.begin(), non_terminals.end(), first_symbol) == 0){
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

set<string> grammar_array::follow(const string& expression, set<string>& calculated){
    set<string> follow_result;

    if(calculated.find(expression) != calculated.end())
      return follow_result;
    else
      calculated.insert(expression);

    if(expression == initial_variable){
        follow_result.insert("$");
    }
    set<pair<int, string>> right_result = get_right_sides(expression);
    for(const auto& rule_f : right_result){
      string rule = collection[rule_f];

      for(int i=0; i<(int)rule.length(); i++){
        if(string(1, rule[i]) == expression){
          set<pair<int, string>> e_result;
          if(i != (rule.length()-1)){
            set<string> first_result;
            first_result = first(string(1, rule[i+1]));
            for(const string& first_elem : first_result){
              if(first_elem != "#"){
                follow_result.insert(first_elem);
              }else{
                e_result = get_right_sides_e(expression);
              }
            }
          }else{
            e_result = get_right_sides_e(expression);
          }

          for(const auto& e_result_elem : e_result){
            set<string> follow_e = follow(e_result_elem.second, calculated);
            for(const string& e_r : follow_e)
              follow_result.insert(e_r);
          }
        }
      }
    }
    return follow_result;
}
