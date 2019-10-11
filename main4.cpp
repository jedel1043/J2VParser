#include <iostream>
#include <vector>
#include <io/buffer.h>
#include <error.h>
#include <compiler/macros.h>
#include <compiler/Grammar_Scanner.h>
#include <compiler/grammar_parser/grammar_parser.h>
#include <compiler/grammar_parser/grammar_array.h>
#include <automata/NFA.h>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {

    TextSourceBuffer buf("prueba2.txt");
    Grammar_Scanner scanner(&buf);
    Grammar_Parser parser(scanner);
    Grammar_Array ga = parser.parse();

    ga.print_collection();

    return 0;

}

