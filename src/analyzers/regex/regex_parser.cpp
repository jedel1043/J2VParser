#include "J2VParser/analyzers/regex/regex_parser.h"
#include "J2VParser/error.h"

#include <string>

namespace J2VParser::regex {

    namespace {

        void Regex(RegexScanner &regex_scanner, std::vector<automata::NFA> &lexical_nfa);

        std::string Var(RegexScanner &regex_scanner);

        bool Expr(RegexScanner &regex_scanner, automata::NFA &input);

        bool Term(RegexScanner &regex_scanner, automata::NFA &input);

        bool Anchor(RegexScanner &regex_scanner, automata::NFA &input);

        bool BasicExp(RegexScanner &regex_scanner, automata::NFA &input);

        bool ElemExp(RegexScanner &regex_scanner, automata::NFA &input);

        bool Integer(RegexScanner &regex_scanner, std::string &input);

        bool CharSet(RegexScanner &regex_scanner, automata::NFA &input);

        bool SetItem(RegexScanner &regex_scanner, automata::NFA &input);


        void Regex(RegexScanner &regex_scanner, std::vector<automata::NFA> &lexical_nfa) {
            regex_scanner.FetchToken();
            while (regex_scanner.current_token() != TokenCodeRegex::EOS) {
                automata::NFA result;
                regex_scanner.SkipWhiteSpace();
                std::string token = Var(regex_scanner);
                regex_scanner.SkipWhiteSpace();
                if (Expr(regex_scanner, result)) {
                    result.AddAcceptingValue(token);
                    lexical_nfa.emplace_back(result);
                } else
                    error::SyntaxError(error::AbortCode::MissingExpression,
                                       regex_scanner.getSourceBuffer().GetBufferStatus());
                regex_scanner.SkipWhiteSpace();
            }
        }

        bool Integer(RegexScanner &regex_scanner, std::string &input) {
            if (std::isdigit(regex_scanner.current_token().lexeme)) {
                input += regex_scanner.current_token().lexeme;
                while (std::isdigit(regex_scanner.FetchToken().lexeme))
                    input += regex_scanner.current_token().lexeme;
                return true;
            }
            return false;
        }

        std::string Var(RegexScanner &regex_scanner) {
            std::string var_name;
            char c = regex_scanner.current_token().lexeme;
            if (std::isalpha(c) || c == '_') {
                var_name += c;
                while (isalnum(c = regex_scanner.FetchToken().lexeme) || c == '-' || c == '_')
                    var_name += c;
            } else
                error::SyntaxError(error::InvalidVarName,
                                   regex_scanner.getSourceBuffer().GetBufferStatus());
            return var_name;
        }

        bool Expr(RegexScanner &regex_scanner, automata::NFA &input) {
            if (Anchor(regex_scanner, input)) {
                automata::NFA value;
                while (regex_scanner.current_token() == TokenCodeRegex::UNION) {
                    regex_scanner.FetchToken();
                    if (Anchor(regex_scanner, value))
                        input = input.Union(value);
                    else
                        error::SyntaxError(error::AbortCode::MissingExpression,
                                           regex_scanner.getSourceBuffer().GetBufferStatus());
                }
                return true;
            }
            return false;
        }

        bool Anchor(RegexScanner &regex_scanner, automata::NFA &input) {
            bool begin_line = false;
            if (regex_scanner.current_token() == TokenCodeRegex::BEGINLINE) {
                regex_scanner.FetchToken();
                begin_line = true;
            }
            if (Term(regex_scanner, input)) {
                if (begin_line)
                    input = automata::NFA('\n').Concat(input);
                if (regex_scanner.current_token() == TokenCodeRegex::ENDLINE) {
                    regex_scanner.FetchToken();
                    input = input.Concat(automata::NFA('\n'));
                }
                return true;
            }
            return false;
        }

        bool Term(RegexScanner &regex_scanner, automata::NFA &input) {
            if (BasicExp(regex_scanner, input)) {
                automata::NFA result;
                while (BasicExp(regex_scanner, result))
                    input = input.Concat(result);
                return true;
            }
            return false;
        }

