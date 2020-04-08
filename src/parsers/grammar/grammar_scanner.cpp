#include "J2VParser/parsers/grammar/grammar_scanner.h"

namespace J2VParser::grammar {

    using ::J2VParser::automata::DFA;

    GrammarScanner::GrammarScanner(io_buffer::TextSourceBuffer &sourceBuffer) :
            LexicalAnalyzerF(sourceBuffer, CreateAutomaton()) {}

    DFA GrammarScanner::CreateAutomaton() {
        //defines all variables for the creation of the characteristic dfa
        DFA::StateSet states = {0, 1, 2, 3, 4, 5, 6, 7};
        DFA::StateSet accepting_states = {1, 2, 3, 4, 5, 6, 7};
        std::map<DFA::State, DFA::Token> tokens = {
                {1, "QM"},
                {2, "EPSILON"},
                {3, "APOS"},
                {4, "COLON"},
                {5, "SEMICOLON"},
                {6, "VAR"},
                {7, "OR"}
        };
        std::string_view alpha_ = ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
        std::string_view numbers = "0123456789";
        std::string_view symbols = "\"#':;|";
        DFA::SymbolSet alphabet = {};
        std::map<DFA::Transition, DFA::State> transitions;

        //alpha transitions
        for (auto c : alpha_) {
            alphabet.insert(c);
            //var name states
            transitions[{0, c}] = 6;
            transitions[{6, c}] = 6;

            //invalid states
            for (DFA::State i = 1; i <= 5; i++) {
                transitions[{i, c}] = -1;
            }
            transitions[{7, c}] = -1;
        }

        //numbers transitions
        for (auto n : numbers) {
            alphabet.insert(n);
            for (DFA::State i = 0; i <= 5; i++)
                transitions[{i, n}] = -1;
            transitions[{6, n}] = 6;
            transitions[{7, n}] = -1;
        }

        //symbols transitions
        for (auto c : symbols) {
            alphabet.insert(c);
            for (DFA::State i = 1; i <= 7; i++)
                transitions[{i, c}] = -1;
        }
        // " symbol
        transitions[{0, '"'}] = 1;
        // # symbol
        transitions[{0, '#'}] = 2;
        // ' symbol
        transitions[{0, '\''}] = 3;
        // : symbol
        transitions[{0, ':'}] = 4;
        // ; symbol
        transitions[{0, ';'}] = 5;
        // | symbol
        transitions[{0, '|'}] = 7;

        return automata::DFA(states, alphabet, transitions, 0, accepting_states, tokens);
    }
} // namespace compiler::grammar

