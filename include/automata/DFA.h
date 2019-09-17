#ifndef DFA_H
#define DFA_H

#include <map>
#include <set>
#include <cstdio>

using namespace std;

class DFA{
	int size;
	set<int> states;
	set<char> alphabet;
	map<pair<int, char>, int> transitions;
	int initial_state;
	set<int> accepting_states;

	set<int> inverse_transition(set<int> states, char c);
	int compute(string str);

public:
	DFA(int s, set<char> a, map<pair<int, char>, int> t, int i, set<int> f): size(s),
	alphabet(a), transitions(t), initial_state(i), accepting_states(f){
		for(int i=1; i<=s; i++)
			states.insert(i);
	}
	void print();
	void toFile(string filename);
	DFA minimize();
	bool accept(string str);
};

#endif
