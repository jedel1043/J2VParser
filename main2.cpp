//
// Created by asusi7 on 9/18/2019.
//

#include <automata/NFA.h>
#include <automata/DFA.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {

    vector<pair<int, NFA>> automata;
    int contador = 0;
    string read = "";

    cout << "Creador de automatas\n Ingrese \"s\" para salir";

    while (read != "s") {
        vector<string> opciones = {"Ingrese una de las opciones: ",
                                   "b para crear automata basico",
                                   "u para union", "c para concatenacion",
                                   "k para cerradura de Kleene",
                                   "+ para cerradura positiva",
                                   "? para ?",
                                   "n para computar",
                                   "i para imprimir"};
        int i = 0;

        cout << "\n Automatas: \n";

        if (!automata.empty()) {
            vector<pair<int, NFA> >::iterator iterador;

            for (iterador = automata.begin(); iterador != automata.end(); ++iterador) {
                cout << "Automata " << iterador->first << "\n";
            }

        }

        cout << "\n";


        for (const string t : opciones) {
            cout << t;
            cout << "\n";
        }

        cin >> read;

        if (read == "u" || read == "c") {
            int ind1 = -1;
            int ind2 = -1;

            NFA a1;
            NFA a2;

            cout << "\nIngrese el indice del primer automata: ";
            cin >> ind1;
            cout << "\nIngrese el indice del segundo automata: ";
            cin >> ind2;

            cout << "\n";

            vector<pair<int, NFA> >::iterator iterador;
            int j = 0;

            for (iterador = automata.begin(); iterador != automata.end(); ++iterador) {
                if (iterador->first == ind1) {
                    a1 = iterador->second;
                    ind1 = j;
                }

                if (iterador->first == ind2) {
                    a2 = iterador->second;
                    ind2 = j;
                }
                j = j + 1;
            }

            automata.erase(automata.begin() + ind1);

            if (ind1 < ind2) {
                automata.erase(automata.begin() + ind2 - 1);
            } else {
                automata.erase(automata.begin() + ind2);
            }

            if (read == "u") {
                automata.push_back(make_pair(contador, a1.nfa_union(a2)));
            } else {
                automata.push_back(make_pair(contador, a1.nfa_concat(a2)));
            }

            cout << "\nEl automata creado tiene por indice " << contador << "\n";


            contador = contador + 1;

        }


        if (read == "b") {
            char reg = '\0';
            cout << "\nIngrese un caracter: ";
            cin >> reg;

            automata.push_back(make_pair(contador, NFA::simpleNFA(reg)));

            cout << "\nEl automata creado tiene por indice " << contador << "\n";

            contador = contador + 1;
        }

        if (read == "k" || read == "+" || read == "?") {
            int ind = -1;
            NFA a;

            cout << "\nIngrese el indice del automata: ";
            cin >> ind;

            vector<pair<int, NFA> >::iterator iterador;

            int j = 0;

            for (iterador = automata.begin(); iterador != automata.end(); ++iterador) {
                if (iterador->first == ind) {
                    a = iterador->second;
                    automata.erase(automata.begin() + j);

                    if (read == "k") {
                        automata.push_back(make_pair(contador, a.kleene_closure()));
                    } else if (read == "+") {
                        automata.push_back(make_pair(contador, a.plus_closure()));
                    } else {
                        automata.push_back(make_pair(contador, a.zero_or_one()));
                    }

                    cout << "\nEl automata creado tiene por indice " << contador << "\n";

                    contador = contador + 1;

                }
                j++;
            }

            j = 0;
        }

        if (read == "n") {
            int ind = -1;
            string cadena = "";
            cout << "\nIngrese el indice del automata que computara: ";
            cin >> ind;
            bool acpt = false;

            cout << "\nIngrese la cadena a computar: ";
            cin >> cadena;

            vector<pair<int, NFA> >::iterator iterador;

            for (iterador = automata.begin(); iterador != automata.end(); ++iterador) {
                if (iterador->first == ind) {
                    acpt = iterador->second.accept(cadena);

                    if (acpt) {
                        cout << "\nla cadena es aceptada";
                    } else {
                        cout << "\nLa cadena no es aceptada";
                    }
                }

            }
        }

        if (read == "i") {
            int ind = 0;
            cout << "\nIngrese el indice del automata: ";
            cin >> ind;

            vector<pair<int, NFA> >::iterator iterador;

            for (iterador = automata.begin(); iterador != automata.end(); ++iterador) {
                if (iterador->first == ind) {
                    NFA temp = iterador->second;
                    DFA r = temp.toDFA().minimize();
                    r.print();
                    cout << "a";
                }
            }
        }

    }

    return 0;
}
