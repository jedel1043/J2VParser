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
        automata::NFA new_automaton;

        if (regex_scanner_.current_token() == TokenCodeRegex::AT_BOL) {
            regex_scanner_.GetNextToken();
            new_automaton = automata::NFA::CreateSimpleNFA('\n');
            Expr(new_automaton);
        } else {
            Expr(new_automaton);
        }

        if (regex_scanner_.current_token() == TokenCodeRegex::AT_EOL) {
            regex_scanner_.GetNextToken();
            std::set<char> labels;
            labels.insert('\n');
            labels.insert('\r');
            new_automaton.Concatenation(automata::NFA::CreateSimpleNFA(labels));
        }

        regex_scanner_.GetNextToken();
        std::string action;
        while (regex_scanner_.current_token() != TokenCodeRegex::EOS) {
            action += regex_scanner_.current_token().lexeme;
            regex_scanner_.GetNextToken();
        }
        new_automaton.AddAcceptingValue(action);
        regex_scanner_.GetNextToken();
        return new_automaton;
    }

    void RegexParser::Expr(automata::NFA &automaton) {
        automata::NFA new_automaton;
        CatExpr(automaton);
        while (regex_scanner_.current_token() == TokenCodeRegex::OR) {
            regex_scanner_.GetNextToken();
            CatExpr(new_automaton);
            automaton = automaton.Union(new_automaton);
        }
    }

    void RegexParser::CatExpr(automata::NFA &automaton) {
        automata::NFA new_automaton;

        if (isConcatenable(regex_scanner_.current_token()))
            Factor(automaton);
        while (isConcatenable(regex_scanner_.current_token())) {
            Factor(new_automaton);
            automaton = automaton.Concatenation(new_automaton);
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
                SyntaxError(error::MissingExpression, regex_scanner_.getSourceBuffer()->GetLineData());
                return false;
            case TokenCodeRegex::CCL_END:
                SyntaxError(error::MissingBracket, regex_scanner_.getSourceBuffer()->GetLineData());
                return false;
            case TokenCodeRegex::AT_BOL:
                SyntaxError(error::MissingStartExpression, regex_scanner_.getSourceBuffer()->GetLineData());
                return false;
            default:
                return true;
        }

    }

    void RegexParser::Factor(automata::NFA &automaton) {
        Term(automaton);
        if (regex_scanner_.current_token() == TokenCodeRegex::CLOSURE) {
            automaton = automaton.KleeneClosure();
            regex_scanner_.GetNextToken();
        } else if (regex_scanner_.current_token() == TokenCodeRegex::PLUS_CLOSURE) {
            automaton = automaton.PlusClosure();
            regex_scanner_.GetNextToken();
        } else if (regex_scanner_.current_token() == TokenCodeRegex::OPTIONAL) {
            automaton = automaton.Optional();
            regex_scanner_.GetNextToken();
        }
    }

    void RegexParser::Term(automata::NFA &automaton) {
        bool complement = false;
        if (regex_scanner_.current_token() == TokenCodeRegex::OPEN_PAREN) {
            regex_scanner_.GetNextToken();
            Expr(automaton);
            if (regex_scanner_.current_token() == TokenCodeRegex::CLOSE_PAREN)
                regex_scanner_.GetNextToken();
            else
                SyntaxError(error::MissingCloseParenthesis, regex_scanner_.getSourceBuffer()->GetLineData());
        } else {
            if (regex_scanner_.current_token() != TokenCodeRegex::ANY &&
                regex_scanner_.current_token() != TokenCodeRegex::CCL_START) {
                automaton = automata::NFA::CreateSimpleNFA(regex_scanner_.current_token().lexeme);
                regex_scanner_.GetNextToken();
            } else {
                if (regex_scanner_.current_token() == TokenCodeRegex::ANY) {
                    automaton = automata::NFA::CreateSimpleNFA(any_char_);
                } else {
                    std::set<char> cs;
                    if (regex_scanner_.GetNextToken() == TokenCodeRegex::AT_BOL) {
                        regex_scanner_.GetNextToken();
                        complement = true;
                    }
                    if (regex_scanner_.current_token() != TokenCodeRegex::CCL_END) {
                        Dash(cs);
                    } else {
                        for (char c = 0; c <= ' '; c++)
                            cs.insert(c);
                    }
                    if (complement) {
                        std::set<char> ccs;
                        for (int i = 0; i <= 127; i++) {
                            if (!cs.count((char) i))
                                ccs.insert((char) i);
                        }
                        automaton = automata::NFA::CreateSimpleNFA(ccs);
                    } else {
                        automaton = automata::NFA::CreateSimpleNFA(cs);
                    }
                }
                regex_scanner_.GetNextToken();
            }
        }
    }

    void RegexParser::Dash(std::set<char> &automaton) {
        char lastLexeme = 0;
        for (; regex_scanner_.current_token() != TokenCodeRegex::EOS &&
               regex_scanner_.current_token() != TokenCodeRegex::CCL_END; regex_scanner_.GetNextToken()) {
            if (regex_scanner_.current_token() != TokenCodeRegex::DASH) {
                lastLexeme = regex_scanner_.current_token().lexeme;
                automaton.insert(lastLexeme);
            } else {
                regex_scanner_.GetNextToken();
                for (; lastLexeme <= regex_scanner_.current_token().lexeme; ++lastLexeme)
                    automaton.insert(lastLexeme);
            }
        }
    }
} //namespace compiler::regex
