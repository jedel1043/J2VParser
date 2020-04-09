#ifndef J2VPARSER_LEXICAL_ANALYZER_H
#define J2VPARSER_LEXICAL_ANALYZER_H

#include <string>
#include <utility>

#include "J2VParser/analyzers/automata/nfa.h"
#include "J2VParser/analyzers/automata/dfa.h"
#include "J2VParser/analyzers/regex/regex_parser.h"
#include "J2VParser/analyzers/regex/regex_scanner.h"
#include "J2VParser/buffer.h"

namespace J2VParser::analyzers {

    struct Token {
        std::string token_name;
        std::string lexeme;

        bool operator==(const std::string &token) const { return token_name == token; }

        bool operator!=(const std::string &token) const { return token_name != token; }
    };

    class LexicalAnalyzer {
    protected:
        automata::DFA automaton_;
        bool skip_whitespace_;
        Token current_token_;
    public:
        LexicalAnalyzer(automata::DFA automaton, bool skip_whitespace) :
                automaton_(std::move(automaton)),
                skip_whitespace_(skip_whitespace) {};

        LexicalAnalyzer(io_buffer::TextSourceBuffer &regex_f, bool skip_whitespace) :
                automaton_(regex::ParseRegex(regex_f)),
                skip_whitespace_(skip_whitespace) {};

        virtual Token jvly() = 0;

        [[nodiscard]] virtual bool eos() const = 0;

        virtual char SkipWS() = 0;

        Token current_token() { return current_token_; };

        virtual ~LexicalAnalyzer() = default;
    };
} //namespace compiler::analyzers


#endif //J2VPARSER_LEXICAL_ANALYZER_H
