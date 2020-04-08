#ifndef J2VPARSER_NFA_H
#define J2VPARSER_NFA_H

#include <utility>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>

#include "J2VParser/analyzers/automata/dfa.h"
#include "J2VParser/analyzers/automata/automaton.h"

namespace J2VParser::automata {
    class NFA : public Automaton {
    public:

        NFA() = default;

        explicit NFA(Symbol c);

        explicit NFA(Symbol c_begin, Symbol c_end);

        explicit NFA(const std::string &input_str);

        explicit NFA(const SymbolSet &charset);

        void AddAcceptingValue(const std::string &value);

        [[nodiscard]] DFA ToDFA() const;

        [[nodiscard]] NFA Concat(const NFA &nfa_in) const;

        [[nodiscard]] NFA Union(const NFA &nfa_in) const;

        [[nodiscard]] NFA KleeneClose() const;

        [[nodiscard]] NFA PlusClose() const;

        [[nodiscard]] NFA Optionalize() const;

        [[nodiscard]] StateSet at(Transition input_tran) const;

        [[nodiscard]] StateSet at(const StateSet &states_set, Symbol symbol) const;

        [[nodiscard]] StateSet ComputeString(const std::string &str) const;

        [[nodiscard]] bool AcceptsString(const std::string &str) const override;

        [[nodiscard]] Token Tokenize(const std::string &str) const override;

        [[nodiscard]] SymbolSet alphabet() const;

        [[nodiscard]] std::string GetAcceptingValue(State state) const;

        static NFA CalculateLexicalUnion(const std::vector<NFA> &union_set);

    private:
        std::map<Transition, StateSet> transitions_;

        NFA(StateSet states, State initial_state,
            std::map<Transition, StateSet> transitions, StateSet accepting_states) :
                Automaton(std::move(states), initial_state, std::move(accepting_states)),
                transitions_(std::move(transitions)) {};

        NFA(StateSet states, State initial_state, std::map<Transition, StateSet> transitions,
            StateSet accepting_states, std::map<State, Token> tokens) :
                Automaton(std::move(states), initial_state, std::move(accepting_states), std::move(tokens)),
                transitions_(std::move(transitions)) {};

        [[nodiscard]] StateSet CalculateEpsilonClosure(State state) const;

        [[nodiscard]] StateSet CalculateEpsilonClosure(const StateSet &states_set) const;

        [[nodiscard]] NFA ShiftStates(State shift_len) const;

        void print(std::ostream &ostream) const override;

    };

}// namespace compiler::automata
#endif //J2VPARSER_NFA_H
