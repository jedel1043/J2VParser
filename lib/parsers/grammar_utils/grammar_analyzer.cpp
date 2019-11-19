#include "parsers/grammar_utils/grammar_analyzer.h"

#include "parsers/regex_utils/regex_scanner.h"
#include "parsers/regex_utils/regex_parser.h"

namespace compiler::grammar {

    GrammarAnalyzer::GrammarAnalyzer(io_buffer::TextSourceBuffer *input_file) :
            LexicalAnalyzerF(input_file, CreateAutomaton()) {}

    automata::DFA GrammarAnalyzer::CreateAutomaton() {
        regex::RegexScanner scanner(CreateFileBuffer());
        regex::RegexParser parser(scanner);
        std::vector <automata::NFA> machines = parser.Parse();
        automata::NFA result = automata::NFA::CalculateLexicalUnion(machines);
        return result.ToDFA().Minimize();
    }

    io_buffer::TextSourceBuffer *GrammarAnalyzer::CreateFileBuffer() {
        std::string regex_file = "grammar_regex.txt";
        std::ofstream file_out(regex_file);
        file_out << "[a-zA-Z_][a-zA-Z0-9_]*        VAR" << std::endl;
        file_out << R"(":"                           COLON)" << std::endl;
        file_out << R"(";"                           SEMICOLON)" << std::endl;
        file_out << R"("#"                           EPSILON)" << std::endl;
        file_out << R"("|"                           OR)" << std::endl;
        file_out << R"("'"                          APOS)" << std::endl;
        file_out << R"("""                          QM)" << std::endl;
        static io_buffer::TextSourceBuffer regex_grammar_file(regex_file);
        return &regex_grammar_file;
    }
} // namespace compiler::grammar

