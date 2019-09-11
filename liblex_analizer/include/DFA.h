#ifndef DFA_H
#define DFA_H

#include <map>
#include <set>
#include <cstdio>
#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>

using namespace std;

class DFA{
	set<int> states;
	set<char> alphabet;
	map<pair<int, char>, int> transitions;
	int initial_state;
	set<int> accepting_states;
	map<int, int> tokens;

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive & ar, const unsigned int version){
	    ar & states;
	    ar & alphabet;
	    ar & transitions;
	    ar & initial_state;
	    ar & accepting_states;
	}

	set<int> inverse_transition(const set<int> & in_states, char c);
	int compute(const string& str);

public:
	DFA(set<char> alphabet, map<pair<int, char>, int> transitions, set<int> states, int initial_state, set<int> accepting_states);
	string stringify();
	int get_size();
	DFA minimize();
	bool accept(const string& str);

    int get_token(int state);
};

#endif