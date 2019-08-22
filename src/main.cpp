#include "NFA.h"
#include "Regex.h"

#include <iostream>

using namespace std;

int main(int argc, char const *argv[]){
//	NFA result = Regex::compile("a.b*.(c|d).b+");
//	cout << result << endl;
//	cout << "T: abbbcb: " << result.accept("abbbcb") << endl;
//	cout << "F: abbbbbbbbd: " << result.accept("abbbbbbbbd") << endl;
//	cout << "F: bbbbcbbb: " << result.accept("bbbbcbbb") << endl;
//	cout << "T: acb: " << result.accept("acb") << endl;
//	cout << "F: abbbbbbbbbbbbcdbbb: " << result.accept("abbbbbbbbbbbbcdbbb") << endl;
//	cout << "T: adbbbbbbbbbbbbb: " << result.accept("adbbbbbbbbbbbbb") << endl;
//
//	NFA result = Regex::compile("(0|1|2|3|4|5|6|7|8|9).-.(0|1|2|3|4|5|6|7|8|9)");
//	cout << "T: 125446-47390124: " << result.accept("125446-47390124") << endl;
//	cout << "F: 125446: " << result.accept("125446") << endl;

	string test(argv[1]);
	cout << Regex::preCompile(test) << endl;

	return 0;
}
