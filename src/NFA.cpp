#include "../include/NFA.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm> 
#include <stack>
#include <cstring>

using namespace std;

set<State> NFA::next_states(const State& state, char symbol){
	set<State> empty_set;
	pair<State, char> input(state, symbol);
	auto transition = transitions.find(input);
	if(transitions.find(input) != transitions.end())
		return transition->second;
	return empty_set;
}

set<State> NFA::eclousure(const State& state){
	set<State> clousure_set;
	stack<State> aux_states_stack;
	aux_states_stack.push(state);

	while(!aux_states_stack.empty()){
		State check_state = aux_states_stack.top();
		aux_states_stack.pop();
		if(!clousure_set.count(check_state))
			clousure_set.insert(check_state);
		set<State> state_set = this->next_states(check_state);
		for(State const& element : state_set){
			if(!clousure_set.count(element)){
				clousure_set.insert(element);
				aux_states_stack.push(element);
			}
		}
	}

	return clousure_set;
}

set<State> NFA::eclousure(const set<State>& states_set){
	set<State> result;
	for(State const& state : states_set){
		set<State> e_state = this->eclousure(state);
		result.insert(e_state.begin(), e_state.end());
	}
	return result;
}

set<State> NFA::compute(const set<State>& states_set, char *string){
	set<State> result;
	if(*string == '\0'){
		return this->eclousure(states_set);
	}
	else{
		for(State const& state : states_set){
			set<State> clousure = this->eclousure(state);
			for(State const& s : clousure){
				set<State> s_states = this->next_states(s, *string);
				result.insert(s_states.begin(), s_states.end());
			}
		}
	}
	string++;
	return this->compute(result, string);
}

bool NFA::accept(const string& str){
	set<State> configuration;
	configuration.insert(this->initial_state);
	char cstr[str.size() + 1];
	strcpy(cstr, str.c_str());
	set<State> result = this->compute(configuration, cstr);

	set<State> intersection;
	set_intersection(result.begin(), result.end(), this->accepting_states.begin(), this->accepting_states.end(), inserter(intersection, intersection.begin()));

	return (!intersection.empty());
}

void NFA::print(){
	cout << "------------------------\n";
	cout << "States: ";
	for(State const& element : states)
    		cout << element << ", ";
    cout << "\n";

    cout << "Transition function: \n";
	map<pair<State, char>, set<State>>::iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it){
		if(it->first.second == '\0')
    		cout << "\t(" << it->first.first << ", ) => {";
		else
    	    cout << "\t(" << it->first.first << ", " << it->first.second << ") => { ";
    	for(State const& element : it->second)
    		cout << element << ", ";
    	cout << "}\n";
	}

	cout << "Initial state: " << initial_state << endl;
	cout << "Accepting states: ";
	for(State const& element : accepting_states)
    		cout << element << ", ";
    cout << "\n";

    cout << "------------------------\n";
}

NFA NFA::simpleNFA(char c){
	set<string> states;
	map<pair<string, char>, set<string>> transitions;
	set<string> accepting_states;

	set<string> t;
	t.insert("1");

	states.insert("0");
	states.insert("1");
	transitions.insert(pair<pair<string, char>, set<string>>(std::make_pair("0", c), t));
	accepting_states.insert("1");

	return NFA(states, "0", transitions, accepting_states);
}

void NFA::addTransition(const State& from, const set<State>& to, char symbol){
	this->transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair(from, symbol), to));
}

set<State> NFA::getNewStatesName(const string& prefix){
	set<State> newSates;
	for(State const& state : this->states)
		newSates.insert(prefix + state);
	return newSates;
}
set<State> NFA::getNewStatesName(const set<State>& states, const string& prefix){
	set<State> newSates;
	for(State const& state : states)
		newSates.insert(prefix + state);
	return newSates;
}

NFA NFA::nfa_concat(NFA nfa){
	// New composite states
	set<State> new_states;
	set<State> modified_states1 = NFA::getNewStatesName(this->states, "p");
	set<State> modified_states2 = NFA::getNewStatesName(nfa.states, "q");
	new_states.insert(modified_states1.begin(), modified_states1.end());
	new_states.insert(modified_states2.begin(), modified_states2.end());
	// New initial state
	State new_initial_state = "p" + this->initial_state;
	// New accepting states
	set<State> new_accepting_states;
	new_accepting_states = NFA::getNewStatesName(nfa.accepting_states, "q");
	// New transition function
	map<pair<State, char>, set<State>> new_transitions;
	map<pair<State, char>, set<State>>::iterator map_it;
	for(map_it = this->transitions.begin(); map_it != this->transitions.end(); ++map_it){
		set<State> transition_states;
		transition_states = NFA::getNewStatesName(map_it->second, "p");
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("p" + map_it->first.first, map_it->first.second), transition_states));
	}
	for(map_it = nfa.transitions.begin(); map_it != nfa.transitions.end(); ++map_it){
		set<State> transition_states;
		transition_states = NFA::getNewStatesName(map_it->second, "q");
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("q" + map_it->first.first, map_it->first.second), transition_states));
	}
	set<State> thompson_concat;
	thompson_concat.insert("q" + nfa.initial_state);
	for(State const& state : this->accepting_states){
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("p" + state, '\0'), thompson_concat));
	}

	return NFA(new_states, new_initial_state, new_transitions, new_accepting_states);
}

