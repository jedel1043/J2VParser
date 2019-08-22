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

	static const string alpha_lower;
	static const string alpha_upper;
	static const string digit;

public:
	static NFA compile(const vector<int> & str);
	static vector<int> preCompile(const string & str);
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if ( !v.empty() ) {
        out << '[';
        std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

#endif
