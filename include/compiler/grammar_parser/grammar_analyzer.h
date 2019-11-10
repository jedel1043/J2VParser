#ifndef COMPILER_GRAMMAR_ANALYZER_H
#define COMPILER_GRAMMAR_ANALYZER_H


#include <compiler/lexical_analyzer/lexical_analyzer_f.h>

class grammar_analyzer : public lexical_analyzer_f{
private:
    static const string f_grammar_regex;
    friend DFA create_automaton();
public:
    explicit grammar_analyzer(TextSourceBuffer *inputFile);
};

DFA create_automaton();

#endif //COMPILER_GRAMMAR_ANALYZER_H
