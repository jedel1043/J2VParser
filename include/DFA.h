#ifndef DFA_H
#define DFA_H

#include <map>
#include <set>
#include <cstdio>

using namespace std;

class DFA{
	set<int> states;
	set<char> alphabet;
	map<pair<int, char>, int> transitions;
	int initial_state;
	set<int> accepting_states;

	set<int> inverse_transition(const set<int> & in_states, char c);
	int compute(const string& str);

public:
	DFA(set<char> alphabet, map<pair<int, char>, int> transitions, set<int> states, int initial_state, set<int> accepting_states):
	alphabet(move(alphabet)),
	transitions(move(transitions)),
	states(move(states)),
	initial_state(initial_state),
	accepting_states(move(accepting_states)){}
	void print();
	int get_size();
	DFA minimize();
	bool accept(const string& str);
};

#endif