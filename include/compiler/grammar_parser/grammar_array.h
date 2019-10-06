#ifndef STRUCT_GRAMMARARRAY_H
#define STRUCT_GRAMMARARRAY_H

#include <string>
#include <map>
#include <vector>
#include <utility>

using namespace std;

class grammar_array {
    map<pair<int, string>, string> collection;
    map<string, vector<pair<int, string>>> right_sides;
    vector<string> variables;
    map<string, vector<int>> collection_index;

    void get_missing_rules(const string &variable);

public:
    vector<pair<int, string>> get_right_sides(const string &variable);

    void insert(const string &variable, const string &r_side);

    vector<pair<int, string>> get_right_sides_e(const string &variable);

    map<string, vector<int>> get_collection_index();

    vector<string> get_variables();

    void print_collection_index(const string &variable);

    void print_collection();

    void print_right_sides(const string &variable);
};


#endif //STRUCT_GRAMMARARRAY_H
