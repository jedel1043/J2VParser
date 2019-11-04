//
// Created by asusi7 on 10/5/2019.
//

#include <algorithm>
#include <iostream>
#include <set>
#include "compiler/grammar_parser/grammar_array.h"


// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable (para usarse en el atributo collection.
vector<pair<int, string>> VariableArray::get_right_sides(const string &variable) {
    return right_sides[variable];
}

// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable y donde la variable no es la regla (para usarse en el atributo collection.
vector<pair<int, string>> VariableArray::get_right_sides_e(const string &variable) {
    vector<pair<int, string>> t = right_sides[variable];
    vector<pair<int, string>> ret;

    for(const pair<int, string>& temp : t){
        if(temp.second != variable){
            ret.push_back(temp);
        }
    }

    return ret;
}

//insercion a la coleccion de reglas.
void VariableArray::insert(const string &variable, const string &r_side) {
    bool isn_in_variables = true;

    for(string t : variables){
        if(t == variable)
            isn_in_variables = false;
    }

    if(isn_in_variables)
        variables.push_back(variable);

    for(string t : variables){
        if(r_side.find(t) != string::npos){
            right_sides[t].push_back(make_pair(collection.size(), variable));
        }
    }
    get_missing_rules(variable);
    collection_index[variable].push_back(collection.size());
    pair<int, string> key(collection.size(), variable);
    collection[key] = r_side;
}

void VariableArray::get_missing_rules(const string &variable) {
    map<pair<int, string>, string>::iterator it;

    for(it = collection.begin(); it != collection.end(); ++it){
        pair<int, string> key = it->first;
        string r_side = it->second;

        vector<pair<int, string>> r_sides_variable = right_sides[variable];

        if( (r_side.find(variable) != string::npos) && (find(r_sides_variable.begin(), r_sides_variable.end(), key) == r_sides_variable.end())){
            right_sides[variable].push_back(key);
        }
    }
}

map<string, vector<int>> VariableArray::get_collection_index() {
    return collection_index;
}

vector<string> VariableArray::get_variables() {
    return variables;
}

void VariableArray::print_collection_index(const string &variable) {
    vector<int> indexes = collection_index[variable];
    cout << variable;
    cout << " : ";
    for(int t : indexes){
        cout << t;
        cout << " ";
    }
}

void VariableArray::print_collection() {
    map<pair<int, string>, string>::iterator it;

    for(it = collection.begin(); it != collection.end(); ++it){
        pair<int, string> var = it->first;
        cout << ("(" + var.second);
        cout << ", " << var.first << ") -> ";
        cout << it->second << "\n";
    }
}

void VariableArray::print_right_sides(const string &variable) {
    cout << variable << " : ";
    auto r_sides = right_sides[variable];

    for(pair<int, string> t : r_sides){
        cout << "(" << t.first << ", " << t.second << ") ";
    }
}



bool VariableArray::is_nullable(const string& variable){
  for(auto rule : collection){
    if(rule.first.second == variable){
      if(rule.second.length() == 1 && rule.second[0] == '#')
        return true;
    }
  }
  return false;
}

set<string> VariableArray::first(const string& expression){
    set<string> first_result;
    if(count(variables.begin(), variables.end(), expression) == 0){
        first_result.insert(expression);
    }else{
        for(const auto& rule : collection){
            if(rule.first.second == expression){
                bool epsilon = true;
                for(char c : rule.second){
                  string first_symbol(1, c);
                  if(!is_nullable(first_symbol)){
                    if(count(variables.begin(), variables.end(), first_symbol) == 0){
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

set<string> VariableArray::follow(const string& expression){
    set<string> follow_result;
    if(expression == "E"){
        follow_result.insert("$");
    }
    vector<pair<int, string>> right_result = get_right_sides(expression);
    for(const auto& rule_f : right_result){
      string rule = collection[rule_f];

      for(int i=0; i<rule.length(); i++){
        if(string(1, rule[i]) == expression){
          vector<pair<int, string>> e_result;
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
            set<string> follow_e = follow(e_result_elem.second);
            for(string e_r : follow_e)
              follow_result.insert(e_r);
          }
        }
      }
    }
    return follow_result;
}
