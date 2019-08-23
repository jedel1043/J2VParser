#ifndef REGEX_H
#define REGEX_H

#include "NFA.h"
#include <set>
#include <iostream>
#include <iterator>

using namespace std;



class Regex{
	static bool isOperator(int c);
	static bool hasLessOrEqualPriority(int operator1, int operator2);
	static vector<int> toPostfix(const vector<int> & infix);
	static vector<int> toVector(const string & str);
    static vector<char> toCharVector(const vector<int> &v);

public:
	static NFA compile(const vector<int> & str);
	static vector<int> preCompile(const string & str);
    friend std::ostream& operator<< (std::ostream& out, const std::vector<int>& v);
    static const string & getAlphaLower();
    static const string & getAlphaUpper();
    static const string & getDigit();
    static const map<int, char> & getOperators();
    static int getCharValue(char c);

    static vector<int> getCharacterClassVector(const string &char_class);
};

std::ostream& operator<< (std::ostream& out, const std::vector<int>& v);

#endif
