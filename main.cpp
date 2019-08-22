#include "NFA.h"
#include "Regex.h"

#include <iostream>
#include <set>
#include <cstring>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]){
	string test(argv[1]);
	NFA result = Regex::compile(Regex::preCompile(argv[1]));
	cout << "Accept? " << argv[2] << ": " << result.accept(argv[2]) << endl;

	/*NFA result = Regex::compile("(0|1|2|3|4|5|6|7|8|9).-.(0|1|2|3|4|5|6|7|8|9)");
	cout << "T: 125446-47390124: " << result.accept("125446-47390124") << endl;
	cout << "F: 125446: " << result.accept("125446") << endl;*/

	vector<int> r = Regex::preCompile(test);
	for(int const d : r){
		if(d>0)
			cout << (char) d << " ";
		else
			cout << d << " ";
	}

	return 0;
}
