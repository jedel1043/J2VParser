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
            } else SyntaxError(error::MissingColon, scanner_.getInputFile()->GetLineData());
        } else if (scanner_.current_token() == "$");
        else SyntaxError(error::MissingRuleName, scanner_.getInputFile()->GetLineData());
    }

    void GrammarParser::RightSide(const std::string &left_s, GrammarArray &new_grammar) {
        std::vector<std::string> right_s;
        analyzers::Token saver = scanner_.current_token();
        while (scanner_.yylex() != "OR" && scanner_.current_token() != "SEMICOLON") {
            if (scanner_.current_token() == "$" || scanner_.current_token() == "COLON")
                SyntaxError(error::MissingSemicolon, scanner_.getInputFile()->GetLineData());
            right_s.push_back(Symbol());
            saver = scanner_.current_token();
        }

        if ((saver == "COLON" && scanner_.current_token() == "OR")
            || (saver == "OR" && scanner_.current_token() == "OR")
            || (saver == "OR" && scanner_.current_token() == "SEMICOLON"))
            right_s.emplace_back("#");
        new_grammar.InsertRule(left_s, right_s);
    }

    std::string GrammarParser::Symbol() {
        analyzers::Token actual_token = scanner_.current_token();
        if (actual_token == "VAR")
            return actual_token.lexeme;
        else if (actual_token == "APOS") {
            std::string saver;
            while (scanner_.yylex() != "APOS" && scanner_.current_token() != "$")
                saver += scanner_.current_token().lexeme;
            if (scanner_.current_token() == "$")
                SyntaxError(error::MissingApostrophe, scanner_.getInputFile()->GetLineData());
            if (saver.empty())
                SyntaxError(error::MissingSymbol, scanner_.getInputFile()->GetLineData());
            return saver;
        } else if (actual_token == "QM") {
            std::string saver;
            while (scanner_.yylex() != "QM" && scanner_.current_token() != "$")
                saver += scanner_.current_token().lexeme;
            if (scanner_.current_token() == "$")
                SyntaxError(error::MissingQuotationMark, scanner_.getInputFile()->GetLineData());
            if (saver.empty())
                SyntaxError(error::MissingSymbol, scanner_.getInputFile()->GetLineData());
            return saver;
        }
        SyntaxError(error::UnknownSymbol, scanner_.getInputFile()->GetLineData());
        return "ERROR";
    }
} // namespace compiler::grammar


