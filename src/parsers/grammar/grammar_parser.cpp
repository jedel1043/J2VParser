#include "J2VParser/parsers/grammar/grammar_parser.h"
#include "J2VParser/parsers/grammar/grammar_scanner.h"
#include "J2VParser/error.h"

namespace J2VParser::grammar {
    namespace {
        void Grammar(GrammarScanner &analyzer_, GrammarArray &new_grammar);

        void Rule(GrammarScanner &analyzer_, GrammarArray &new_grammar);

        void RightSide(GrammarScanner &analyzer_, const std::string &left_s, GrammarArray &new_grammar);

        std::string Symbol(GrammarScanner &analyzer_);

        void Grammar(GrammarScanner &analyzer_, GrammarArray &new_grammar) {
            while (analyzer_.current_token() != "$") {
                Rule(analyzer_, new_grammar);
            }
        }

        void Rule(GrammarScanner &analyzer_, GrammarArray &new_grammar) {
            std::string left_s;
            if (analyzer_.yylex() == "VAR") {
                left_s = analyzer_.current_token().lexeme;
                if (analyzer_.yylex() == "COLON") {
                    while (analyzer_.current_token() != "SEMICOLON")
                        RightSide(analyzer_, left_s, new_grammar);
                } else SyntaxError(error::MissingColon, analyzer_.getInputFile().GetLineData());
            } else if (analyzer_.current_token() == "$");
            else SyntaxError(error::MissingRuleName, analyzer_.getInputFile().GetLineData());
        }

        void RightSide(GrammarScanner &analyzer_, const std::string &left_s, GrammarArray &new_grammar) {
            std::vector<std::string> right_s;
            analyzers::Token saver = analyzer_.current_token();
            while (analyzer_.yylex() != "OR" && analyzer_.current_token() != "SEMICOLON") {
                if (analyzer_.current_token() == "$" || analyzer_.current_token() == "COLON")
                    SyntaxError(error::MissingSemicolon, analyzer_.getInputFile().GetLineData());
                right_s.push_back(Symbol(analyzer_));
                saver = analyzer_.current_token();
            }

            if ((saver == "COLON" && analyzer_.current_token() == "OR")
                || (saver == "OR" && analyzer_.current_token() == "OR")
                || (saver == "OR" && analyzer_.current_token() == "SEMICOLON"))
                right_s.emplace_back("#");
            new_grammar.InsertRule(left_s, right_s);
        }

        std::string Symbol(GrammarScanner &analyzer_) {
            analyzers::Token actual_token = analyzer_.current_token();
            if (actual_token == "VAR")
                return actual_token.lexeme;
            else if (actual_token == "APOS") {
                std::string saver;
                while (analyzer_.yylex() != "APOS" && analyzer_.current_token() != "$")
                    saver += analyzer_.current_token().lexeme;
                if (analyzer_.current_token() == "$")
                    SyntaxError(error::MissingApostrophe, analyzer_.getInputFile().GetLineData());
                if (saver.empty())
                    SyntaxError(error::MissingSymbol, analyzer_.getInputFile().GetLineData());
                return saver;
            } else if (actual_token == "QM") {
                std::string saver;
                while (analyzer_.yylex() != "QM" && analyzer_.current_token() != "$")
                    saver += analyzer_.current_token().lexeme;
                if (analyzer_.current_token() == "$")
                    SyntaxError(error::MissingQuotationMark, analyzer_.getInputFile().GetLineData());
                if (saver.empty())
                    SyntaxError(error::MissingSymbol, analyzer_.getInputFile().GetLineData());
                return saver;
            }
            SyntaxError(error::UnknownSymbol, analyzer_.getInputFile().GetLineData());
            return "ERROR";
        }
    } // anonymous namespace

    GrammarArray ParseGrammar(io_buffer::TextSourceBuffer &input_file) {
        GrammarScanner scanner(input_file);
        return ParseGrammar(scanner);
    }

    GrammarArray ParseGrammar(GrammarScanner &grammar_scanner) {
        GrammarArray parsed_grammar;
        Grammar(grammar_scanner, parsed_grammar);
        return parsed_grammar;
    }
} // namespace compiler::grammar