        bool BasicExp(RegexScanner &regex_scanner, automata::NFA &input) {
            if (ElemExp(regex_scanner, input)) {
                regex_scanner.SkipWhiteSpace();
                if (regex_scanner.current_token() == TokenCodeRegex::OPT) {
                    input = input.Optionalize();
                    regex_scanner.FetchToken();
                }
                if (regex_scanner.current_token() == TokenCodeRegex::CLOSURE) {
                    input = input.KleeneClose();
                    regex_scanner.FetchToken();
                } else if (regex_scanner.current_token() == TokenCodeRegex::PLCLOSURE) {
                    input = input.PlusClose();
                    regex_scanner.FetchToken();
                } else if (regex_scanner.current_token() == TokenCodeRegex::BEGINCOUNT) {
                    regex_scanner.FetchToken();
                    std::string begin, end;
                    if (Integer(regex_scanner, begin)) {
                        if (regex_scanner.current_token().lexeme == ',') {
                            regex_scanner.FetchToken();
                            if (Integer(regex_scanner, end)) {
                                if (regex_scanner.current_token() == TokenCodeRegex::ENDCOUNT) {
                                    return true;
                                }
                            }
                        }
                    }
                }
                return true;
            }
            return false;
        }

        bool ElemExp(RegexScanner &regex_scanner, automata::NFA &input) {
            if (regex_scanner.current_token() == TokenCodeRegex::BEGINGR) {
                regex_scanner.FetchToken();
                if (Expr(regex_scanner, input)) {
                    if (regex_scanner.current_token() == TokenCodeRegex::ENDGR) {
                        regex_scanner.FetchToken();
                        return true;
                    }
                    error::SyntaxError(error::AbortCode::MissingCloseParenthesis,
                                       regex_scanner.getSourceBuffer().GetBufferStatus());
                }
                error::SyntaxError(error::AbortCode::MissingExpression,
                                   regex_scanner.getSourceBuffer().GetBufferStatus());
            } else if (regex_scanner.current_token() == TokenCodeRegex::ANY) {
                regex_scanner.FetchToken();
                input = automata::NFA(' ', '~').Union(automata::NFA('\t'));
                return true;
            } else if (regex_scanner.current_token() == TokenCodeRegex::C) {
                input = automata::NFA(regex_scanner.current_token().lexeme);
                regex_scanner.FetchToken();
                return true;
            } else if (regex_scanner.current_token() == TokenCodeRegex::BS) {
                input = automata::NFA(regex_scanner.FetchToken().lexeme);
                regex_scanner.FetchToken();
                return true;
            } else if (CharSet(regex_scanner, input))
                return true;
            return false;
        }

        bool CharSet(RegexScanner &regex_scanner, automata::NFA &input) {
            auto regex_state_saver = regex_scanner.getSourceBuffer().GetBufferStatus();
            if (regex_scanner.current_token() == TokenCodeRegex::BEGINCCL) {
                bool negset = false;
                if (regex_scanner.FetchToken() == TokenCodeRegex::BEGINLINE) {
                    regex_scanner.FetchToken();
                    negset = true;
                }
                if (SetItem(regex_scanner, input)) {
                    automata::NFA saver;
                    while (SetItem(regex_scanner, saver))
                        input = input.Union(saver);

                    if (regex_scanner.current_token() == TokenCodeRegex::ENDCCL) {
                        regex_scanner.FetchToken();
                        return true;
                    }
                    error::SyntaxError(error::AbortCode::MissingBracket,
                                       regex_state_saver);
                }
                error::SyntaxError(error::AbortCode::MissingExpression,
                                   regex_state_saver);
            }
            return false;
        }

        bool SetItem(RegexScanner &regex_scanner, automata::NFA &input) {
            if (regex_scanner.current_token() == TokenCodeRegex::ENDCCL
                || regex_scanner.current_token() == TokenCodeRegex::EOS) {
                return false;
            }
            if (regex_scanner.current_token() == TokenCodeRegex::BS) {
                if (regex_scanner.FetchToken() != TokenCodeRegex::EOS) {
                    input = automata::NFA(regex_scanner.current_token().lexeme);
                    regex_scanner.FetchToken();
                    return true;
                }
                return false;
            }
            char c = regex_scanner.current_token().lexeme;
            if (regex_scanner.FetchToken().lexeme == '-') {
                if (regex_scanner.FetchToken() != TokenCodeRegex::ENDCCL
                    && regex_scanner.current_token() != TokenCodeRegex::EOS) {
                    input = automata::NFA(c, regex_scanner.current_token().lexeme);
                    regex_scanner.FetchToken();
                } else
                    input = automata::NFA(c).Union(automata::NFA('-'));
            } else
                input = automata::NFA(c);
            return true;
        }

    }
    // namespace

    automata::DFA ParseRegex(io_buffer::TextSourceBuffer &source_buffer) {
        RegexScanner scanner(source_buffer);
        return ParseRegex(scanner);
    }

    automata::DFA ParseRegex(RegexScanner &regex_scanner) {
        std::vector<automata::NFA> lexical_nfa;
        Regex(regex_scanner, lexical_nfa);
        return automata::NFA::CalculateLexicalUnion(lexical_nfa).ToDFA().Minimize();
    }

} //namespace compiler::regex
