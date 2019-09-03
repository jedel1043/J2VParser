#include "DFA.h"

#include <iostream>
#include <boost/format.hpp>

using namespace std;

string DFA::stringify(){
    string out;
	out += "     |";
    for(const char c : alphabet)
        out += (boost::format("  %c  |") % c).str();
    out += "\n------";
    for(int i = 1; i <= (int)alphabet.size(); i++)
        out += "------";
    out += '\n';

	for(int i=1; i<=get_size(); i++){
		if(accepting_states.count(i) != 0)
			out.append("*");
		else
			out.append(" ");
		if(initial_state == i)
			out.append(">");
		else
			out.append(" ");

        out += (boost::format("%2d |") % i).str();
        for(const char c : alphabet){
            out += (boost::format(" %3d |") % transitions[make_pair(i, c)]).str();
        }
		out += '\n';
	}
	return out;
}

set<int> DFA::inverse_transition(const set<int> & in_states, char c){
	set<int> result;
	map<pair<int, char>, int>::iterator it;
	for(const int state : in_states){
		for(it=transitions.begin(); it!=transitions.end(); ++it){
			if(it->second == state && it->first.second == c)
				result.insert(it->first.first);
		}
	}
	return result;
}

DFA DFA::minimize(){
	set<set<int>> P;
	P.insert(accepting_states);

	set<int> intersection;
	set_difference(states.begin(), states.end(), accepting_states.begin(), accepting_states.end(), 
		inserter(intersection, intersection.end()));
	if (!intersection.empty())
	    P.insert(intersection);

	set<set<int>> W;
	W.insert(accepting_states);

	while(!W.empty()){

		set<int> A = *W.begin();
		W.erase(W.begin());
		for(const char c : alphabet){
			set<int> X = this->inverse_transition(A, c);
			for(const auto & it : P){
				const set<int>& Y = it;
				set<int> XY;
				set<int> Y_X;
				set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), inserter(XY, XY.end()));
				set_difference(Y.begin(), Y.end(), X.begin(), X.end(), inserter(Y_X, Y_X.end()));
				if(XY.empty() || Y_X.empty())
				    continue;
                P.erase(it);
				P.insert(XY);
				P.insert(Y_X);
				if(W.count(Y) != 0){
					W.erase(it);
					W.insert(XY);
					W.insert(Y_X);
				}else{
					if(XY.size() <= Y_X.size())
						W.insert(XY);
					else
						W.insert(Y_X);
				}
			}
		}
	}
	set<int> new_states;
	map<pair<int, char>, int> new_transitions;
	map<pair<int, char>, int>::iterator itt;

	for(int i=1; i <= (int)P.size(); i++)
        new_states.insert(i);

	for(itt=transitions.begin(); itt!=transitions.end(); ++itt){
		int from_state = 1, to_state = 1;
		bool from_flag = true, to_flag = true;
		for(const set<int> & class_p : P){
			if(class_p.count(itt->first.first) != 0) from_flag = false;
			else if(from_flag) from_state++;

			if(itt->second == -1){
				to_state = -1;
				to_flag = false;
			}
			else if(class_p.count(itt->second) != 0) to_flag = false;
			else if(to_flag) to_state++;
		}
		new_transitions.insert(pair<pair<int, char>, int>(make_pair(from_state, itt->first.second), to_state));
	}
	int new_initial = 1;
	for(const set<int>& class_p : P){
		if(class_p.count(initial_state) != 0) break;
		else new_initial++;
	}
	set<int> new_accepting;
	int current_class = 1;
	for(const set<int>& class_p : P){
		for(const int state : accepting_states){
			if(class_p.count(state) != 0) new_accepting.insert(current_class);
		}
		current_class++;
	}
	return DFA(alphabet, new_transitions, new_states, new_initial, new_accepting);
}

int DFA::compute(const string& str){
	int current_state = initial_state;
	for(const char c : str){
		current_state = transitions[make_pair(current_state, c)];
		if(current_state==-1) return -1;
	}
	cout << endl;
	return current_state;
}

bool DFA::accept(const string& str){
	int result = this->compute(str);
	for(const int state : accepting_states){
		if(state == result)
			return true;
	}
	return false;
}

int DFA::get_size() {
    return states.size();
}
