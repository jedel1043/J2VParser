#ifndef NFA_H
#define NFA_H

#include <vector>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <automata/DFA.h>

using namespace std;

class NFA {
    static int StateCounter;

    static int getStateNumber() {
        int number = NFA::StateCounter;
        NFA::StateCounter++;
        return number;
    }

    set<int> states;
    int initial_state{};
    map<pair<int, char>, set<int>> transitions;
    set<int> accepting_states;
    int number_of_states;
    map<int, int> accepting_values;

    set<int> next_states(int state, char symbol = '\0');

    set<int> e_closure(int state);

    set<int> e_closure(const set<int> &states_set);

    set<int> compute(const set<int> &states_set, char *string);

    void addTransition(int from, const set<int> &to, char symbol);

    friend std::ostream &operator<<(std::ostream &ostream1, const NFA &obj);

public:
    NFA(int size, int initial_state, map<pair<int, char>, set<int>> transitions, const set<int> &accepting_states);

    bool accept(const string &str);

    int lex_accept(const string &str);

    set<char> getAlphabet();

    DFA toDFA();

    static NFA simpleNFA(char c);

    static NFA simpleNFA(char from, char to);

    static NFA simpleNFA(const set<char> &chars);

    static NFA lexical_union(const vector<NFA> &nfas);

    NFA nfa_concat(NFA nfa);

    NFA nfa_union(NFA nfa);

    NFA kleene_closure();

    NFA plus_closure();

    NFA zero_or_one();

    virtual ~NFA();

    friend std::ostream &operator<<(std::ostream &ostream1, const NFA &obj);

    void addAcceptingValue(int value);

    NFA();

    int getAcceptValue(int state);
};

std::ostream &operator<<(std::ostream &ostream1, const NFA &obj);

#endif
