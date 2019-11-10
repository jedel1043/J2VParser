#include "compiler/lexical_analyzer/lexical_analyzer_f.h"

#include <utility>

lexical_analyzer_f::lexical_analyzer_f(TextSourceBuffer *inputFile, DFA automata, bool skip_whitespace) :
lexical_analyzer(std::move(automata), skip_whitespace), input_file(inputFile){}

Token lexical_analyzer_f::yylex() {
    if (is_in_end())
        return current_token = Token("$", "$");
    std::string token;
    std::string lexeme;
    char c = input_file->getChar();
    int actual_state = automata.getInitialState();

    while(actual_state != -1){
        if(is_in_end() || is_eos(c))
            break;
        lexeme += c;
        actual_state = automata.compute(actual_state, c);
        if(automata.getAcceptingStates().count(actual_state))
            token = automata.getTokens().at(actual_state);
        c = input_file->fetchChar();
    }
    if(is_eos(c) && actual_state != -1)
        skip_ws();
    else if(!is_in_end()) {
        input_file->putBackChar();
        lexeme.pop_back();
    }
    return current_token = Token(token, lexeme);
}

bool lexical_analyzer_f::is_in_end() {
    return input_file->getChar() == EOFChar;
}

char lexical_analyzer_f::skip_ws(){
    if(skip_whitespace){
        while(is_eos(input_file->getChar()))
            input_file->fetchChar();
    }
    return input_file-> getChar();
}

bool lexical_analyzer_f::is_eos(char c){
    return isspace(c) || c == '\0';
}


