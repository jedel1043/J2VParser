#include "J2VParser/analyzers/lexical_analyzer_s.h"

namespace compiler::analyzers {

    LexicalAnalyzerS::LexicalAnalyzerS(std::string strInput, automata::DFA automaton, bool skip_whitespace) :
            LexicalAnalyzer(std::move(automaton), skip_whitespace),
            str_input_(std::move(strInput)) {
        str_pos_ = str_input_.begin();
    }

    Token LexicalAnalyzerS::yylex() {
        SkipWS();
        if (isInEnd())
            return current_token_ = {"$", "$"};

        if (automaton_.Compute(automaton_.initial_state(), *str_pos_) == -1)
            return current_token_ = {"ANY", std::string(1, *str_pos_++)};

        std::string token_name;
        std::string lexeme;

        int actual_state = automaton_.initial_state();

        while (actual_state != -1) {
            if (isInEnd())
                break;
            SkipWS();

            lexeme += *str_pos_;
            actual_state = automaton_.Compute(actual_state, *str_pos_);
            if (automaton_.accepting_states().count(actual_state))
                token_name = automaton_.tokens().at(actual_state);
            str_pos_++;
        }
        if (actual_state == -1) {
            str_pos_--;
            lexeme.pop_back();
        }
        return current_token_ = {token_name, lexeme};
    }

    bool LexicalAnalyzerS::isInEnd() {
        return str_pos_ == str_input_.end();
    }

    void LexicalAnalyzerS::set_str_input(const std::string &new_str) {
        str_input_ = new_str;
        str_pos_ = str_input_.begin();
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