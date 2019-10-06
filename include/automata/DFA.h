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
    map<int, int> tokens;

    set<int> inverse_transition(const set<int> &new_states, char c);

    int compute(const string &str);

public:
    DFA(int s, set<char> a, map<pair<int, char>, int> t, int i, set<int> f, map<int, int> tokens) : size(s),
                                                                                                    alphabet(std::move(
                                                                                                            a)),
                                                                                                    transitions(
                                                                                                            std::move(
                                                                                                                    t)),
                                                                                                    initial_state(i),
                                                                                                    accepting_states(
                                                                                                            std::move(
                                                                                                                    f)),
                                                                                                    tokens(std::move(
                                                                                                            tokens)) {
        for (int j = 1; j <= s; j++)
            states.insert(j);
    }

    void print();

    void toFile(const string &filename);

    DFA minimize();

    int accept(const string &str);

    void printMap(vector<string> maping);
};

#endif
