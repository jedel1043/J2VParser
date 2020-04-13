#include "J2VParser/analyzers/lexical_analyzer_s.h"

namespace J2VParser::analyzers {

    LexicalAnalyzerS::LexicalAnalyzerS(const std::string &str_input,
                                       automata::DFA automaton,
                                       bool skip_whitespace) :
            LexicalAnalyzer(std::move(automaton), skip_whitespace),
            str_input_(str_input) { str_pos_ = str_input_.begin(); }

    LexicalAnalyzerS::LexicalAnalyzerS(const std::string &str_input,
                                       io_buffer::TextSourceBuffer &regex_f,
                                       bool skip_whitespace) :
            LexicalAnalyzer(regex_f, skip_whitespace),
            str_input_(str_input) { str_pos_ = str_input_.begin(); }

    Token LexicalAnalyzerS::jvly() {
        if (eos())
            return current_token_ = {"$", "$"};

        char c = SkipWS();

        if (automaton_.at({automaton_.initial_state(), c}) == -1) {
            ++str_pos_;
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
            c = *++str_pos_;
        }
        SkipWS();
        return current_token_ = {token_name, lexeme};
    }

    bool LexicalAnalyzerS::eos() const {
        return str_pos_ == str_input_.end();
    }

    char LexicalAnalyzerS::SkipWS() {
        if (skip_whitespace_) {
            while (isspace(*str_pos_)) str_pos_++;
        }
        return *str_pos_;
    }

    std::tuple<std::string, int> LexicalAnalyzerS::getStrData() {
        return {str_input_, str_pos_ - str_input_.begin()};
    }

} //namespace compiler::analyzers