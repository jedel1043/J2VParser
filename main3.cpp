#include "compiler/grammar_parser/grammar_array.h"


using namespace std;

int main() {

    Grammar_Array test;

    test.insert("R", "aaBRac");
    test.insert("M", "aaMRc");
    test.insert("C", "aRb");
    test.insert("C", "aCb");
    test.insert("C", "aDb");
    test.insert("F", "aMCr");
    test.insert("D", "RDa");

    test.print_collection();
    test.print_right_sides("D");


    return 0;
}