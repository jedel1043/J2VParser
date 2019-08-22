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
	int initial_state{};
	map<pair<int, char>, set<int>> transitions;
	set<int> accepting_states;
	int number_of_states{};

	set<int> next_states(int state, char symbol = '\0');
	set<int> e_closure(int state);
	set<int> e_closure(const set<int>& states_set);
	set<int> compute(const set<int>& states_set, char *string);
	void addTransition(int from, const set<int>& to, char symbol);
    friend std::ostream & operator<<(std::ostream & ostream1, const NFA & obj);

public:
    NFA(int size, int initial_state, map<pair<int, char>, set<int>> transitions, const set<int> & accepting_states);
    bool accept(const string & str);
    static NFA simpleNFA(char c);
    NFA nfa_concat(NFA nfa);
    NFA nfa_union(NFA nfa);
    NFA kleene_closure();
    NFA plus_closure();
    NFA zero_or_one();
    virtual ~NFA();
    friend std::ostream & operator<<(std::ostream & ostream1, const NFA & obj);
};

std::ostream & operator<<(std::ostream & ostream1, const NFA & obj);

#endif
