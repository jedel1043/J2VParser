#ifndef J2VPARSER_LEXICAL_ANALYZER_S_H
#define J2VPARSER_LEXICAL_ANALYZER_S_H

#include <string>
#include <utility>

#include "J2VParser/analyzers/lexical_analyzer.h"
#include "J2VParser/analyzers/automata/dfa.h"

namespace J2VParser::analyzers {
    class LexicalAnalyzerS : public LexicalAnalyzer {
    private:
        const std::string &str_input_;
        std::string::const_iterator str_pos_;
    public:
        LexicalAnalyzerS(const std::string &str_input,
                         automata::DFA automaton,
                         bool skip_whitespace = true);

        LexicalAnalyzerS(const std::string &str_input,
                         io_buffer::TextSourceBuffer &regex_f,
                         bool skip_whitespace = true);

        Token jvly() override;

        [[nodiscard]] bool eos() const override;

        char SkipWS() override;

        std::tuple<std::string, int> getStrData();
    };
} //namespace compiler::analyzers


#endif //J2VPARSER_LEXICAL_ANALYZER_S_H
