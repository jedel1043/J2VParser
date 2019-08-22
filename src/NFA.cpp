#include "NFA.h"

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <stack>
#include <cstring>

using namespace std;

int NFA::StateCounter = 0;

NFA::NFA() = default;

NFA::~NFA() = default;

NFA::NFA(int size, int initial_state, map<pair<int, char>, set<int>> transitions, const set<int>& accepting_states){
	this->number_of_states = size;
	map<int, int> tmp_dic;
	for(int i=0; i<size; i++){
		this->states.insert(NFA::StateCounter);
		tmp_dic.insert(pair<int, int>(std::make_pair(i, NFA::StateCounter)));
		NFA::StateCounter++;
	}
	map<pair<int, char>, set<int>>::iterator map_it;
	for(map_it = transitions.begin(); map_it != transitions.end(); ++map_it){
		set<int> transition_states;
		for(int const t : map_it->second)
			transition_states.insert(tmp_dic[t]);
		this->transitions.insert(pair<pair<int, char>, set<int>>(std::make_pair(tmp_dic[map_it->first.first], map_it->first.second), transition_states));
	}
	for(int const state : accepting_states)
		this->accepting_states.insert(tmp_dic[state]);
	this->initial_state = tmp_dic[initial_state];
}

set<int> NFA::next_states(int state, char symbol){
	set<int> empty_set;
	pair<int, char> input(state, symbol);
	auto transition = this->transitions.find(input);
	if(transition != this->transitions.end())
		return transition->second;
	return empty_set;
}

set<int> NFA::e_closure(int state){
	set<int> closure_set;
	stack<int> states_saver;
	states_saver.push(state);

	while(!states_saver.empty()){
		int check_state = states_saver.top();
		states_saver.pop();
		if(!closure_set.count(check_state))
			closure_set.insert(check_state);
		set<int> state_set = this->next_states(check_state);
		for(int const element : state_set){
			if(!closure_set.count(element)){
				closure_set.insert(element);
				states_saver.push(element);
			}
		}
	}
	return closure_set;
}

set<int> NFA::e_closure(const set<int>& states_set){
	set<int> result;
	for(int const state : states_set){
		set<int> e_state = this->e_closure(state);
		result.insert(e_state.begin(), e_state.end());
	}
	return result;
}

set<int> NFA::compute(const set<int>& states_set, char *string){
	set<int> result;
	if(*string == '\0'){
		return this->e_closure(states_set);
	}
	else{
		for(int const state : states_set){
			set<int> closure = this->e_closure(state);
			for(int const s : closure){
				set<int> s_states = this->next_states(s, *string);
				result.insert(s_states.begin(), s_states.end());
			}
		}
	}
	string++;
	return this->compute(result, string);
}

bool NFA::accept(const string& str){
	set<int> configuration;
	configuration.insert(this->initial_state);
	char cstr[str.size() + 1];
	strcpy(cstr, str.c_str());
	set<int> result = this->compute(configuration, cstr);

	set<int> intersection;
	set_intersection(result.begin(), result.end(), this->accepting_states.begin(), this->accepting_states.end(), inserter(intersection, intersection.begin()));

	return (!intersection.empty());
}

NFA NFA::simpleNFA(char c){
	map<pair<int, char>, set<int>> transitions;
	set<int> accepting_states;

	set<int> t;
	t.insert(1);

	transitions.insert(pair<pair<int, char>, set<int>>(std::make_pair(0, c), t));
	accepting_states.insert(1);

	return NFA(2, 0, transitions, accepting_states);
}

void NFA::addTransition(int from, const set<int>& to, char symbol){
	this->transitions.insert(pair<pair<int, char>, set<int>>(std::make_pair(from, symbol), to));
}

NFA NFA::nfa_concat(NFA nfa){
	// New NFA
	NFA result;
	// New composite states
	result.states.insert(this->states.begin(), this->states.end());
	result.states.insert(nfa.states.begin(), nfa.states.end());
	// New initial state
	result.initial_state = this->initial_state;
	// New accepting states
	result.accepting_states.insert(nfa.accepting_states.begin(), nfa.accepting_states.end());
	// New transition function
	result.transitions.insert(this->transitions.begin(), this->transitions.end());
	result.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
	set<int> thompson_concat;
	thompson_concat.insert(nfa.initial_state);
	for(int const state : this->accepting_states){
		result.addTransition(state, thompson_concat, '\0');
	}

	return result;
}

