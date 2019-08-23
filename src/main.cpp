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
//	cout << "T: acb: " << resul t.accept("acb") << endl;
//	cout << "F: abbbbbbbbbbbbcdbbb: " << result.accept("abbbbbbbbbbbbcdbbb") << endl;
//	cout << "T: adbbbbbbbbbbbbb: " << result.accept("adbbbbbbbbbbbbb") << endl;
//
//    NFA result;
//    cout << result << endl;

	cout << Regex::compile(argv[1]) << endl;

	return 0;
}
