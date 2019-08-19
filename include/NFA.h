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

public:
	NFA(set<State> states, State initial_state, map<pair<State, char>, set<State>> transitions, set<State> accepting_states);
    const set<State> &getStates() const;
    const State &getInitialState() const;
    const map<pair<State, char>, set<State>> & getTransitions() const;
    const set<State> &getAcceptingStates() const;
    bool accept(const string& str);
	static NFA simpleNFA(char c);
	NFA nfa_concat(NFA nfa);
	NFA nfa_union(NFA nfa);
	NFA kleene_closure();
	NFA plus_closure();

    virtual ~NFA();
};

inline std::ostream & operator<<(std::ostream & str, const NFA & obj){
    str << "------------------------\n";
    str << "States: ";
    for(State const& element : obj.getStates())
        str << element << ", ";
    str << "\n";

    str << "Transition function: \n";

    map<pair<State, char>, set<State>>::const_iterator it;
    for (it = obj.getTransitions().begin(); it != obj.getTransitions().end(); ++it){
        if(it->first.second == '\0')
            str << "\t(" << it->first.first << ", ) => {";
        else
            str << "\t(" << it->first.first << ", " << it->first.second << ") => { ";
        for(State const& element : it->second)
            str << element << ", ";
        str << "}\n";
    }

    str << "Initial state: " << obj.getInitialState() << endl;
    str << "Accepting states: ";
    for(State const& element : obj.getAcceptingStates())
        str << element << ", ";
    str << "\n";
    str << "------------------------\n";
    return str;
}

#endif