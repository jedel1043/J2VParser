#include <iostream>
#include <compiler/grammar_parser/grammar_parser.h>
#include "compiler/grammar_parser/grammar_array.h"


using namespace std;

int main() {
    TextSourceBuffer input("../prueba2.txt");
    grammar_parser gp(&input);
    grammar_array test = gp.parse_grammar();

    cout << test << std::endl;

    cout << "{";
    std::vector<std::string> test_v = {"Tp", "E"};
    for(const auto& it : test.follow("F"))
        cout << it << ", ";
    cout << "}" << std::endl;

    return 0;
}