#include "NFA.h"
#include "DFA.h"
#include "Regex.h"
#include <iostream>
#include <cstring>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]){
    int nextarg = 1;
    bool nfaflag = false, opflag = false, regexflag = false, newflag=false, readflag=false, testflag=false;
    if (nextarg < argc){
        if(!strcmp(argv[nextarg], "--nfa"))
            nfaflag = true;
        else if (!strcmp(argv[nextarg], "--regex"))
            regexflag = true;
        nextarg++;
    }

    if(nfaflag){
        if(nextarg < argc){
            if(!strcmp(argv[nextarg], "-op"))
                opflag = true;
            else if(!strcmp(argv[nextarg], "-n"))
                newflag = true;
            else if(!strcmp(argv[nextarg], "-r"))
                readflag = true;
            else if(!strcmp(argv[nextarg], "-t"))
                testflag = true;
            nextarg++;

            if(opflag){
                if(nextarg < argc){
                    string operation = argv[nextarg];
                    //-plus, -kleene, -opt, -union, -concat
                    char op =   operation == "-plus"        ? '+' :
                                operation == "-kleene"      ? '*' :
                                operation == "-opt"         ? '?' :
                                operation == "-union"       ? '|' :
                                operation == "-concat"      ? '.' : '\0';
                    if (++nextarg < argc){
                        switch (op){
                            case '*':
                            case '?':
                            case '+':
                                if(nextarg+1 < argc){
                                    ifstream infile1(argv[nextarg]);
                                    ofstream outfile(argv[nextarg+1]);
                                    boost::archive::text_iarchive inarchive1(infile1);
                                    boost::archive::text_oarchive outarchive(outfile);
                                    NFA in, out;
                                    inarchive1 >> in;
                                    cout << "Input automaton: " << endl << in.stringify() << endl;
                                    if(op == '*')
                                        out = in.kleene_closure();
                                    else if (op == '?')
                                        out = in.zero_or_one();
                                    else
                                        out = in.plus_closure();
                                    outarchive << out;
                                    cout << "Output automaton: " << endl << out.stringify();
                                }
                                else{
                                    throw invalid_argument("No output file detected.");
                                }
                                break;
                            case '|':
                            case '.':
                                if(nextarg+1 < argc){
                                    if(nextarg+2 < argc){
                                        ifstream infile1(argv[nextarg]);
                                        ifstream infile2(argv[nextarg+1]);
                                        ofstream outfile(argv[nextarg+2]);
                                        boost::archive::text_iarchive inarchive1(infile1);
                                        boost::archive::text_iarchive inarchive2(infile2);
                                        boost::archive::text_oarchive outarchive(outfile);
                                        NFA in1, in2, out;
                                        inarchive1 >> in1;
                                        inarchive2 >> in2;
                                        cout << "Input automaton 1: " << endl << in1.stringify() << endl;
                                        cout << "Input automaton 2: " << endl << in2.stringify() << endl;
                                        if(op == '|')
                                            out = in1.nfa_union(in2);
                                        else
                                            out = in1.nfa_concat(in2);
                                        outarchive << out;
                                        cout << "Output automaton: " << endl << out.stringify();
                                    }
                                    else{
                                        throw invalid_argument("No output file detected.");
                                    }
                                }
                                else{
                                    throw invalid_argument("No input file 2 detected.");
                                }
                                break;
                            default:
                                throw invalid_argument("Invalid operation argument.");
                        }
                    }
                    else {
                        throw invalid_argument("No input file detected.");
                    }
                }
                else {
                    cout << "main.exe --nfa -op (-plus, -kleene, -opt, -union, -concat) {infile1} [infile2] {outfile}" << endl;
                }
            }
            else if (newflag){
                if(nextarg < argc && strlen(argv[nextarg]) == 1){
                    if(nextarg + 1 < argc){
                        ofstream outfile(argv[nextarg+1]);
                        boost::archive::text_oarchive outarchive(outfile);
                        NFA out = NFA::simpleNFA(argv[nextarg][0]);

                        outarchive << out;
                        cout << "Output automaton: " << endl << out.stringify();
                    }
                    else{
                        throw invalid_argument("No output file detected.");
                    }
                }
                else{
                    cout << "usage: main --nfa -n {char} {outfile}" << endl;
                }
            }
            else if (readflag){
                if (nextarg < argc){
                    ifstream infile1(argv[nextarg]);
                    boost::archive::text_iarchive inarchive1(infile1);
                    NFA in;
                    inarchive1 >> in;
                    cout << "Input automaton: " << endl;
                    cout << in.stringify() << endl;
                }
                else{
                    cout << "usage: main --nfa -r {infile}" << endl;
                }
            }
            else if (testflag){
                if (nextarg < argc){
                    ifstream infile1(argv[nextarg]);
                    boost::archive::text_iarchive inarchive1(infile1);
                    NFA in;
                    inarchive1 >> in;
                    cout << "Input automaton: " << endl;
                    cout << in.stringify() << endl;
                    while(true){
                        string str;
                        cout << "\nWrite your string: ";
                        cin >> str;
                        if(in.accept(str))
                            cout << "  *String accepted*" << endl;
                        else
                            cout << "  *String rejected*" << endl;
                    }
                }
                else{
                    cout << "usage: main --nfa -r {infile}" << endl;
                }
            }
            else{
                throw invalid_argument("Invalid execution mode argument.");
            }

        }
        else{
            throw invalid_argument("No execution mode argument detected.");
        }
    }
    else if (regexflag){
        if(nextarg < argc){
            string regex = argv[nextarg];
            cout << " DFA for " << regex << "\n\n";
            NFA nfa = Regex::compile(regex);
            DFA dfa = nfa.toDFA();
            dfa = dfa.minimize();
            cout << dfa.stringify();
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
        }
        else{
            throw invalid_argument("--regex requires a regular expression as input.");
        }
    }
    else{
        cout << "usage:" << endl;
        cout << "main.exe --regex {regular expression}" << endl;
        cout << "main.exe --nfa -op (-plus, -kleene, -opt, -union, -concat) {infile1} [infile2] {outfile}" << endl;
        cout << "main.exe --nfa -n {char} {outfile}" << endl;
        cout << "main.exe --nfa -r {infile}" << endl;
        cout << "main.exe --nfa -t {infile}" << endl;
    }

    return 0;
}
