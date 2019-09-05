#ifndef NFA_H
#define NFA_H

#include "DFA.h"

#include <map>
#include <set>
#include <utility>
#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>

using namespace std;

class NFA{

    set<int> states;
	int initial_state=0;
	map<pair<int, char>, set<int>> transitions;
	set<int> accepting_states;

	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & states;
        ar & initial_state;
        ar & transitions;
        ar & accepting_states;
    }
	set<int> next_states(int state, char symbol = '\0');
	set<int> e_closure(int state);
	set<int> e_closure(const set<int>& states_set);
	set<int> compute(const set<int>& states_set, char *string);
	void addTransition(int from, const set<int>& to, char symbol);

public:
    NFA(set<int> states, int initial_state, map<pair<int, char>, set<int>> transitions, set<int> accepting_states):
    states(move(states)), initial_state(initial_state), transitions(move(transitions)), accepting_states(move(accepting_states)) {};
    bool accept(const string & str);
    static NFA simpleNFA(char c);
    int get_size();
    NFA nfa_concat(NFA nfa);
    NFA nfa_union(NFA nfa);
    NFA kleene_closure();
    NFA plus_closure();
    NFA zero_or_one();
    map<pair<int, char>, set<int>> get_new_trans_function(int x);
    virtual ~NFA();
    NFA();

    //for test purposes
    string stringify();

    set<char> getAlphabet();

    DFA toDFA();
};

#endif
