#include <string>
#include <io/buffer.h>
#include <compiler/grammar_parser/grammar_analyzer.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

    TextSourceBuffer grammar("../prueba2.txt");
    grammar_analyzer analyzer(&grammar);

    Token t;
    while(!analyzer.is_in_end()) {
        t = analyzer.yylex();
        cout << "(" + t.get_lexeme() + ", " + t.get_token_name() + ") ";
    }
}
