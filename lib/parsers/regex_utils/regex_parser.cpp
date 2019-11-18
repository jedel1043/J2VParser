#include "parsers/regex_utils/regex_parser.h"

#include <set>
#include <string>

namespace compiler::regex {

    void RegexParser::Machine() {
        regex_scanner_.GetNextToken();
        lexical_nfa_.push_back(Rule());
        while (regex_scanner_.current_token() != TokenCodeRegex::END_OF_INPUT && regex_scanner_.current_token() != TokenCodeRegex::EOS) {
            lexical_nfa_.push_back(Rule());
        }
    }

    automata::NFA RegexParser::Rule() {
        automata::NFA new_automata;

        if (regex_scanner_.current_token() == TokenCodeRegex::AT_BOL) {
            regex_scanner_.GetNextToken();
            new_automata = automata::NFA::CreateSimpleNFA('\n');
            Expr(new_automata);
        } else {
            Expr(new_automata);
        }

        if (regex_scanner_.current_token() == TokenCodeRegex::AT_EOL) {
            regex_scanner_.GetNextToken();
            std::set<char> labels;
            labels.insert('\n');
            labels.insert('\r');
            new_automata.Concatenation(automata::NFA::CreateSimpleNFA(labels));
        }

        regex_scanner_.GetNextToken();
        std::string action;
        while (regex_scanner_.current_token() != TokenCodeRegex::EOS) {
            action += regex_scanner_.current_token().lexeme;
            regex_scanner_.GetNextToken();
        }
        new_automata.AddAcceptingValue(action);
        regex_scanner_.GetNextToken();
        return new_automata;
    }

    void RegexParser::Expr(automata::NFA &automata) {
        automata::NFA new_automata;
        CatExpr(automata);
        while (regex_scanner_.current_token() == TokenCodeRegex::OR) {
            regex_scanner_.GetNextToken();
            CatExpr(new_automata);
            automata = automata.Union(new_automata);
        }
    }

    void RegexParser::CatExpr(automata::NFA &automata) {
        automata::NFA new_automata;

        if (isConcatenable(regex_scanner_.current_token()))
            Factor(automata);
        while (isConcatenable(regex_scanner_.current_token())) {
            Factor(new_automata);
            automata = automata.Concatenation(new_automata);
        }
    }

    bool RegexParser::isConcatenable(RegexToken input_char) {
        switch (input_char.token) {
            case TokenCodeRegex::CLOSE_PAREN:
            case TokenCodeRegex::AT_EOL:
            case TokenCodeRegex::OR:
            case TokenCodeRegex::EOS:
                return false;
            case TokenCodeRegex::CLOSURE:
            case TokenCodeRegex::PLUS_CLOSURE:
            case TokenCodeRegex::OPTIONAL:
                SyntaxError(error::MissingExpression);
                return false;
            case TokenCodeRegex::CCL_END:
                SyntaxError(error::MissingBracket);
                return false;
            case TokenCodeRegex::AT_BOL:
                SyntaxError(error::MissingStartExpression);
                return false;
            default:
                return true;
        }

    }

    void RegexParser::Factor(automata::NFA &automata) {
        Term(automata);
        if (regex_scanner_.current_token() == TokenCodeRegex::CLOSURE) {
            automata = automata.KleeneClosure();
            regex_scanner_.GetNextToken();
        } else if (regex_scanner_.current_token() == TokenCodeRegex::PLUS_CLOSURE) {
            automata = automata.PlusClosure();
            regex_scanner_.GetNextToken();
        } else if (regex_scanner_.current_token() == TokenCodeRegex::OPTIONAL) {
            automata = automata.Optional();
            regex_scanner_.GetNextToken();
        }
    }

    void RegexParser::Term(automata::NFA &automata) {
        bool complement = false;
        if (regex_scanner_.current_token() == TokenCodeRegex::OPEN_PAREN) {
            regex_scanner_.GetNextToken();
            Expr(automata);
            if (regex_scanner_.current_token() == TokenCodeRegex::CLOSE_PAREN)
                regex_scanner_.GetNextToken();
            else
                SyntaxError(error::MissingCloseParenthesis);
        } else {
            if (regex_scanner_.current_token() != TokenCodeRegex::ANY && regex_scanner_.current_token() != TokenCodeRegex::CCL_START) {
                automata = automata::NFA::CreateSimpleNFA(regex_scanner_.current_token().lexeme);
                regex_scanner_.GetNextToken();
            } else {
                if (regex_scanner_.current_token() == TokenCodeRegex::ANY) {
                    automata = automata::NFA::CreateSimpleNFA(any_char_);
                } else {
                    std::set<char> cs;
                    if (regex_scanner_.GetNextToken() == TokenCodeRegex::AT_BOL) {
                        regex_scanner_.GetNextToken();
                        complement = true;
                    }
                    if (regex_scanner_.current_token() != TokenCodeRegex::CCL_END) {
                        Dash(cs);
                    } else {
                        for (int c = 0; c <= ' '; c++)
                            cs.insert(c);
                    }
                    if (complement) {
                        std::set<char> ccs;
                        for (int i = 0; i <= 127; i++) {
                            if (!cs.count((char) i))
                                ccs.insert((char) i);
                        }
                        automata = automata::NFA::CreateSimpleNFA(ccs);
                    } else {
                        automata = automata::NFA::CreateSimpleNFA(cs);
                    }
                }
                regex_scanner_.GetNextToken();
            }
        }
    }

    void RegexParser::Dash(std::set<char> &automata) {
        char lastLexeme = 0;
        for (; regex_scanner_.current_token() != TokenCodeRegex::EOS &&
               regex_scanner_.current_token() != TokenCodeRegex::CCL_END; regex_scanner_.GetNextToken()) {
            if (regex_scanner_.current_token() != TokenCodeRegex::DASH) {
                lastLexeme = regex_scanner_.current_token().lexeme;
                automata.insert(lastLexeme);
            } else {
                regex_scanner_.GetNextToken();
                for (; lastLexeme <= regex_scanner_.current_token().lexeme; ++lastLexeme)
                    automata.insert(lastLexeme);
            }
        }
    }
} //namespace compiler::regex
