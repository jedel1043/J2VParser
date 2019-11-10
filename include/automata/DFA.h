#ifndef DFA_H
#define DFA_H

#include <map>
#include <set>
#include <cstdio>
#include <utility>
#include <vector>

using namespace std;

class DFA {
    int size;
    set<int> states;
    set<char> alphabet;
    map<pair<int, char>, int> transitions;
    int initial_state;
    set<int> accepting_states;
    map<int, string> tokens;
public:
    set<int> inverse_transition(const set<int> &new_states, char c);

    int compute(const string &str);

    DFA(int s, set<char> a, map<pair<int, char>, int> t, int i, set<int> f, map<int, string> tokens) :
    size(s),
    alphabet(std::move(a)),
    transitions(std::move(t)),
    initial_state(i),
    accepting_states(std::move(f)),
    tokens(std::move(tokens)) {
        for (int j = 1; j <= s; j++)
            states.insert(j);
    }


    void print();

    void toFile(const string &filename);

    DFA minimize();

    string accept(const string &str);

    const set<int> &getAcceptingStates();

    const map<int, string> &getTokens();

    int getInitialState();

    int compute(int state, char c);
};



#endif
