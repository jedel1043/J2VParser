#include "J2VParser/parsers/grammar/grammar_scanner.h"

namespace J2VParser::grammar {

    GrammarScanner::GrammarScanner(io_buffer::TextSourceBuffer &sourceBuffer) :
            LexicalAnalyzerF(sourceBuffer, CreateAutomaton()) {}

    automata::DFA GrammarScanner::CreateAutomaton() {
        //defines all variables for the creation of the characteristic dfa
        std::set<int> states = {1, 2, 3, 4, 5, 6, 7, 8};
        std::set<int> accepting_states = {2, 3, 4, 5, 6, 7, 8};
        std::map<int, std::string> tokens = {
                {2, "QM"},
                {3, "EPSILON"},
                {4, "APOS"},
                {5, "COLON"},
                {6, "SEMICOLON"},
                {7, "VAR"},
                {8, "OR"}
        };
        std::string_view alpha_ = ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
        std::string_view numbers = "0123456789";
        std::string_view symbols = "\"#':;|";
        std::set<char> alphabet = {};
        std::map<std::pair<int, char>, int> transitions;

        //alpha transitions
        for (auto c : alpha_) {
            alphabet.insert(c);
            //var name states
            transitions[{1, c}] = 7;
            transitions[{7, c}] = 7;

            //invalid states
            for (int i = 2; i <= 6; i++) {
                transitions[{i, c}] = -1;
            }
            transitions[{8, c}] = -1;
        }

        //numbers transitions
        for (auto n : numbers) {
            alphabet.insert(n);
            for (int i = 1; i <= 6; i++)
                transitions[{i, n}] = -1;
            transitions[{7, n}] = 7;
            transitions[{8, n}] = -1;
        }

        //symbols transitions
        for (auto c : symbols) {
            alphabet.insert(c);
            for (int i = 2; i <= 8; i++)
                transitions[{i, c}] = -1;
        }
        // " symbol
        transitions[{1, '"'}] = 2;
        // # symbol
        transitions[{1, '#'}] = 3;
        // ' symbol
        transitions[{1, '\''}] = 4;
        // : symbol
        transitions[{1, ':'}] = 5;
        // ; symbol
        transitions[{1, ';'}] = 6;
        // | symbol
        transitions[{1, '|'}] = 8;

        return automata::DFA(states, alphabet, transitions, 1, accepting_states, tokens);
    }
} // namespace compiler::grammar

