#ifndef J2VPARSER_DFA_H
#define J2VPARSER_DFA_H

#include <map>
#include <set>
#include <utility>

#include "J2VParser/analyzers/automata/automaton.h"


namespace J2VParser::automata {

    class DFA : public Automaton {
    public:
        inline static int kColWPrint = 9;

        DFA(StateSet states, SymbolSet alphabet, std::map<Transition, State> transitions,
            State initial_state, StateSet accepting_states, std::map<State, std::string> tokens) :
                Automaton(std::move(states), initial_state,
                          std::move(accepting_states), std::move(tokens)),
                alphabet_(std::move(alphabet)), transitions_(std::move(transitions)) {}

        [[nodiscard]] State at(Transition input_tran) const;

        [[nodiscard]] State ComputeString(const std::string &str) const;

        [[nodiscard]] Token Tokenize(const std::string &str) const override;

        [[nodiscard]] bool AcceptsString(const std::string &str) const override;

        DFA Minimize() const;

    private:
        SymbolSet alphabet_;
        std::map<Transition, State> transitions_;

        StateSet InverseTransition(const StateSet &input_sts, Symbol input_sym) const;

        void print(std::ostream &ostream) const override;
    };

} //namespace compiler::automata

#endif //J2VPARSER_DFA_H
