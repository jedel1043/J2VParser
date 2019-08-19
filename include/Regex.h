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

public:
	static NFA compile(const string& str);
};

#endif