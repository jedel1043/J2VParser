#include <iostream>
#include <vector>
#include <io/buffer.h>
#include <error.h>
#include <compiler/macros.h>
#include <compiler/scanner.h>
#include <compiler/parser.h>
#include <automata/NFA.h>

using namespace std;

int main(int argc, char const *argv[]) {
  if(argc<2)
    AbortTranslation(InvalidCommandLineArgs);
  TextSourceBuffer buf(argv[1]);
  Scanner scanner(&buf);
  Parser parser(scanner);
  vector<NFA> machines = parser.parse();
  NFA result = NFA::lexycal_union(machines);

  while(true){
    string str;
    cout << "Ingresa una cadena: ";
    cin >> str;
    int r = result.lex_accept(str);
    if(r != -1)
      cout << "Aceptada!: " << r << endl;
    else
      cout << "Rechazada!" << endl;
  }
}
