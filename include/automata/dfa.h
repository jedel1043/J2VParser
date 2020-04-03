#ifndef DFA_H
#define DFA_H

#include <map>
#include <set>
#include <cstdio>
#include <utility>
#include <vector>

namespace compiler::automata {
    class DFA {
    private:
        int size_;
        std::set<int> states_;
        std::set<char> alphabet_;
        std::map<std::pair<int, char>, int> transitions_;
        int initial_state_;
        std::set<int> accepting_states_;
        std::map<int, std::string> tokens_;
    public:
        std::set<int> InverseTransition(const std::set<int> &new_states, char input_char);

        int Compute(const std::string &string_input);

        DFA(int size, std::set<char> alphabet, std::map<std::pair<int, char>, int> transitions,
            int initial_state, std::set<int> accepting_states, std::map<int, std::string> tokens) :
                size_(size),
                alphabet_(std::move(alphabet)),
                transitions_(std::move(transitions)),
                initial_state_(initial_state),
                accepting_states_(std::move(accepting_states)),
                tokens_(std::move(tokens)) {
            for (int j = 1; j <= size; j++)
                states_.insert(j);
        }

        DFA(std::set<int> states, std::set<char> alphabet, std::map<std::pair<int, char>, int> transitions,
            int initial_state, std::set<int> accepting_states, std::map<int, std::string> tokens) :
                states_(std::move(states)),
                alphabet_(std::move(alphabet)),
                transitions_(std::move(transitions)),
                initial_state_(initial_state),
                accepting_states_(std::move(accepting_states)),
                tokens_(std::move(tokens)),
                size_(states.size()) {
        }

        void Print();

        void PrintToFile(const std::string &filename);

        DFA Minimize();

        std::string ComputeString(const std::string &str);

        const std::set<int> &accepting_states();

        const std::map<int, std::string> &tokens();

        int initial_state();

        int Compute(int state, char c);
    };

} //nampespace compiler::automata

#endif
