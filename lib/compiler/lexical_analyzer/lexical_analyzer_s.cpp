#include "compiler/lexical_analyzer/lexical_analyzer_s.h"
#include <utility>

lexical_analyzer_s::lexical_analyzer_s(DFA automata, std::string strInput, bool skip_whitespace) :
        lexical_analyzer(std::move(automata), skip_whitespace),
        str_input(std::move(strInput)){
    str_pos = str_input.begin();
}

Token lexical_analyzer_s::yylex(){
    if(is_in_end())
        return current_token = Token("$", "$");
    std::string token;
    std::string lexeme;
    int actual_state = automata.getInitialState();

    while(actual_state != -1){
        skip_ws();
        if(is_in_end())
            break;
        lexeme += *str_pos;
        actual_state = automata.compute(actual_state, *str_pos);
        if(automata.getAcceptingStates().count(actual_state))
            token = automata.getTokens().at(actual_state);
        str_pos++;
    }
    if (!is_in_end()) {
        str_pos--;
        lexeme.pop_back();
    }
    return current_token = Token(token, lexeme);
}

bool lexical_analyzer_s::is_in_end(){
    return str_pos == str_input.end();
}

void lexical_analyzer_s::set_str_input(const string &new_str) {
    str_input = new_str;
    str_pos = str_input.begin();
}

char lexical_analyzer_s::skip_ws(){
    if(skip_whitespace){
        while(isspace(*str_pos)) str_pos++;
    }
    return *str_pos;
}
