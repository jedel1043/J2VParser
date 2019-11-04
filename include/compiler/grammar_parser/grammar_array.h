//
// Created by asusi7 on 10/5/2019.
//

#ifndef STRUCT_VARIABLEARRAY_H
#define STRUCT_VARIABLEARRAY_H

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <set>

using namespace std;

class VariableArray {
    string initial_variable;
    map<pair<int, string>, string> collection;
    map<string, vector<pair<int, string>>> right_sides;
    vector<string> variables;
    map<string, vector<int>> collection_index;
    void get_missing_rules(const string& variable);
    bool is_nullable(const string& variable);

public:
    vector<pair<int, string>> get_right_sides(const string& variable);
    void insert(const string& variable, const string& r_side);
    vector<pair<int, string>> get_right_sides_e(const string &variable);
    map<string, vector<int>> get_collection_index();
    vector<string> get_variables();
    void print_collection_index(const string& variable);
    void print_collection();
    void print_right_sides(const string& variable);

    set<string> first(const string& expression);
    set<string> follow(const string& expression);
};


#endif //STRUCT_VARIABLEARRAY_H
