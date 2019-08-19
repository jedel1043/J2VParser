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

	set<State> next_states(State state, char symbol = '\0');
	set<State> eclousure(State state);
	set<State> eclousure(set<State> states);
	set<State> compute(set<State> states, char *string);
	set<State> getNewStatesName(string prefix);
	void addTransition(State from, set<State> to, char symbol);
	static set<State> getNewStatesName(set<State> states, string prefix);

public:
	NFA(set<State> states, State initial_state, map<pair<State, char>, set<State>> transitions, set<State> accepting_states): states(states), initial_state(initial_state), transitions(transitions), accepting_states(accepting_states){}
	bool accept(string str);
	void print();
	static NFA simpleNFA(char c);
	NFA nfa_concat(NFA nfa);
	NFA nfa_union(NFA nfa);
	NFA kleene_clousure();
	NFA plus_clousure();
};

#endif