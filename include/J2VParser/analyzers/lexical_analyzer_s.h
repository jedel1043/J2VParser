#ifndef J2VP_LEXICAL_ANALYZER_S_H
#define J2VP_LEXICAL_ANALYZER_S_H

#include <string>
#include <utility>

#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/automata/dfa.h"

namespace compiler::analyzers {
    class LexicalAnalyzerS : public LexicalAnalyzer {
    private:
        std::string str_input_;
        std::string::iterator str_pos_;
    public:
        LexicalAnalyzerS(std::string strInput, automata::DFA automaton, bool skip_whitespace = true);

        explicit LexicalAnalyzerS(automata::DFA automaton, bool skip_whitespace = true) :
                LexicalAnalyzer(std::move(automaton), skip_whitespace) {};

        void set_str_input(const std::string &new_str);

        Token yylex() override;

        bool isInEnd() override;

        char SkipWS() override;

        std::tuple<std::string, int> getStrData();
    };
} //namespace compiler::analyzers


#endif //J2VP_LEXICAL_ANALYZER_S_H
