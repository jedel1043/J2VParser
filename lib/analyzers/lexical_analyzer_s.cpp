#include "analyzers/lexical_analyzer_s.h"

#include <utility>

namespace compiler::analyzers {

    LexicalAnalyzerS::LexicalAnalyzerS(std::string strInput, automata::DFA automata, bool skip_whitespace) :
            LexicalAnalyzer(std::move(automata), skip_whitespace),
            str_input_(std::move(strInput)) {
        str_pos_ = str_input_.begin();
    }

    Token LexicalAnalyzerS::yylex() {
        if (isInEnd())
            return current_token_ = {"$", "$"};
        std::string token_name;
        std::string lexeme;
        int actual_state = automata_.initial_state();

        while (actual_state != -1) {
            if (isInEnd())
                break;
            SkipWS();

            lexeme += *str_pos_;
            actual_state = automata_.Compute(actual_state, *str_pos_);
            if (automata_.accepting_states().count(actual_state))
                token_name = automata_.tokens().at(actual_state);
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
} //namespace compiler::analyzers