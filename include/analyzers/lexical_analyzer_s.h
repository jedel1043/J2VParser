#ifndef COMPILER_LEXICAL_ANALYZER_S_H
#define COMPILER_LEXICAL_ANALYZER_S_H

#include <string>
#include <utility>

#include "analyzers/lexical_analyzer.h"
#include "automata/dfa.h"

namespace compiler::analyzers {
    class LexicalAnalyzerS : public LexicalAnalyzer {
    private:
        std::string str_input_;
        std::string::iterator str_pos_;
    public:
        LexicalAnalyzerS(std::string strInput, automata::DFA automata, bool skip_whitespace = true);

        explicit LexicalAnalyzerS(automata::DFA automata, bool skip_whitespace = true) :
                LexicalAnalyzer(std::move(automata), skip_whitespace) {};

        void set_str_input(const std::string &new_str);

        Token yylex() override;

        bool isInEnd() override;

        char SkipWS() override;

        std::tuple<std::string, int> getStrData();
    };
} //namespace compiler::analyzers


#endif //COMPILER_LEXICAL_ANALYZER_S_H
