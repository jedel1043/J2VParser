#include <compiler/regex_scanner.h>
#include <compiler/regex_parser.h>
#include "compiler/grammar_parser/grammar_analyzer.h"


const string grammar_analyzer::f_grammar_regex("grammar_regex.txt");

grammar_analyzer::grammar_analyzer(TextSourceBuffer *inputFile):
lexical_analyzer_f(inputFile, create_automaton()){}

DFA create_automaton() {
    TextSourceBuffer buf(grammar_analyzer::f_grammar_regex);
    Regex_Scanner scanner(&buf);
    Regex_Parser parser(scanner);
    vector<NFA> machines = parser.parse();
    NFA result = NFA::lexical_union(machines);
    return result.toDFA().minimize();
}


