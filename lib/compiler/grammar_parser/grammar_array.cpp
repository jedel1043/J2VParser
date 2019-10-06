//
// Created by asusi7 on 10/5/2019.
//

#include <algorithm>
#include <iostream>
#include "compiler/grammar_parser/grammar_array.h"


// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable (para usarse en el atributo collection.
vector<pair<int, string>> grammar_array::get_right_sides(const string &variable) {
    return right_sides[variable];
}

// retorna una lista que contiene las llaves a los lados derechos donde aparece
// la variable y donde la variable no es la regla (para usarse en el atributo collection.
vector<pair<int, string>> grammar_array::get_right_sides_e(const string &variable) {
    vector<pair<int, string>> t = right_sides[variable];
    vector<pair<int, string>> ret;

    for (const pair<int, string> &temp : t) {
        if (temp.second != variable) {
            ret.push_back(temp);
        }
    }

    return ret;
}

//insercion a la coleccion de reglas.
void grammar_array::insert(const string &variable, const string &r_side) {
    bool isn_in_variables = true;

    for (const string &t : variables) {
        if (t == variable)
            isn_in_variables = false;
    }

    if (isn_in_variables)
        variables.push_back(variable);

    for (const string &t : variables) {
        if (r_side.find(t) != string::npos) {
            right_sides[t].push_back(make_pair(collection.size(), variable));
        }
    }
    get_missing_rules(variable);
    collection_index[variable].push_back(collection.size());
    pair<int, string> key(collection.size(), variable);
    collection[key] = r_side;
}

void grammar_array::get_missing_rules(const string &variable) {
    map<pair<int, string>, string>::iterator it;

    for (it = collection.begin(); it != collection.end(); ++it) {
        pair<int, string> key = it->first;
        string r_side = it->second;

        vector<pair<int, string>> r_sides_variable = right_sides[variable];

        if ((r_side.find(variable) != string::npos) &&
            (find(r_sides_variable.begin(), r_sides_variable.end(), key) == r_sides_variable.end())) {
            right_sides[variable].push_back(key);
        }
    }
}

map<string, vector<int>> grammar_array::get_collection_index() {
    return collection_index;
}

vector<string> grammar_array::get_variables() {
    return variables;
}

void grammar_array::print_collection_index(const string &variable) {
    vector<int> indexes = collection_index[variable];
    cout << variable;
    cout << " : ";
    for (int t : indexes) {
        cout << t;
        cout << " ";
    }
}

void grammar_array::print_collection() {
    map<pair<int, string>, string>::iterator it;

    for (it = collection.begin(); it != collection.end(); ++it) {
        pair<int, string> var = it->first;
        cout << ("(" + var.second);
        cout << ", " << var.first << ") -> ";
        cout << it->second << "\n";
    }
}

void grammar_array::print_right_sides(const string &variable) {
    cout << variable << " : ";
    auto r_sides = right_sides[variable];

    for (const pair<int, string> &t : r_sides) {
        cout << "(" << t.first << ", " << t.second << ") ";
    }
}
