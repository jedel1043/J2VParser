#include "../include/Regex.h"

#include <map>
#include <set>
#include <stack>

using namespace std;

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
			results.push(nfa.kleene_clousure());
		}
		else if(c == '+'){
			NFA nfa = results.top();
			results.pop();
			results.push(nfa.plus_clousure());
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