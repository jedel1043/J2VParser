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
};

inline std::ostream & operator<<(std::ostream & ostream1, const NFA & obj){
    ostream1 << "------------------------\n";
    ostream1 << "States: ";
    for(int const state : obj.states)
        ostream1 << state << ", ";
    ostream1 << endl;

    ostream1 << "Transition function: \n";
    map<pair<int, char>, set<int>>::const_iterator it;
    for (it = obj.transitions.begin(); it != obj.transitions.end(); ++it){
        if(it->first.second == '\0')
            ostream1 << "\t(" << it->first.first << ", ) => {";
        else
            ostream1 << "\t(" << it->first.first << ", " << it->first.second << ") => { ";
        for(int const element : it->second)
            ostream1 << element << ", ";
        ostream1 << "}\n";
    }

    ostream1 << "Initial state: " << obj.initial_state << endl;
    ostream1 << "Accepting states: ";
    for(int const element : obj.accepting_states)
        ostream1 << element << ", ";
    ostream1 << "\n";

    ostream1 << "------------------------\n";

    return ostream1;
}

#endif