NFA NFA::nfa_union(NFA nfa){
	// New NFA
	NFA result;
	// New composite states
	result.states.insert(this->states.begin(), this->states.end());
	result.states.insert(nfa.states.begin(), nfa.states.end());
	// New initial state
	result.initial_state = NFA::getStateNumber();
	// New accepting states
	int new_final_state = NFA::getStateNumber();
	result.accepting_states.insert(new_final_state);
	// Adding the new states
	result.states.insert(result.initial_state);
	result.states.insert(new_final_state);
	// New transition function
	result.transitions.insert(this->transitions.begin(), this->transitions.end());
	result.transitions.insert(nfa.transitions.begin(), nfa.transitions.end());
	set<int> thompson_union;
	thompson_union.insert(this->initial_state);
	thompson_union.insert(nfa.initial_state);
	result.addTransition(result.initial_state, thompson_union, '\0');
	set<int> thompson_union2;
	thompson_union2.insert(new_final_state);
	for(int const state : this->accepting_states)
		result.addTransition(state, thompson_union2, '\0');
	for(int const state : nfa.accepting_states)
		result.addTransition(state, thompson_union2, '\0');

	return result;
}

NFA NFA::kleene_closure(){
	// New NFA
	NFA result;
	// New states
	result.states.insert(this->states.begin(), this->states.end());
	// New initial state
	result.initial_state = NFA::getStateNumber();
	// New final states
	int new_final_state = NFA::getStateNumber();
	result.accepting_states.insert(new_final_state);
	// Adding the new states
	result.states.insert(result.initial_state);
	result.states.insert(new_final_state);
	// New transitions
	result.transitions.insert(this->transitions.begin(), this->transitions.end());
	set<int> initial_trans_state = result.accepting_states;
	initial_trans_state.insert(this->initial_state);
	result.addTransition(result.initial_state, initial_trans_state, '\0');
	set<int> new_final_states;
	new_final_states.insert(this->initial_state);
	new_final_states.insert(new_final_state);
	for(int const state : this->accepting_states)
		result.addTransition(state, new_final_states, '\0');
	// Returning the result
	return result;
}

NFA NFA::plus_closure(){
	// New NFA
	NFA result;
	// New states
	result.states.insert(this->states.begin(), this->states.end());
	// New initial state
	result.initial_state = NFA::getStateNumber();
	// New final states
	int new_final_state = NFA::getStateNumber();
	result.accepting_states.insert(new_final_state);
	// Adding the new states
	result.states.insert(result.initial_state);
	result.states.insert(new_final_state);
	// New transitions
	result.transitions.insert(this->transitions.begin(), this->transitions.end());
	set<int> initial_trans_state;
	initial_trans_state.insert(this->initial_state);
	result.addTransition(result.initial_state, initial_trans_state, '\0');
	set<int> new_final_states;
	new_final_states.insert(this->initial_state);
	new_final_states.insert(new_final_state);
	for(int const state : this->accepting_states)
		result.addTransition(state, new_final_states, '\0');
	// Returning the result
	return result;
}

NFA NFA::zero_or_one(){
	// New NFA
	NFA result;
	// New states
	result.states.insert(this->states.begin(), this->states.end());
	// New initial state
	result.initial_state = NFA::getStateNumber();
	// New final states
	int new_final_state = NFA::getStateNumber();
	result.accepting_states.insert(new_final_state);
	// Adding the new states
	result.states.insert(result.initial_state);
	result.states.insert(new_final_state);
	// New transitions
	result.transitions.insert(this->transitions.begin(), this->transitions.end());
	set<int> initial_trans_state;
	initial_trans_state.insert(this->initial_state);
	initial_trans_state.insert(new_final_state);
	result.addTransition(result.initial_state, initial_trans_state, '\0');
	set<int> new_final_states;
	new_final_states.insert(new_final_state);
	for(int const state : this->accepting_states)
		result.addTransition(state, new_final_states, '\0');
	// Returning the result
	return result;
}

std::ostream & operator<<(std::ostream & ostream1, const NFA & obj){
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
