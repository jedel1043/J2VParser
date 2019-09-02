#include "NFA.h"
#include "DFA.h"
#include "Regex.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]){

    ofstream outfile;
    if(argc == 4 && !strcmp(argv[1], "--nfa")){
         outfile = ofstream(argv[3]);
    }
    else if(argc == 3 && !strcmp(argv[1], "--regex")){
        cout << " DFA for " << argv[2] << "\n\n";
        NFA nfa = Regex::compile(argv[2]);
        DFA dfa = nfa.toDFA();
        dfa = dfa.minimize();
        dfa.print();
        cout << "\n";

        while(true){
            string str;
            cout << "\nWrite your string: ";
            cin >> str;
            if(dfa.accept(str))
                cout << "  *String accepted*" << endl;
            else
                cout << "  *String rejected*" << endl;
        }
        return 1;
    }
    string options[] = {"Add new simple automaton", "Concat operation", "Union operation", "Kleene clousure", "Positive clousure", "Optional DFA", "Print table", "Quit"};
    bool on = true;
    int opt_selected;
    map<string, NFA> automata;

    while(on){
        system("cls");
        cout << "     *** REGEX EVALUATOR ***     \n\n";
        int index_opt = 1;
        for(string opt : options){
            cout << " [" << index_opt << "]  " << opt << endl;
            index_opt++;
        }
        printf("  OPTION SELECTED: ");
        cin >> opt_selected;
        cout << "\n\n";
        map<string, NFA>:: iterator it;
        switch(opt_selected){
            case 1:{
                cout << "Add automaton for: ";
                char c;
                cin >> c;
                string name;
                name += c;
                automata.insert(make_pair(name, NFA::simpleNFA(c)));
                break;
            }

            case 2:{
                printf("Automata created:\n");
                string first, second;
                for(it=automata.begin(); it!=automata.end(); ++it){
                    cout << "  " << it->first << endl;
                }
                cout << "\nFirst automaton:  ";
                cin >> first;
                cout << "Second automaton: ";
                cin >> second;
                string name = "(" + first + "." + second + ")";
                automata.insert(make_pair(name, automata[first].nfa_concat(automata[second])));
                break;
            }

            case 3:{
                printf("Automata created:\n");
                string first, second;
                for(it=automata.begin(); it!=automata.end(); ++it){
                    cout << "  " << it->first << endl;
                }
                cout << "\nFirst automaton:  ";
                cin >> first;
                cout << "Second automaton: ";
                cin >> second;
                string name = "(" + first + "|" + second + ")";
                automata.insert(make_pair(name, automata[first].nfa_union(automata[second])));
                break;
            }

            case 4:{
                printf("Automata created:\n");
                string first;
                for(it=automata.begin(); it!=automata.end(); ++it){
                    cout << "  " << it->first << endl;
                }
                cout << "\nSelect automaton:  ";
                cin >> first;
                string name;
                if(first.size() == 1)
                    name = first + "*";
                else
                    name = "(" +first + ")*";
                automata.insert(make_pair(name, automata[first].kleene_closure()));
                break;
            }

            case 5:{
                printf("Automata created:\n");
                string first;
                for(it=automata.begin(); it!=automata.end(); ++it){
                    cout << "  " << it->first << endl;
                }
                cout << "\nSelect automaton:  ";
                cin >> first;
                string name;
                if(first.size() == 1)
                    name = first + "+";
                else
                    name = "(" +first + ")+";
                automata.insert(make_pair(name, automata[first].plus_closure()));
                break;
            }

            case 6:{
                printf("Automata created:\n");
                string first;
                for(it=automata.begin(); it!=automata.end(); ++it){
                    cout << "  " << it->first << endl;
                }
                cout << "\nSelect automaton:  ";
                cin >> first;
                string name;
                if(first.size() == 1)
                    name = first + "?";
                else
                    name = "(" +first + ")?";
                automata.insert(make_pair(name, automata[first].zero_or_one()));
                break;
            }

            case 7:{
                printf("Automata created:\n");
                string first;
                for(it=automata.begin(); it!=automata.end(); ++it){
                    cout << "  " << it->first << endl;
                }
                cout << "\nSelect automaton:  ";
                cin >> first;
                DFA dfa = automata[first].toDFA();
                dfa = dfa.minimize();
                dfa.print();

                char ch;
                while (cin.readsome(&ch, 1) != 0);
                cout << "Press any key to continue...\n";
                getchar();
                break;
            }

            case 8:
                on = false;
                break;

            default:
                continue;
                break;
        }
    }

    return 0;
}
