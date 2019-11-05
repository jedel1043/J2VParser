//
// Created by asusi7 on 10/5/2019.
//

#ifndef STRUCT_VARIABLEARRAY_H
#define STRUCT_VARIABLEARRAY_H

#include <string>
#include <map>
#include <set>
#include <utility>
#include <set>

using namespace std;

class Grammar_Array {
    string initial_variable;
    map<pair<int, string>, string> collection;
    map<string, set<pair<int, string>>> right_sides;
    set<string> nonterminals;
    set<string> terminals;
    map<string, set<int>> collection_index;
    void get_missing_rules(const string& variable);
    bool is_nullable(const string& variable);

public:
    Grammar_Array(): initial_variable(""){}
    set<pair<int, string>> get_right_sides(const string& variable);
    void insert(const string& variable, const string& r_side);
    set<pair<int, string>> get_right_sides_e(const string &variable);
    map<string, set<int>> get_collection_index();
    set<string> get_nonterminals(){ return nonterminals; }
    set<string> get_terminals(){ return terminals; }
    void print_collection_index(const string& variable);
    void print_collection();
    void print_right_sides(const string& variable);
    string operator[](pair<int, string> index){ return collection.at(index); }
    map<int, string> operator[](string index);
    string initial(){ return initial_variable; }

    set<string> first(const string& expression);
    set<string> follow(const string& expression, set<string>& calculated);
};


#endif //STRUCT_VARIABLEARRAY_H
