#include "Regex.h"
#include "NFA.h"
#include <map>
#include <set>
#include <stack>
#include <cstring>
#include <iostream>

using namespace std;

const string Regex::alpha_lower = "a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z";
const string Regex::alpha_upper = "A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z";
const string Regex::digit = "0|1|2|3|4|5|6|7|8|9";

bool Regex::isOperator(int c){
	return c<=0;
}

bool Regex::hasLessOrEqualPriority(int operator1, int operator2){
	map<int, int> operators;
	operators.insert(pair<char,int>(-5, 4));
	operators.insert(pair<char,int>(-4, 3));
	operators.insert(pair<char,int>(-3, 3));
	operators.insert(pair<char,int>(-2, 2));
	operators.insert(pair<char,int>(-1, 1));
	operators.insert(pair<char,int>(0, 0));

	return operators[operator1] <= operators[operator2];
}

vector<int> Regex::toVector(const string & str){
	vector<int> result;
	for(char const c : str){
		if(c != '|')
			result.push_back((int) c);
		else
			result.push_back(-1);
	}
	return result;
}

vector<int> Regex::preCompile(const string & str){
	// '(': 0, '|': -1, '.': -2, '+': -3, '*': -4, ')': -5, '?': -6
	vector<int> result;

	int flag_union = 0;
	int flag_range = 0;
	int flag_open = 0;
	int flag_begin = 0;
	int flag_concat = 0;
	int flag_escape = 0;

	string range_name;

	for(char const c : str){
		switch(c){
			case '+':
			case '*':
			case '?':
				if(flag_escape){
					flag_escape--;
					if(flag_union){
						if(!flag_begin)
							flag_begin++;
						if(flag_concat && flag_open){
							flag_concat = 0;
							result.push_back(-2);
						}
						if(flag_open){
							flag_open--;
							result.push_back(0);
							result.push_back((int) c);
						}else{
							result.push_back(-1);
							result.push_back((int) c);
						}
					} else if(flag_range){
						range_name += c;
					} else {
						if(!flag_begin)
							flag_begin++;
						else
							result.push_back(-2);
						result.push_back((int) c);
						flag_concat = 1;
					}
				}else{
					if(c == '+') result.push_back(-3);
					else if(c == '*') result.push_back(-4);
					else result.push_back(-6);
				}
			continue;

			case '\\':
				if(!flag_escape)
					flag_escape++;
			continue;

			case '[':
				if(flag_range >= 2){
					/* Error */
				}
				else if(flag_union){
					flag_range += 2;
					flag_union--;
					flag_open--;
				}
				else{
					flag_union++;
					flag_open++;
				}
			continue;

			case ']':
				if(flag_union){
					flag_union--;
					result.push_back(-5);
					flag_concat = 1;
				}
				else if(flag_range){
					flag_range--;
					if(!flag_range){
						if(!flag_begin)
							flag_begin++;
						if(flag_concat)
							result.push_back(-2);
						if(range_name == ":alpha:"){
							vector<int> range;
							result.push_back(0);
							range = Regex::toVector(Regex::alpha_lower);
							result.insert(result.end(), range.begin(), range.end());
							result.push_back(-1);
							range = Regex::toVector(Regex::alpha_upper);
							result.insert(result.end(), range.begin(), range.end());
							result.push_back(-5);
						}
						else if(range_name == ":alnum:"){
							vector<int> range;
							result.push_back(0);
							range = Regex::toVector(Regex::alpha_lower);
							result.insert(result.end(), range.begin(), range.end());
							result.push_back(-1);
							range = Regex::toVector(Regex::alpha_upper);
							result.insert(result.end(), range.begin(), range.end());
							result.push_back(-1);
							range = Regex::toVector(Regex::digit);
							result.insert(result.end(), range.begin(), range.end());
							result.push_back(-5);
						}
						else if(range_name == ":digit:"){
							vector<int> range;
							result.push_back(0);
							range = Regex::toVector(Regex::digit);
							result.insert(result.end(), range.begin(), range.end());
							result.push_back(-5);
						}
						else{
							// Error
						}
						range_name = "";
					}
				}
				else{
					/* Error */
				}
			continue;

			default:
				if(flag_union){
					if(!flag_begin)
						flag_begin++;
					if(flag_concat && flag_open){
						flag_concat = 0;
						result.push_back(-2);
					}
					if(flag_open){
						flag_open--;
						result.push_back(0);
						result.push_back((int) c);
					}else{
						result.push_back(-1);
						result.push_back((int) c);
					}
				} else if(flag_range){
					range_name += c;
				} else {
					if(!flag_begin)
						flag_begin++;
					else
						result.push_back(-2);
					result.push_back((int) c);
					flag_concat = 1;
				}
			continue;
		}
	}
	return result;
}

vector<int> Regex::toPostfix(const vector<int> & infix){
	stack<int> s;
	vector<int> postfix;
	for(int const c : infix){
		if(c == 0){
			s.push(c);
		}
		else if(c == -5){
			while(s.top() != 0){
				postfix.push_back(s.top());
				s.pop();
			}
			s.pop();
		}
		else if(Regex::isOperator(c)){
			while(!s.empty() && s.top() != 0 && Regex::hasLessOrEqualPriority(c, s.top())){
				postfix.push_back(s.top());
				s.pop();
			}
			s.push(c);
		}
		else{
			postfix.push_back(c);
		}
	}
	while(!s.empty()){
		postfix.push_back(s.top());
		s.pop();
	}
	return postfix;
}

NFA Regex::compile(const vector<int> & str){
	// '(': 0, '|': -1, '.': -2, '+': -3, '*': -4, ')': -5, '?': -6
	stack<NFA> results;
	vector<int> postfix = Regex::toPostfix(str);
	for(int const c : postfix){
		if(c == -4){
			NFA nfa = results.top();
			results.pop();
			results.push(nfa.kleene_closure());
		}
		else if(c == -3){
			NFA nfa = results.top();
			results.pop();
			results.push(nfa.plus_closure());
		}
		else if(c == -6){
			NFA nfa = results.top();
			results.pop();
			results.push(nfa.zero_or_one());
		}
		else if(c == -2){
			NFA nfa2 = results.top();
			results.pop();
			NFA nfa1 = results.top();
			results.pop();
			results.push(nfa1.nfa_concat(nfa2));
		}
		else if(c == -1){
			NFA nfa2 = results.top();
			results.pop();
			NFA nfa1 = results.top();
			results.pop();
			results.push(nfa1.nfa_union(nfa2));
		}
		else{
			NFA nfa = NFA::simpleNFA((char) c);
			results.push(nfa);
		}
	}
	NFA result = results.top();
	return result;
}
