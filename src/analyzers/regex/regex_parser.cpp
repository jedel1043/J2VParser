#include "J2VParser/analyzers/regex/regex_parser.h"
#include "J2VParser/error.h"

#include <string>

namespace J2VParser::regex {

    namespace {
        std::set<char> charset;

        void Machine(RegexScanner &regex_scanner, std::vector<automata::NFA> &lexical_nfa);

        automata::NFA Rule(RegexScanner &regex_scanner);

        void Expr(RegexScanner &regex_scanner, automata::NFA &automaton);

        void CatExpr(RegexScanner &regex_scanner, automata::NFA &automaton);

        void Factor(RegexScanner &regex_scanner, automata::NFA &automaton);

        void Term(RegexScanner &regex_scanner, automata::NFA &automaton);

        void Dash(RegexScanner &regex_scanner, std::set<char> &set_char);

        bool isConcatenable(RegexScanner &regex_scanner, RegexToken input_char);

        void Machine(RegexScanner &regex_scanner, std::vector<automata::NFA> &lexical_nfa) {
            regex_scanner.GetNextToken();
            lexical_nfa.push_back(Rule(regex_scanner));
            while (regex_scanner.current_token() != TokenCodeRegex::END_OF_INPUT &&
                   regex_scanner.current_token() != TokenCodeRegex::EOS) {
                lexical_nfa.push_back(Rule(regex_scanner));
            }
        }

        automata::NFA Rule(RegexScanner &regex_scanner) {
            automata::NFA new_automaton;

            if (regex_scanner.current_token() == TokenCodeRegex::AT_BOL) {
                regex_scanner.GetNextToken();
                new_automaton = automata::NFA('\n');
                Expr(regex_scanner, new_automaton);
            } else {
                Expr(regex_scanner, new_automaton);
            }

            if (regex_scanner.current_token() == TokenCodeRegex::AT_EOL) {
                regex_scanner.GetNextToken();
                std::set<char> labels;
                labels.insert('\n');
                labels.insert('\r');
                new_automaton = new_automaton.Concat(automata::NFA(labels));
            }

            regex_scanner.GetNextToken();
            std::string action;
            while (regex_scanner.current_token() != TokenCodeRegex::EOS) {
                action += regex_scanner.current_token().lexeme;
                regex_scanner.GetNextToken();
            }
            new_automaton.AddAcceptingValue(action);
            regex_scanner.GetNextToken();
            return new_automaton;
        }

        void Expr(RegexScanner &regex_scanner, automata::NFA &automaton) {
            automata::NFA new_automaton;
            CatExpr(regex_scanner, automaton);
            while (regex_scanner.current_token() == TokenCodeRegex::OR) {
                regex_scanner.GetNextToken();
                CatExpr(regex_scanner, new_automaton);
                automaton = automaton.Union(new_automaton);
            }
        }

        void CatExpr(RegexScanner &regex_scanner, automata::NFA &automaton) {
            automata::NFA new_automaton;

            if (isConcatenable(regex_scanner, regex_scanner.current_token()))
                Factor(regex_scanner, automaton);
            while (isConcatenable(regex_scanner, regex_scanner.current_token())) {
                Factor(regex_scanner, new_automaton);
                automaton = automaton.Concat(new_automaton);
            }
        }

        void Factor(RegexScanner &regex_scanner, automata::NFA &automaton) {
            Term(regex_scanner, automaton);
            if (regex_scanner.current_token() == TokenCodeRegex::CLOSURE) {
                automaton = automaton.KleeneClose();
                regex_scanner.GetNextToken();
            } else if (regex_scanner.current_token() == TokenCodeRegex::PLUS_CLOSURE) {
                automaton = automaton.PlusClose();
                regex_scanner.GetNextToken();
            } else if (regex_scanner.current_token() == TokenCodeRegex::OPTIONAL) {
                automaton = automaton.Optionalize();
                regex_scanner.GetNextToken();
            }
        }

