#include <iostream>
#include "compiler/grammar_parser/grammar_parser.h"

bool Grammar_Parser::grammar() {
    return rule_list();
}

bool Grammar_Parser::rule_list(){
    if(rule()){
        if(scanner.get() == SEMICOLON){
            return rule_list_p();
        }
    }
    return false;
}

bool Grammar_Parser::rule_list_p(){
    TextSourceBuffer* saver = scanner.get_pos();

    if(rule()){
        if(scanner.get() == SEMICOLON){
            return rule_list_p();
        }
        return false;
    }
    scanner.set_pos(saver);
    return true;
}

bool Grammar_Parser::rule(){
    string left_symbol;
    if(left(left_symbol)){
        if(scanner.get() == COLON){
            return right_sides(left_symbol);
        }
    }
    return false;
}

bool Grammar_Parser::left(string &left_symbol) {
    Token t = scanner.get();
    if(t == L) {
        left_symbol += t.getLexeme();
        return true;
    }
    return false;
}

bool Grammar_Parser::right_sides(string &left_symbol){
    string right;
    if(symbols(right)){
        parsed_grammar.insert(left_symbol, right);
        return right_sides_p(left_symbol);
    }
    return false;
}

bool Grammar_Parser::right_sides_p(string &left_symbol){
    string right;
    if(scanner.get() == OR){
        if(symbols(right)){
            parsed_grammar.insert(left_symbol, right);
            return right_sides_p(left_symbol);
        }
        return false;
    }
    scanner.pushBack();
    return true;
}

bool Grammar_Parser::symbols(string &right){
    Token t = scanner.get();
    if(t == L){
        right += t.getLexeme();
        return symbols_p(right);
    }
    return false;
}

bool Grammar_Parser::symbols_p(string &right) {
    Token t = scanner.get();
    if(t == L){
        right += t.getLexeme();
        return symbols_p(right);
    }
    scanner.pushBack();
    return true;
}
