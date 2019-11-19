#include "analyzers/lexical_analyzer_f.h"

#include <utility>

namespace compiler::analyzers {

    LexicalAnalyzerF::LexicalAnalyzerF(io_buffer::TextSourceBuffer *inputFile, automata::DFA automata, bool skip_whitespace) :
            LexicalAnalyzer(std::move(automata), skip_whitespace), input_file_(inputFile) {}

    Token LexicalAnalyzerF::yylex() {
        char c = SkipWS();
        if (isInEnd())
            return current_token_ = {"$", "$"};

        if(automata_.Compute(automata_.initial_state(), c) == -1) {
            input_file_->FetchChar();
            return current_token_ = {"ANY", std::string(1, c)};
        }

        int actual_state = automata_.initial_state();
        std::string token_name;
        std::string lexeme;
        while (actual_state != -1) {
            if (isInEnd() || isEOS(c))
                break;
            lexeme += c;
            actual_state = automata_.Compute(actual_state, c);
            if (automata_.accepting_states().count(actual_state))
                token_name = automata_.tokens().at(actual_state);
            c = input_file_->FetchChar();
        }
        if (isEOS(c) && actual_state != -1)
            SkipWS();
        else if (!isInEnd()) {
            input_file_->PutBackChar();
            lexeme.pop_back();
        }
        return current_token_ = {token_name, lexeme};
    }

    bool LexicalAnalyzerF::isInEnd() {
        return input_file_->GetChar() == io_buffer::EOF_char;
    }

    char LexicalAnalyzerF::SkipWS() {
        if (skip_whitespace_) {
            while (isEOS(input_file_->GetChar()))
                input_file_->FetchChar();
        }
        return input_file_->GetChar();
    }

    bool LexicalAnalyzerF::isEOS(char c) {
        return isspace(c) || c == '\0';
    }
} //namespace compiler::analyzers

