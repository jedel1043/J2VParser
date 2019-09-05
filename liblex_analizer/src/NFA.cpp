#include "NFA.h"

#include "DFA.h"

#include <vector>
#include <stack>
#include <iostream>
#include <cstring>
#include <queue>

using namespace std;

NFA::NFA() = default;

NFA::~NFA() = default;

int NFA::get_size(){
    return states.size();
}

set<char> NFA::getAlphabet(){
    map<pair<int, char>, set<int>>::iterator it;
    set<char> alphabet;
    for(it = transitions.begin(); it != transitions.end(); ++it){
        char c = it->first.second;
        if(c != 0)
            alphabet.insert(c);
    }
    return alphabet;
}

DFA NFA::toDFA(){
    // Configuration for the equivalent DFA
    int n = 1;
    map<pair<int, char>, int> dfa_transition;
    set<char> alphabet = this->getAlphabet();
    set<int> new_states; new_states.insert(1);
    set<int> final_states;

    // Preparing the algorithm
    vector<set<int>> old_states;
    old_states.push_back(this->e_closure(initial_state));
    queue<int> pending_states;
    pending_states.push(1);

    while(!pending_states.empty()){
        int dstate = pending_states.front();
        states.insert(dstate);
        pending_states.pop();
        set<int> state = old_states[dstate-1];

        for(const char c : alphabet){
            char str[2];
            str[0] = c;
            str[1] = '\0';
            set<int> result = this->compute(state, &str[0]);

            if(result.empty()){
                dfa_transition.insert(pair<pair<int, char>, int>(make_pair(dstate, c), -1));
                continue;
            }

            auto it = find(old_states.begin(), old_states.end(), result);
            if(it != old_states.end()){
                int state_index = (int)distance(old_states.begin(), it) + 1;
                dfa_transition.insert(pair<pair<int, char>, int>(make_pair(dstate, c), state_index));
            }else{
                new_states.insert(++n);
                pending_states.push(n);
                old_states.push_back(result);
                dfa_transition.insert(pair<pair<int, char>, int>(make_pair(dstate, c), n));
            }
        }

        vector<int> intersection;
        set_intersection(state.begin(), state.end(), accepting_states.begin(), accepting_states.end(), back_inserter(intersection));
        if(!intersection.empty())
            final_states.insert(dstate);
    }

    return DFA(alphabet, dfa_transition, new_states, 1, final_states);
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

	transitions.insert(pair<pair<int, char>, set<int>>(std::make_pair(0, c), {1}));

	return NFA({0,1}, 0, transitions, {1});
}

void NFA::addTransition(int from, const set<int>& to, char symbol){
    auto it = transitions.find(pair<int,char>(from, symbol));
    if(it  == transitions.end())
        transitions.insert(pair<pair<int, char>, set<int>>(pair<int, char>(from, symbol), to));
    else
        it->second.insert(to.begin(), to.end());
}

map<pair<int, char>, set<int>> NFA::get_new_trans_function(int x){
    map<pair<int, char>, set<int>> new_transition;
    for(auto const & trans : transitions){
        pair<int, char> origin_state(trans.first.first + x, trans.first.second);
        set<int> next_states;
        for(int state : trans.second)
            next_states.insert(state + x);
        new_transition.insert(pair<pair<int, char>, set<int>>(origin_state, next_states));
    }
    return new_transition;
}

NFA NFA::nfa_concat(NFA nfa){
	// New NFA
	NFA result;
	// New composite states
	result.states.insert(states.begin(), states.end());
	for(int state : nfa.states)
	    result.states.insert(state + get_size());

	// New initial state
	result.initial_state = initial_state;
	// New accepting states
    for(int state : nfa.accepting_states)
        result.accepting_states.insert(state + get_size());
	// New transition function

    result.transitions.insert(transitions.begin(), transitions.end());

    map<pair<int, char>, set<int>> new_trans = nfa.get_new_trans_function(get_size());
	result.transitions.insert(new_trans.begin(), new_trans.end());

	for(int state : accepting_states){
		result.addTransition(state, {get_size()}, 0);
	}

	return result;
}

