#include "DFA.h"

#include <iostream>
#include <boost/format.hpp>

using namespace std;



string DFA::stringify(){

    int groupingLen = 20;
    int numberOfGroups = (int) alphabet.size() / groupingLen;
    set<set<char>> groups;
    string out;

    for(int i = 0; i <= numberOfGroups; i++){
        auto inicio = alphabet.begin();
        auto fin = alphabet.begin();
        set<char> temp;

        advance(inicio, i*groupingLen);

        if(i == numberOfGroups){
            fin = alphabet.end();
        }
        else{
            advance(fin, (i+1)*groupingLen);
        }

        temp.insert(inicio, fin);

        out += "     |";

        for(const char c : temp)
            out += (boost::format("  %c  |") % c).str();
        out += "\n------";
        for (int j = 1; j <= (int) temp.size(); j++)
            out += "------";
        out += '\n';

        for(int k=1; k <= get_size(); k++){
            if(accepting_states.count(k) != 0)
                out.append("*");
            else
                out.append(" ");
            if(initial_state == k)
                out.append(">");
            else
                out.append(" ");

            out += (boost::format("%2d |") % k).str();
            for(const char c : temp){
                out += (boost::format(" %3d |") % transitions[make_pair(k, c)]).str();
            }
            out += '\n';
        }

        out += "\n\n";

    }

    out += "\n\n";

    out += "     |";
    out += (boost::format("  %s  |") % "token").str();
    out += "\n------------\n";

    for(int k : states){
        if(accepting_states.count(k) != 0)
            out.append("*");
        else
            out.append(" ");
        if(initial_state == k)
            out.append(">");
        else
            out.append(" ");

        out += (boost::format("%2d |") % k).str();
        out += (boost::format(" %3d |") % get_token(k)).str();
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
    intersection.insert(-1);
    set_difference(states.begin(), states.end(), accepting_states.begin(), accepting_states.end(),
                   inserter(intersection, intersection.end()));
    P.insert(intersection);
    set<set<int>> W;
    W.insert(accepting_states);
    while(!W.empty()){
        set<int> A = *W.begin();
        W.erase(W.begin());
        for(const char c : alphabet){
            set<int> X = this->inverse_transition(A, c);
            auto it = P.begin();
            while(it != P.end()){
                set<int> Y = *it++;
                set<int> XY;
                set<int> Y_X;
                set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), inserter(XY, XY.end()));
                set_difference(Y.begin(), Y.end(), X.begin(), X.end(), inserter(Y_X, Y_X.end()));
                if(XY.empty() || Y_X.empty()) continue;
                P.erase(Y);
                P.insert(XY);
                P.insert(Y_X);
                if(W.count(Y) != 0){
                    W.erase(Y);
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

	for(int i=1; i <= (int)P.size()-1; i++)
        new_states.insert(i);
    {
        int i = 1;
        for(const auto& it : P){
            if (it.count(-1))
                continue;
            for(const char & c : alphabet){
                int next_state = transitions[make_pair(*it.begin(), c)];
                if(next_state == -1)
                    new_transitions.insert(pair<pair<int, char>, int>(make_pair(i, c), -1));
                else{
                    int j = 1;
                    for(const auto& it2 : P){
                        if (it2.count(-1))
                            continue;
                        if(it2.count(next_state)){
                            new_transitions.insert(pair<pair<int, char>, int>(make_pair(i, c), j));
                            break;
                        }
                        j++;
                    }
                }
            }
            i++;
        }
    }
	int new_initial = 1;
	for(const set<int>& class_p : P){
		if(class_p.count(initial_state) || class_p.count(-1)) break;
		else new_initial++;
	}
	set<int> new_accepting;
	int current_class = 1;
	for(const set<int>& class_p : P){
	    if(class_p.count(-1))
            continue;
		for(const int state : accepting_states){
			if(class_p.count(state)) new_accepting.insert(current_class);
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

int DFA::get_token(int state){
    auto it = tokens.find(state);
    return it != tokens.end() ? it->second : -1;
}

DFA::DFA(set<char> alphabet, map<pair<int, char>, int> transitions, set<int> states, int initial_state,
         set<int> accepting_states):
         alphabet(move(alphabet)),
         transitions(move(transitions)),
         states(move(states)),
         initial_state(initial_state),
         accepting_states(move(accepting_states)){
    {
        int counter=1;
        for (int state : this->accepting_states)
            tokens.insert(pair<int, int>(state, counter++));
    }
}
