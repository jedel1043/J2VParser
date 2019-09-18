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

  vector<NFA> :: iterator iterador;


//  int  i = 1;
//
//  for(iterador = machines.begin(); iterador != machines.end(); ++iterador){
//      cout  << to_string(i) << ":\n";
//      DFA temp = (*iterador).toDFA().minimize();
//      temp.print();
//      cout << "\n";
//      i = i  + 1;
//  }

  DFA temp  = result.toDFA().minimize();
  temp.print();


  while(true){
    string str;
    cout << "Ingresa una cadena: ";
    cin >> str;
    int r = temp.accept(str);
    if(r != -1)
      cout << "Aceptada!: " << r << endl;
    else
      cout << "Rechazada!" << endl;
  }



}
