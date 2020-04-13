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

    Token LexicalAnalyzerF::jvly() {
        if (eos())
            return current_token_ = {"$", "$"};

        char c = SkipWS();

        if (automaton_.at({automaton_.initial_state(), c}) == -1) {
            input_file_.FetchChar();
            return current_token_ = {"ANY", {c}};
        }

        automata::Automaton::State actual_state = automaton_.initial_state();
        std::string token_name;
        std::string lexeme;
        while (true) {
            actual_state = automaton_.at({actual_state, c});
            if (actual_state == -1)
                break;
            lexeme += c;
            if (automaton_.accepting_states().count(actual_state))
                token_name = automaton_.tokens().at(actual_state);
            c = input_file_.FetchChar();
        }
        SkipWS();
        return current_token_ = {token_name, lexeme};
    }

    bool LexicalAnalyzerF::eos() const {
        return input_file_.eof();
    }

    char LexicalAnalyzerF::SkipWS() {
        if (skip_whitespace_) {
            while (std::isspace(input_file_.GetChar()))
                input_file_.FetchChar();
        }
        return input_file_.GetChar();
    }

    io_buffer::TextSourceBuffer &LexicalAnalyzerF::getInputFile() const {
        return input_file_;
    }
} //namespace compiler::analyzers

