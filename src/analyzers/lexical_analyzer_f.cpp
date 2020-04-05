#include "J2VParser/analyzers/lexical_analyzer_f.h"

namespace J2VParser::analyzers {

    LexicalAnalyzerF::LexicalAnalyzerF(io_buffer::TextSourceBuffer &input_f,
                                       automata::DFA automaton,
                                       bool skip_whitespace) :
            LexicalAnalyzer(std::move(automaton), skip_whitespace), input_file_(input_f) {}

    LexicalAnalyzerF::LexicalAnalyzerF(io_buffer::TextSourceBuffer &input_f,
                                       io_buffer::TextSourceBuffer &regex_f,
                                       bool skip_whitespace) :
            LexicalAnalyzer(regex_f, skip_whitespace), input_file_(input_f) {}

    Token LexicalAnalyzerF::yylex() {
        char c = SkipWS();
        if (isInEnd())
            return current_token_ = {"$", "$"};

        if (automaton_.Compute(automaton_.initial_state(), c) == -1) {
            input_file_.FetchChar();
            return current_token_ = {"ANY", std::string(1, c)};
        }

        int actual_state = automaton_.initial_state();
        std::string token_name;
        std::string lexeme;
        while (actual_state != -1) {
            if (isInEnd() || isEOS(c))
                break;
            lexeme += c;
            actual_state = automaton_.Compute(actual_state, c);
            if (automaton_.accepting_states().count(actual_state))
                token_name = automaton_.tokens().at(actual_state);
            c = input_file_.FetchChar();
        }
        if (isEOS(c) && actual_state != -1)
            SkipWS();
        else if (!isInEnd()) {
            input_file_.PutBackChar();
            lexeme.pop_back();
        }
        return current_token_ = {token_name, lexeme};
    }

    bool LexicalAnalyzerF::isInEnd() const {
        return input_file_.GetChar() == io_buffer::EOF_char;
    }

    char LexicalAnalyzerF::SkipWS() {
        if (skip_whitespace_) {
            while (isEOS(input_file_.GetChar()))
                input_file_.FetchChar();
        }
        return input_file_.GetChar();
    }

    bool LexicalAnalyzerF::isEOS(char c) {
        return isspace(c) || c == '\0';
    }

    io_buffer::TextSourceBuffer &LexicalAnalyzerF::getInputFile() const {
        return input_file_;
    }
} //namespace compiler::analyzers

