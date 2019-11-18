#include "parsers/grammar_utils/grammar_parser.h"

namespace compiler::grammar {

    void GrammarParser::Grammar(GrammarArray &new_grammar) {
        while (scanner_.current_token() != "$") {
            Rule(new_grammar);
        }
    }

    void GrammarParser::Rule(GrammarArray &new_grammar) {
        std::string left_s;
        if (scanner_.yylex() == "VAR") {
            left_s = scanner_.current_token().lexeme;
            if (scanner_.yylex() == "COLON") {
                while (scanner_.current_token() != "SEMICOLON")
                    RightSide(left_s, new_grammar);
            } else SyntaxError(error::MissingColon);
        } else if (scanner_.current_token() == "$");
        else SyntaxError(error::MissingRuleName);
    }

    void GrammarParser::RightSide(const std::string &left_s, GrammarArray &new_grammar) {
        std::vector<std::string> right_s;
        analyzers::Token saver = scanner_.current_token();
        while (scanner_.yylex() != "OR" && scanner_.current_token() != "SEMICOLON") {
            if (scanner_.current_token() == "$" || scanner_.current_token() == "COLON")
                SyntaxError(error::MissingSemicolon);
            right_s.push_back(Symbol());
            saver = scanner_.current_token();
        }

        if (saver == "COLON" && scanner_.current_token() == "OR")
            right_s.emplace_back("#");
        else if (saver == "OR" && scanner_.current_token() == "OR")
            right_s.emplace_back("#");
        else if (saver == "OR" && scanner_.current_token() == "SEMICOLON")
            right_s.emplace_back("#");
        new_grammar.InsertRule(left_s, right_s);
    }

    std::string GrammarParser::Symbol() {
        analyzers::Token actual_token = scanner_.current_token();
        if (actual_token == "VAR")
            return actual_token.lexeme;
        else if (actual_token == "APOS") {
            if (scanner_.yylex() == "ANY") {
                std::string saver = scanner_.current_token().lexeme;
                if (scanner_.yylex() == "APOS")
                    return saver;
                else
                    SyntaxError(error::MissingApostrophe);
            } else
                SyntaxError(error::MissingSymbol);
        }
        SyntaxError(error::UnknownSymbol);
        return "ERROR";
    }
} // namespace compiler::grammar


