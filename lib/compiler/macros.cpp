#include <iostream>
#include <map>
#include <cstdio>
#include <cstring>
#include <compiler/macros.h>
#include <misc.h>
#include <error.h>

using namespace std;

static map<string, string> macros;

void new_macro(string definition){
  char name[MACRO_NAME_MAX];
  char body[MACRO_TEXT_MAX];
  sscanf(definition.c_str(), "%s %[^\n]", name, body);
  macros.insert(make_pair(string(name), string(body)));
}

string expand_macro(string const macro){
  if(macro[0] != '{' || macro[macro.length() - 1] != '}') SyntaxError(BadMacro);
  char macro_name[MACRO_NAME_MAX];
  size_t length = macro.copy(macro_name, MACRO_NAME_MAX, 1);
  macro_name[length-1] = '\0';
  auto it = macros.find(string(macro_name));
  if(it==macros.end()) SyntaxError(MissingMacro);
  return it->second;
}

void print_macros(){
  auto iterator = macros.begin();
  for(; iterator!=macros.end(); ++iterator)
    cout << iterator->first << ": " << iterator->second << endl;
}