        void Term(RegexScanner &regex_scanner, automata::NFA &automaton) {
            bool complement = false;
            if (regex_scanner.current_token() == TokenCodeRegex::OPEN_PAREN) {
                regex_scanner.GetNextToken();
                Expr(regex_scanner, automaton);
                if (regex_scanner.current_token() == TokenCodeRegex::CLOSE_PAREN)
                    regex_scanner.GetNextToken();
                else
                    SyntaxError(error::MissingCloseParenthesis, regex_scanner.getSourceBuffer().GetBufferStatus());
            } else {
                if (regex_scanner.current_token() != TokenCodeRegex::ANY &&
                    regex_scanner.current_token() != TokenCodeRegex::CCL_START) {
                    automaton = automata::NFA(regex_scanner.current_token().lexeme);
                    regex_scanner.GetNextToken();
                } else {
                    if (regex_scanner.current_token() == TokenCodeRegex::ANY) {
                        automaton = automata::NFA(charset);
                    } else {
                        std::set<char> cs;
                        if (regex_scanner.GetNextToken() == TokenCodeRegex::AT_BOL) {
                            regex_scanner.GetNextToken();
                            complement = true;
                        }
                        if (regex_scanner.current_token() != TokenCodeRegex::CCL_END) {
                            Dash(regex_scanner, cs);
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
                            automaton = automata::NFA(ccs);
                        } else {
                            automaton = automata::NFA(cs);
                        }
                    }
                    regex_scanner.GetNextToken();
                }
            }
        }

        void Dash(RegexScanner &regex_scanner, std::set<char> &set_char) {
            char lastLexeme = 0;
            for (; regex_scanner.current_token() != TokenCodeRegex::EOS &&
                   regex_scanner.current_token() != TokenCodeRegex::CCL_END; regex_scanner.GetNextToken()) {
                if (regex_scanner.current_token() != TokenCodeRegex::DASH) {
                    lastLexeme = regex_scanner.current_token().lexeme;
                    set_char.insert(lastLexeme);
                } else {
                    regex_scanner.GetNextToken();
                    for (; lastLexeme <= regex_scanner.current_token().lexeme; ++lastLexeme)
                        set_char.insert(lastLexeme);
                }
            }
        }

        bool isConcatenable(RegexScanner &regex_scanner, RegexToken input_char) {
            switch (input_char.token) {
                case TokenCodeRegex::CLOSE_PAREN:
                case TokenCodeRegex::AT_EOL:
                case TokenCodeRegex::OR:
                case TokenCodeRegex::EOS:
                    return false;
                case TokenCodeRegex::CLOSURE:
                case TokenCodeRegex::PLUS_CLOSURE:
                case TokenCodeRegex::OPTIONAL:
                    SyntaxError(error::MissingExpression, regex_scanner.getSourceBuffer().GetBufferStatus());
                    return false;
                case TokenCodeRegex::CCL_END:
                    SyntaxError(error::MissingBracket, regex_scanner.getSourceBuffer().GetBufferStatus());
                    return false;
                case TokenCodeRegex::AT_BOL:
                    SyntaxError(error::MissingStartExpression, regex_scanner.getSourceBuffer().GetBufferStatus());
                    return false;
                default:
                    return true;
            }

        }
    } // anonymous namespace

    automata::DFA ParseRegex(io_buffer::TextSourceBuffer &source_buffer) {
        RegexScanner scanner(source_buffer);
        return ParseRegex(scanner);
    }

    automata::DFA ParseRegex(RegexScanner &regex_scanner) {
        if (charset.empty())
            for (char i = ' '; i <= '}'; i++) charset.insert(i);
        std::vector<automata::NFA> lexical_nfa;
        Machine(regex_scanner, lexical_nfa);
        return automata::NFA::CalculateLexicalUnion(lexical_nfa).ToDFA().Minimize();
    }

} //namespace compiler::regex
