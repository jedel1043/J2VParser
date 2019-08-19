#include "NFA.h"
#include "Regex.h"

#include <iostream>

using namespace std;

int main(){
	NFA result = Regex::compile("a.b*.(c|d).b+");
	cout << "T: abbbcb: " << result.accept("abbbcb") << endl;
	cout << "F: abbbbbbbbd: " << result.accept("abbbbbbbbd") << endl;
	cout << "F: bbbbcbbb: " << result.accept("bbbbcbbb") << endl;
	cout << "T: acb: " << result.accept("acb") << endl;
	cout << "F: abbbbbbbbbbbbcdbbb: " << result.accept("abbbbbbbbbbbbcdbbb") << endl;
	cout << "T: adbbbbbbbbbbbbb: " << result.accept("adbbbbbbbbbbbbb") << endl;

}