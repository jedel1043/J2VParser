#ifndef REGEX_H
#define REGEX_H

#include "NFA.h"
#include <set>
#include <iostream>

using namespace std;

class Regex{
	static bool isOperator(int c);
	static bool hasLessOrEqualPriority(int operator1, int operator2);
	static vector<int> toPostfix(const vector<int> & infix);
	static vector<int> toVector(const string & str);

	static const string alpha_lower;
	static const string alpha_upper;
	static const string digit;

public:
	static NFA compile(const vector<int> & str);
	static vector<int> preCompile(const string & str);
};

#endif
