#ifndef NFA_H
#define NFA_H

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>

using namespace std;
typedef string State;

class NFA{
	set<State> states;
	State initial_state;
	map<pair<State, char>, set<State>> transitions;
	set<State> accepting_states;

	set<State> next_states(const State& state, char symbol = '\0');
	set<State> e_closure(const State& state);
	set<State> e_closure(const set<State>& states_set);
	set<State> compute(const set<State>& states_set, char *string);
	set<State> getNewStatesName(const string& prefix);
	void addTransition(const State& from, const set<State>& to, char symbol);
	static set<State> getNewStatesName(const set<State>& states, const string& prefix);
	friend std::ostream & operator<<(std::ostream & str, const NFA & obj);

public:
	NFA(set<State> states, State initial_state, map<pair<State, char>, set<State>> transitions, set<State> accepting_states);
    virtual ~NFA();
    bool accept(const string& str);
	static NFA simpleNFA(char c);
	NFA nfa_concat(NFA nfa);
	NFA nfa_union(NFA nfa);
	NFA kleene_closure();
	NFA plus_closure();
};

inline std::ostream & operator<<(std::ostream & str, const NFA & obj){
    str << "------------------------\n";
    str << "States: ";
    for(State const& element : obj.states)
        str << element << ", ";
    str << "\n";

    str << "Transition function: \n";

    map<pair<State, char>, set<State>>::const_iterator it;
    for (it = obj.transitions.begin(); it != obj.transitions.end(); ++it){
        if(it->first.second == '\0')
            str << "\t(" << it->first.first << ", ) => {";
        else
            str << "\t(" << it->first.first << ", " << it->first.second << ") => { ";
        for(State const& element : it->second)
            str << element << ", ";
        str << "}\n";
    }

    str << "Initial state: " << obj.initial_state << endl;
    str << "Accepting states: ";
    for(State const& element : obj.accepting_states)
        str << element << ", ";
    str << "\n";
    str << "------------------------\n";
    return str;
}

#endif