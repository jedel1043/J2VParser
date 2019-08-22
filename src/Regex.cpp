#include "Regex.h"
#include "NFA.h"
#include <map>
#include <set>
#include <stack>

using namespace std;

const string Regex::alpha_lower = "a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z";
const string Regex::alpha_upper = "A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z";
const string Regex::digit = "0|1|2|3|4|5|6|7|8|9";

bool Regex::isOperator(char c){
    return c == '*' or c == '|' or c == '.' or c == '+';
}

bool Regex::hasLessOrEqualPriority(char operator1, char operator2){
	map<char, int> operators;
	operators.insert(pair<char,int>('*', 3));
	operators.insert(pair<char,int>('+', 3));
	operators.insert(pair<char,int>('.', 2));
	operators.insert(pair<char,int>('|', 1));
	operators.insert(pair<char,int>('(', 0));
	return operators[operator1] <= operators[operator2];
}

string Regex::preCompile(const string & str){
	string result;

	int flag_union = 0;
	int flag_range = 0;
	int flag_open = 0;
	int flag_begin = 0;
	int flag_concat = 0;

	string range_name;

	for(char const c : str){
		switch(c){
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
					result += ')';
				}
				else if(flag_range){
					flag_range--;
					if(!flag_range){
						if(flag_concat)
							result += ".";
						if(range_name == ":alpha:")
							result += ("(" + Regex::alpha_lower + "|" + Regex::alpha_upper + ")");
						else if(range_name == ":alnum:")
							result += ("(" + Regex::alpha_lower + "|" + Regex::alpha_upper + "|" + Regex::digit + ")");
						else if(range_name == ":digit:")
							result += ("(" + Regex::digit + ")");
						else{
							// Error
						}
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
						result += '.';
					}
					if(flag_open){
						flag_open--;
						result += '(';
						result += c;
					}else{
						result += '|';
						result += c;
					}
				} else if(flag_range){
					range_name += c;
				} else {
					if(!flag_begin)
						flag_begin++;
					else
						result += '.';
					result += c;
					flag_concat = 1;
				}
			continue;
		}
	}

	return result;
}

string Regex::toPostfix(const string& infix){
	stack<char> s;
	string postfix;
	for(char const c : infix){
		if(c == '('){
			s.push(c);
		}
		else if(c == ')'){
			while(s.top() != '('){
				postfix += s.top();
				s.pop();
			}
			s.pop();
		}
		else if(Regex::isOperator(c)){
			while(!s.empty() && s.top() != '(' && Regex::hasLessOrEqualPriority(c, s.top())){
				postfix += s.top();
				s.pop();
			}
			s.push(c);
		}
		else{
			postfix += c;
		}
	}
	while(!s.empty()){
		postfix += s.top();
		s.pop();
	}
	return postfix;
}

NFA Regex::compile(const string& str){
	stack<NFA> results;
	string postfix = Regex::toPostfix(str);
	for(char const c : postfix){
		if(c == '*'){
			NFA nfa = results.top();
			results.pop();
			results.push(nfa.kleene_closure());
		}
		else if(c == '+'){
			NFA nfa = results.top();
			results.pop();
			results.push(nfa.plus_closure());
		}
		else if(c == '.'){
			NFA nfa2 = results.top();
			results.pop();
			NFA nfa1 = results.top();
			results.pop();
			results.push(nfa1.nfa_concat(nfa2));
		}
		else if(c == '|'){
			NFA nfa2 = results.top();
			results.pop();
			NFA nfa1 = results.top();
			results.pop();
			results.push(nfa1.nfa_union(nfa2));
		}
		else{
			NFA nfa = NFA::simpleNFA(c);
			results.push(nfa);
		}
	}
	NFA result = results.top();
	return result;
}
