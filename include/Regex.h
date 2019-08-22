#ifndef REGEX_H
#define REGEX_H

#include "NFA.h"
#include <set>
#include <iostream>

using namespace std;

class Regex{
	static bool isOperator(char c);
	static bool hasLessOrEqualPriority(char operator1, char operator2);
	static string toPostfix(const string& infix);

	static const string alpha_lower;
	static const string alpha_upper;
	static const string digit;

public:
	static NFA compile(const string & str);
	static string preCompile(const string & str);
};

#endif
