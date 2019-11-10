#include "compiler/grammar_parser/grammar_parser.h"

void grammar_parser::grammar() {
    while(scanner.get_current_token() != "$"){
        rule();
    }
}

void grammar_parser::rule(){
    string left_s;
    if(scanner.yylex() == "VAR"){
        left_s = scanner.get_current_token().get_lexeme();
        if(scanner.yylex() == "COLON"){
            while(scanner.get_current_token() != "SEMICOLON")
                right_side(left_s);
        } else SyntaxError(MissingColon);
    }
    else if (scanner.get_current_token() == "$");
    else SyntaxError(MissingRuleName);
}

void grammar_parser::right_side(const string& left_s){
    std::vector<std::string> right_s;
    Token saver = scanner.get_current_token();
    while(scanner.yylex() != "OR" && scanner.get_current_token() != "SEMICOLON"){
        if(scanner.get_current_token() == "$" || scanner.get_current_token() == "COLON")
            SyntaxError(MissingSemicolon);
        right_s.push_back(symbol());
        saver = scanner.get_current_token();
    }

    if(saver == "COLON" && scanner.get_current_token() == "OR")
        right_s.emplace_back("#");
    else if(saver == "OR" && scanner.get_current_token() == "OR")
        right_s.emplace_back("#");
    else if(saver == "OR" && scanner.get_current_token() == "SEMICOLON")
        right_s.emplace_back("#");
    parsed_grammar.insert(left_s, right_s);
}

string grammar_parser::symbol() {
    Token actual_token = scanner.get_current_token();
    if(actual_token == "VAR")
        return actual_token.get_lexeme();
    else if(actual_token == "APOS"){
        if(scanner.yylex() == "ANY"){
            string saver = scanner.get_current_token().get_lexeme();
            if(scanner.yylex() == "APOS")
                return saver;
            else
                SyntaxError(MissingApostrophe);
        }
        else
            SyntaxError(MissingSymbol);
    }
    SyntaxError(UnknownSymbol);
    return "ERROR";
}


