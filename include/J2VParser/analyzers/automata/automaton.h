#ifndef J2VPARSER_AUTOMATA_H
#define J2VPARSER_AUTOMATA_H

#include <set>
#include <string>
#include <map>
#include <ostream>
#include <algorithm>
#include <utility>

#include "J2VParser/utils/utils.h"

namespace J2VParser::automata {

    class Automaton {
    public:
        using State = int;
        using Symbol = char;
        using SymbolSet = std::set<Symbol>;
        using StateSet = std::set<State>;
        using Token = std::string;
        using Transition = std::pair<State, Symbol>;

        [[nodiscard]] State initial_state() const { return initial_state_; };

        [[nodiscard]] const StateSet &accepting_states() const { return accepting_states_; };

        [[nodiscard]] const std::map<State, Token> &tokens() const { return tokens_; };

        [[nodiscard]] virtual std::string Tokenize(const std::string &str) const = 0;

        [[nodiscard]] virtual bool AcceptsString(const std::string &str) const = 0;

        friend std::ostream &operator<<(std::ostream &o, const Automaton &rhs);

        virtual ~Automaton() = default;

    protected:
        StateSet states_;
        State initial_state_;
        StateSet accepting_states_;
        std::map<State, Token> tokens_;

        Automaton() { initial_state_ = 0; };

        Automaton(StateSet states, State initial_state,
                  StateSet accepting_states, std::map<State, Token> tokens) :
                states_(std::move(states)), initial_state_(initial_state),
                accepting_states_(std::move(accepting_states)), tokens_(std::move(tokens)) {};

        Automaton(StateSet states, State initial_state, StateSet accepting_states) :
                states_(std::move(states)), initial_state_(initial_state),
                accepting_states_(std::move(accepting_states)) {};

        virtual void print(std::ostream &ostream) const = 0;
    };

    inline std::ostream &operator<<(std::ostream &ostream, const Automaton &rhs) {
        rhs.print(ostream);
        return ostream;
    }
} // namespace J2VParser::automata
#endif //J2VPARSER_AUTOMATA_H
