#include <iostream>
#include <vector>
#include <io/buffer.h>
#include <error.h>
#include <compiler/macros.h>
#include <compiler/Grammar_Scanner.h>
#include <compiler/grammar_parser/grammar_parser.h>
#include <compiler/grammar_parser/grammar_array.h>
#include <automata/NFA.h>
#include <compiler/LL1.h>
#include <string>
#include <tuple>

using namespace std;

int main(int argc, char const *argv[]) {

    TextSourceBuffer buf("prueba2.txt");
    Grammar_Scanner scanner(&buf);
    Grammar_Parser parser(scanner);
    LL1 yacc(parser);
    yacc.print_function();

    string str = "";
    cout << "Ingresa una cadena: ";
    cin >> str;
    vector<tuple<string, string, string, string>> record;
    cout << "\n\n";
    if(yacc.parse(str, record)){
      cout << "String accepted!" << endl;
    }else{
      cout << "String rejected!" << endl;
    }
    string line = "+";
    for(int i=0; i<15; i++) line += '-';
    line += "+";
    for(int i=0; i<30; i++) line += '-';
    line += "+";
    for(int i=0; i<15; i++) line += '-';
    line += "+";
    for(int i=0; i<10; i++) line += '-';
    line += "+\n";
    cout << line;
    cout << "|    MATCHED    |             TODO             |     INPUT     |  ACTION  |\n";
    cout << line;
    for(const auto& r : record){
      printf("|%15s|%30s|%15s|%10s|\n", get<0>(r).c_str(), get<1>(r).c_str(), get<2>(r).c_str(), get<3>(r).c_str());
      cout << line;
    }
    return 0;

}
