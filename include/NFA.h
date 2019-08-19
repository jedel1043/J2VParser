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
	set<State> eclousure(const State& state);
	set<State> eclousure(const set<State>& states_set);
	set<State> compute(const set<State>& states_set, char *string);
	set<State> getNewStatesName(const string& prefix);
	void addTransition(const State& from, const set<State>& to, char symbol);
	static set<State> getNewStatesName(const set<State>& states, const string& prefix);

public:
	NFA(set<State> states, State initial_state, map<pair<State, char>, set<State>> transitions, set<State> accepting_states):
	    states(std::move(states)), initial_state(std::move(initial_state)), transitions(std::move(transitions)), accepting_states(std::move(accepting_states)){}
	bool accept(const string& str);
	void print();
	static NFA simpleNFA(char c);
	NFA nfa_concat(NFA nfa);
	NFA nfa_union(NFA nfa);
	NFA kleene_clousure();
	NFA plus_clousure();
};

#endif