#ifndef NFA_H
#define NFA_H

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>

using namespace std;

class NFA{
	static int StateCounter;
	static int getStateNumber(){
		int number = NFA::StateCounter;
		NFA::StateCounter++;
		return number;
	}

	NFA();

	set<int> states;
	int initial_state;
	map<pair<int, char>, set<int>> transitions;
	set<int> accepting_states;
	int number_of_states;

	set<int> next_states(int state, char symbol = '\0');
	set<int> eclousure(int state);
	set<int> eclousure(set<int> states);
	set<int> compute(set<int> states, char *string);
	set<int> getNewStatesName(string prefix);
	void addTransition(int from, set<int> to, char symbol);

public:
	NFA(int size, int initial_state, map<pair<int, char>, set<int>> transitions, set<int> accepting_states);
	bool accept(string str);
	void print();
	static NFA simpleNFA(char c);
	NFA nfa_concat(NFA nfa);
	NFA nfa_union(NFA nfa);
	NFA kleene_clousure();
	NFA plus_clousure();
};

#endif
