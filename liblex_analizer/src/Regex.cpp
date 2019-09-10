#include "Regex.h"
#include <set>
#include <stack>
#include <iterator>

int _CRT_glob = 0;

using namespace std;

const string & Regex::getAlphaLower(){
    static const string ALPHA_LOWER = "a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z";
    return ALPHA_LOWER;
}

const string & Regex::getAlphaUpper(){
    static const string ALPHA_UPPER = "A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z";
    return  ALPHA_UPPER;
}
const string & Regex::getDigit(){
    static const string DIGIT = "0|1|2|3|4|5|6|7|8|9";
    return DIGIT;
}
const map<int, char> & Regex::getOperators(){
    // '(': 0, '|': -1, '.': -2, '+': -3, '*': -4, ')': -5, '?': -6
    static const map<int, char> OPS = {{0, '('}, {-1, '|'}, {-2, '.'}, {-3, '+'}, {-4, '*'}, {-5, ')'}, {-6, '?'}};
    return OPS;
}

int Regex::getCharValue(char c){
    switch (c) {
        case '(':
            return 0;
        case '|':
            return -1;
        case '.':
            return -2;
        case '+':
            return -3;
        case '*':
            return -4;
        case ')':
            return -5;
        case '?':
            return -6;
        default:
            return (int) c;
    }
}

bool Regex::isOperator(int c){
	return c<=0;
}

bool Regex::hasLessOrEqualPrecedence(int operator1, int operator2){
    // '(': 0, '|': -1, '.': -2, '+': -3, '*': -4, ')': -5, '?': -6
	map<int, int> operators;
    operators.insert(pair<char,int>(0, 4));
    operators.insert(pair<char,int>(-5, 4));
    operators.insert(pair<char,int>(-6, 5));
	operators.insert(pair<char,int>(-4, 5));
	operators.insert(pair<char,int>(-3, 5));
	operators.insert(pair<char,int>(-2, 6));
	operators.insert(pair<char,int>(-1, 8));


	return operators[operator1] >= operators[operator2];
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

vector<char> Regex::toCharVector(const vector<int> & v){
    vector<char> result;
    for (int const c : v){
        if (c <= 0)
            result.push_back(Regex::getOperators().find(c)->second);
        else
            result.push_back((char) c);
    }
    return result;
}

vector<int> Regex::getCharacterClassVector(const string & char_class){
    vector<int> result;
    if(char_class == "alpha"){
        for(char c : Regex::getAlphaUpper())
            result.push_back(Regex::getCharValue(c));
        result.push_back(Regex::getCharValue('|'));
        for(char c : Regex::getAlphaLower())
            result.push_back(Regex::getCharValue(c));
    }
    else if (char_class == "alnum"){
        for(char c : Regex::getAlphaUpper())
            result.push_back(Regex::getCharValue(c));
        result.push_back(Regex::getCharValue('|'));
        for(char c : Regex::getAlphaLower())
            result.push_back(Regex::getCharValue(c));
        result.push_back(Regex::getCharValue('|'));
        for(char c : Regex::getDigit())
            result.push_back(Regex::getCharValue(c));
    }
    else if (char_class == "digit"){
        for(char c : Regex::getDigit())
            result.push_back(Regex::getCharValue(c));
    }
    else if (char_class == "upper"){
        for(char c : Regex::getAlphaUpper())
            result.push_back(Regex::getCharValue(c));
    }
    else if (char_class == "lower"){
        for(char c : Regex::getAlphaLower())
            result.push_back(Regex::getCharValue(c));
    }
    return result;
}

bool Regex::areParenthesisBalanced(const string & regex){
    stack<char> aux_stack;
    string::const_iterator it = regex.begin();
    while(it < regex.end()){
        if(*it == '\\')
            it+=2;
        else if (*it == '(' || *it == '[')
            aux_stack.push(*it++);
        else if (*it == ')' || *it == ']'){
            if(!aux_stack.empty() && ((aux_stack.top() == '(' && *it == ')') || (aux_stack.top() == '[' && *it == ']'))) {
                aux_stack.pop();
                it++;
            }
            else
                return false;
        } else it++;
    }
    return aux_stack.empty();
}

vector<int> Regex::preCompile(const string & str){
    if(!Regex::areParenthesisBalanced(str))
        throw invalid_argument("Invalid regex input. Unbalanced parenthesis detected.");
	vector<int> result;
	const string & op_search_util = ")+*?|";
	string::const_iterator it = str.begin();
    while(it != str.end()) {
        if (it != str.begin() && *(it - 1) != '(' && *(it - 1) != '|' && op_search_util.find(*it) == string::npos)
            result.push_back(Regex::getCharValue('.'));
        if (*it == '[') {
            result.push_back(Regex::getCharValue('('));
            it++;
            if (*it == '[' && *(it + 1) == ':') {
                string saver;
                vector<int> char_class_vector;
                it += 2;
                while (*it != ':')
                    saver += *it++;
                it += 2;
                char_class_vector = Regex::getCharacterClassVector(saver);
                result.insert(result.end(), char_class_vector.begin(), char_class_vector.end());
            }
            else {
                while (*it != ']') {
                    it += *it == '\\' ? 1 : 0;
                    if (*(it + 1) == '-' && *(it+2) != ']') {
                        bool is_special_char = *(it + 2) == '\\';
                        if (*it > (is_special_char ? *(it + 3) : *(it + 2)))
                            throw invalid_argument(
                                    "Range values reversed. Start char code must be less than end char code.");
                        for (int i = ((int) *it); i <= ((int) is_special_char ? *(it + 3) : *(it + 2)); i++) {
                            result.push_back(i);
                            result.push_back(Regex::getCharValue('|'));
                        }
                        it += (is_special_char ? 4 : 3);
                    } else {
                        result.push_back((int) *it++);
                        result.push_back(Regex::getCharValue('|'));
                    }
                }
                if (result.back() == Regex::getCharValue('|'))
                    result.pop_back();
            }
            result.push_back(Regex::getCharValue(')'));
            it++;
        } else {
            if (*it == '\\') {
                if(it + 1 == str.end())
                    throw invalid_argument("Escape operator without char at end of the string. ");
                result.push_back((int) *++it);
                it++;
            } else
                result.push_back(Regex::getCharValue(*it++));
        }
    }
	return result;
}

vector<int> Regex::toPostfix(const vector<int> & infix){
	stack<int> s;
	vector<int> postfix;
	for(int const c : infix){
		if(c == 0)
			s.push(c);
		else if(c == -5){
			while(s.top() != 0){
				postfix.push_back(s.top());
				s.pop();
			}
			s.pop();
		}
		else if(Regex::isOperator(c)){
			while(!s.empty() && s.top() != 0 && Regex::hasLessOrEqualPrecedence(c, s.top())){
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

NFA Regex::compile(const string & str){
    return Regex::compile(Regex::preCompile(str));
}

NFA Regex::compile(const vector<int> & str_vector){
	// '(': 0, '|': -1, '.': -2, '+': -3, '*': -4, ')': -5, '?': -6
	stack<NFA> results;
	vector<int> postfix = Regex::toPostfix(str_vector);
	for(int c : postfix){
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

std::ostream& operator<< (std::ostream& out, const std::vector<int>& v) {
    vector<char> vector = Regex::toCharVector(v);
    if ( !v.empty() ) {
        out << '[';
        std::copy (vector.begin(), vector.end(), std::ostream_iterator<char>(out, ", "));
        out << "\b\b]";
    }
    return out;
}
