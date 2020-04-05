#ifndef J2VP_NFA_H
#define J2VP_NFA_H

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>

#include "J2VParser/analyzers/automata/dfa.h"

namespace J2VParser::automata {
    class NFA {
    private:
        static int state_counter_;
        std::set<int> states_;
        int initial_state_;
        std::map<std::pair<int, char>, std::set<int>> transitions_;
        std::set<int> accepting_states_;
        std::map<int, std::string> accepting_values_;

        std::set<int> ComputeNextStates(int state, char symbol = '\0');

        std::set<int> CalculateEpsilonClosure(int state);

        std::set<int> CalculateEpsilonClosure(const std::set<int> &states_set);

        std::set<int> Compute(const std::set<int> &states_set, char *string);

        void AddTransition(int from, const std::set<int> &to, char symbol);

        static int state_counter() {
            int number = NFA::state_counter_;
            NFA::state_counter_++;
            return number;
        }

    public:
        NFA();

        NFA(int size,
            int initial_state,
            const std::map<std::pair<int, char>, std::set<int>> &transitions,
            const std::set<int> &accepting_states);

        bool ComputeString(const std::string &str);

        std::set<char> alphabet();

        DFA ToDFA();

        static NFA CreateSimpleNFA(char c);

        static NFA CreateSimpleNFA(char from, char to);

        static NFA CreateSimpleNFA(const std::set<char> &chars);

        static NFA CalculateLexicalUnion(const std::vector <NFA> &union_set);

        NFA Concatenation(NFA concat_obj);

        NFA Union(NFA union_automaton);

        NFA KleeneClosure();

        NFA PlusClosure();

        NFA Optional();

        friend std::ostream &operator<<(std::ostream &ostream1, const NFA &obj);

        void AddAcceptingValue(const std::string &value);

        std::string GetAcceptingValue(int state);

        std::string LexicalAccept(char *str, std::string &token, std::string &lexeme, std::string &str_result);
    };

    std::ostream &operator<<(std::ostream &ostream, const NFA &obj);

}// namespace compiler::automata
#endif //J2VP_NFA_H