NFA NFA::nfa_union(NFA nfa){
	// New composite states
	set<State> new_states;
	set<State> modified_states1 = NFA::getNewStatesName(this->states, "p");
	set<State> modified_states2 = NFA::getNewStatesName(nfa.states, "q");
	new_states.insert(modified_states1.begin(), modified_states1.end());
	new_states.insert(modified_states2.begin(), modified_states2.end());
	// New initial state
	State new_initial_state = "i" + this->initial_state  + nfa.initial_state;
	// New accepting states
	State new_final_state = "f" + this->initial_state  + nfa.initial_state;
	set<State> new_accepting_states;
	new_accepting_states.insert(new_final_state);
	// Adding the new states
	new_states.insert(new_initial_state);
	new_states.insert(new_final_state);
	// New transition function
	map<pair<State, char>, set<State>> new_transitions;
	map<pair<State, char>, set<State>>::iterator map_it;
	for(map_it = this->transitions.begin(); map_it != this->transitions.end(); ++map_it){
		set<State> transition_states;
		transition_states = NFA::getNewStatesName(map_it->second, "p");
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("p" + map_it->first.first, map_it->first.second), transition_states));
	}
	for(map_it = nfa.transitions.begin(); map_it != nfa.transitions.end(); ++map_it){
		set<State> transition_states;
		transition_states = NFA::getNewStatesName(map_it->second, "q");
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("q" + map_it->first.first, map_it->first.second), transition_states));
	}
	set<State> s_thompson_concat;
	s_thompson_concat.insert("p" + this->initial_state);
	s_thompson_concat.insert("q" + nfa.initial_state);
	new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair(new_initial_state, '\0'), s_thompson_concat));

	set<State> f_thompson_concat;
	f_thompson_concat.insert(new_final_state);
	for(State const& state : this->accepting_states){
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("p" + state, '\0'), f_thompson_concat));
	}
	for(State const& state : nfa.accepting_states){
		new_transitions.insert(pair<pair<State, char>, set<State>>(std::make_pair("q" + state, '\0'), f_thompson_concat));
	}

	return NFA(new_states, new_initial_state, new_transitions, new_accepting_states);
}

NFA NFA::kleene_clousure(){
	// New composite states
	set<State> new_states;
	State new_initial_state = "p_i" + this->initial_state;
	State new_final_state = "p_f" + this->initial_state;
	new_states = this->states;
	new_states.insert(new_initial_state);
	new_states.insert(new_final_state);
	// New final states
	set<State> new_accepting_states;
	new_accepting_states.insert(new_final_state);
	// Resulting NFA created
	NFA nfa(new_states, new_initial_state, this->transitions, new_accepting_states);
	// New transitions
	set<State> initial_trans_state = new_accepting_states;
	initial_trans_state.insert(this->initial_state);
	nfa.addTransition(new_initial_state, initial_trans_state, '\0');
	set<State> old_initial_state;
	old_initial_state.insert(this->initial_state);
	old_initial_state.insert(new_final_state);
	for(State const& state : this->accepting_states){
		nfa.addTransition(state, old_initial_state, '\0');
	}
	// Returning the result
	return nfa;
}

NFA NFA::plus_clousure(){
	// New composite states
	set<State> new_states;
	State new_initial_state = "p_i" + this->initial_state;
	State new_final_state = "p_f" + this->initial_state;
	new_states = this->states;
	new_states.insert(new_initial_state);
	new_states.insert(new_final_state);
	// New final states
	set<State> new_accepting_states;
	new_accepting_states.insert(new_final_state);
	// Resulting NFA created
	NFA nfa(new_states, new_initial_state, this->transitions, new_accepting_states);
	// New transitions
	set<State> initial_trans_state;
	initial_trans_state.insert(this->initial_state);
	nfa.addTransition(new_initial_state, initial_trans_state, '\0');
	set<State> old_initial_state;
	old_initial_state.insert(this->initial_state);
	old_initial_state.insert(new_final_state);
	for(State const& state : this->accepting_states){
		nfa.addTransition(state, old_initial_state, '\0');
	}
	// Returning the result
	return nfa;
}