#ifndef MACROS_H
#define MACROS_H

#include <cstring>

using namespace std;

void new_macro(const string &definition);

string expand_macro(string const &macro);

void print_macros();

#endif
