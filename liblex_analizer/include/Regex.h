#ifndef REGEX_H
#define REGEX_H

#include "NFA.h"
#include <vector>
#include <map>
#include <iostream>

using namespace std;



class Regex{
	static bool isOperator(int c);
	static bool hasLessOrEqualPrecedence(int operator1, int operator2);
	static vector<int> toPostfix(const vector<int> & infix);
	static vector<int> toVector(const string & str);
    static vector<char> toCharVector(const vector<int> &v);
    static NFA compile(const vector<int> & str_vector);

public:
    static NFA compile(const string &str);
	static vector<int> preCompile(const string & str);
    friend std::ostream& operator<< (std::ostream& out, const std::vector<int>& v);
    static const string & getAlphaLower();
    static const string & getAlphaUpper();
    static const string & getDigit();
    static const map<int, char> & getOperators();
    static int getCharValue(char c);

    static vector<int> getCharacterClassVector(const string &char_class);

    static bool areParenthesisBalanced(const string &regex);
};

std::ostream& operator<< (std::ostream& out, const std::vector<int>& v);

#endif