NFA NFA::nfa_union(NFA nfa){
	// New NFA
	NFA result;
	// New composite states
    for(int state : states)
        result.states.insert(state + 1);
    for(int state : nfa.states)
        result.states.insert(state + get_size() + 1);
	// New initial state
	result.initial_state = 0;
	// New accepting states
	int new_final_state = get_size() + nfa.get_size() + 1;
	result.accepting_states.insert(new_final_state);
	// Adding the new states
	result.states.insert(0);
	result.states.insert(new_final_state);
	// New transition function
	auto new_trans_a = get_new_trans_function(1);
	auto new_trans_b = nfa.get_new_trans_function(get_size() + 1);
	result.transitions.insert(new_trans_a.begin(), new_trans_a.end());
    result.transitions.insert(new_trans_b.begin(), new_trans_b.end());

    //create new initial and final states
	result.addTransition(result.initial_state, {initial_state+1, nfa.initial_state + get_size() + 1}, 0);

	for(int const state : this->accepting_states)
		result.addTransition(state+1, {new_final_state}, 0);
	for(int const state : nfa.accepting_states)
		result.addTransition(state + get_size() + 1, {new_final_state}, 0);

	return result;
}

NFA NFA::kleene_closure(){
    NFA result = plus_closure();
    result.addTransition(0, {result.get_size()-1}, 0);
    return result;
}

NFA NFA::plus_closure(){
	// New NFA
	NFA result;
	// New states
    for(int state : states)
        result.states.insert(state + 1);
	// New initial state
	result.initial_state = 0;
	// New final states
	int new_final_state = get_size() + 1;
	result.accepting_states.insert(new_final_state);
	// Adding the new states
	result.states.insert(0);
	result.states.insert(new_final_state);
	// New transitions
	auto new_trans = get_new_trans_function(1);
	result.transitions.insert(new_trans.begin(), new_trans.end());
	// Creates new initial and final states
	result.addTransition(0, {1}, 0);
	result.addTransition(get_size(), {1}, 0);
	for(int state : accepting_states)
		result.addTransition(state + 1, {new_final_state}, 0);
	// Returning the result
	return result;
}

NFA NFA::zero_or_one(){
    // New NFA
    NFA result;
    // New states
    for(int state : states)
        result.states.insert(state + 1);
    // New initial state
    result.initial_state = 0;
    // New final states
    int new_final_state = get_size() + 1;
    result.accepting_states.insert(new_final_state);
    // Adding the new states
    result.states.insert(0);
    result.states.insert(new_final_state);
    // New transitions
    auto new_trans = get_new_trans_function(1);
    result.transitions.insert(new_trans.begin(), new_trans.end());
    // Creates new initial and final states
    result.addTransition(0, {1}, 0);
    result.addTransition(0, {get_size()+1}, 0);
    for(int const state : accepting_states)
        result.addTransition(state + 1, {get_size() + 1}, 0);
    // Returning the result
    return result;
}

string NFA::stringify(){
    string out;
    out.append( "------------------------\n");
    out.append( "States: ");
    for(int const state : this->states)
        out.append(to_string(state) + ", ");
    out.append("\n");

    out.append( "Transition function: \n");
    map<pair<int, char>, set<int>>::iterator it;
    for (it = this->transitions.begin(); it != this->transitions.end(); ++it){
        if(it->first.second == '\0')
            out.append( "\t(" + to_string(it->first.first) + ", ) => {");
        else
            out.append( "\t(" + to_string(it->first.first) + ", " + (char)it->first.second + ") => { ");
        for(int const element : it->second)
            out.append( to_string(element) + ", ");
        out.append( "}\n");
    }

    out.append( "Initial state: " + to_string(this->initial_state) + "\n");
    out.append( "Accepting states: ");
    for(int const element : this->accepting_states)
        out.append( to_string(element) + ", ");
    out.append( "\n");

    out.append( "------------------------\n");
    return out;
}
