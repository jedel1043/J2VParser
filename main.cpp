#include <iostream>
#include <vector>
#include <io/buffer.h>
#include <error.h>
#include <compiler/macros.h>
#include <compiler/scanner.h>
#include <compiler/parser.h>
#include <automata/NFA.h>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {
    /*
  if(argc<2)
    AbortTranslation(InvalidCommandLineArgs);*/

    string filePath = "";
    cout << "Ingrese el directorio del archivo:";
    cin >> filePath;

    TextSourceBuffer buf(filePath);
    Scanner scanner(&buf);
    Parser parser(scanner);
    vector<NFA> machines = parser.parse();
    NFA result = NFA::lexical_union(machines);


    int i = 1;
    vector<NFA>::iterator iterador;

    for (iterador = machines.begin(); iterador != machines.end(); ++iterador) {
        cout << to_string(i) << ":\n";
        DFA temp = (*iterador).toDFA().minimize();
        temp.print();
        cout << "\n";
        i = i + 1;
    }

    cout << "\nUnion de todos: \n";
    DFA temp = result.toDFA().minimize();
    temp.print();


    while (true) {
        string str;
        cout << "Ingresa una cadena: ";
        cin >> str;
        int r = result.lex_accept(str);
        if (r != -1)
            cout << "Aceptada!: " << "(" << str << ", " << to_string(r) << ")";
        else
            cout << "Rechazada!" << endl;
    }


}
