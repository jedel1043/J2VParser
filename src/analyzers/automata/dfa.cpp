#include "J2VParser/analyzers/automata/dfa.h"

#include <iostream>
#include <algorithm>
#include <iomanip>


namespace J2VParser::automata {

    DFA::State DFA::at(Transition t) const {
        if (!transitions_.count(t))
            return -1;
        return transitions_.at(t);
    }

    DFA::State DFA::ComputeString(const std::string &str) const {
        State current_state = initial_state_;
        for (auto c : str) {
            current_state = at({current_state, c});
            if (current_state == -1) return -1;
        }
        return current_state;
    }

    DFA::Token DFA::Tokenize(const std::string &str) const {
        State result = ComputeString(str);
        return tokens_.count(result) ? tokens_.at(result) : "";
    }

    bool DFA::AcceptsString(const std::string &str) const {
        return accepting_states_.count(ComputeString(str));
    }

    void DFA::print(std::ostream &ostream) const {
        const int half_colw = kColWPrint / 2;
        const int real_colw = kColWPrint % 2 == 1 ? kColWPrint : kColWPrint + 1;
        const size_t totalw = (alphabet_.size() + 1) * (real_colw + 1) + 1;
        ostream << std::endl << '|' << std::string(real_colw, ' ') << '|';

        for (auto c : alphabet_) {
            ostream << std::string(half_colw, ' ') << std::left
                    << std::setw(half_colw + 1) << c;
            ostream << '|';
        }
        ostream << std::endl << std::string(totalw, '-') << std::endl;
        for (auto st : states_) {
            ostream << '|' << std::right << std::setw(half_colw)
                    << (accepting_states_.count(st) ? std::string("*") : std::string())
                       + (initial_state_ == st ? std::string(">") : std::string())
                    << std::left << std::setw(half_colw + 1) << std::to_string(st);
            ostream << '|';
            for (auto c : alphabet_) {
                if (transitions_.at({st, c}) == -1) {
                    ostream << std::string(half_colw - 1, ' ') << "-1"
                            << std::string(half_colw, ' ') << '|';
                    continue;
                }
                ostream << std::string(half_colw, ' ') << std::left
                        << std::setw(half_colw + 1) << transitions_.at({st, c});
                ostream << '|';
            }
            ostream << std::endl;
        }
        if (!tokens_.empty()) {
            std::vector<std::string> str_tokens;
            std::for_each(tokens_.begin(), tokens_.end(),
                          [&str_tokens](const std::pair<Automaton::State, Automaton::Token> &input) {
                              str_tokens.emplace_back(std::to_string(input.first) + " -> " + input.second);
                          });
            ostream << "Tokens: {" << utils::ElemDivider(str_tokens) << '}' << std::endl;
        }
    }

    DFA::StateSet DFA::InverseTransition(const StateSet &input_sts, Symbol input_sym) const {
        StateSet result;
        for (const State state : input_sts) {
            for (const auto&[tran, next] : transitions_) {
                if (next == state && tran.second == input_sym)
                    result.insert(tran.first);
            }
        }
        return result;
    }

    DFA DFA::Minimize() const {
        std::set<StateSet> P;
        std::map<State, Token> new_tokens;
        if (!tokens_.empty()) {
            for (State s : accepting_states_)
                P.insert({s});
        } else
            P.insert(accepting_states_);

//        P.insert({*accepting_states_.begin()});
//        for (State state : accepting_states_) {
//            if (tokens_.at(state) != tokens_.at(*accepting_states_.begin())) {
//                auto it = P.begin();
//                while (it != P.end()) {
//                    if (tokens_.at(*((*it).begin())) == tokens_.at(state)) {
//                        StateSet a = *it;
//                        a.insert(state);
//                        P.insert(a);
//                        P.erase(*it);
//                        break;
//                    }
//                    it++;
//                }
//                if (it == P.end())
//                    P.insert({state});
//            }
//        }
        std::set<StateSet> W;
        W.insert(P.begin(), P.end());

        StateSet intersection;
        std::set_difference(states_.begin(), states_.end(), accepting_states_.begin(), accepting_states_.end(),
                            inserter(intersection, intersection.end()));
        if (!intersection.empty())
            P.insert(intersection);

        while (!W.empty()) {
            std::set<StateSet>::iterator it;
            it = W.begin();
            StateSet A = *it;
            W.erase(it);
            for (const char c : alphabet_) {
                StateSet X = InverseTransition(A, c);
                for (it = P.begin(); it != P.end();) {
                    StateSet Y = *it;
                    StateSet XY;
                    StateSet Y_X;
                    std::set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), inserter(XY, XY.end()));
                    std::set_difference(Y.begin(), Y.end(), X.begin(), X.end(), inserter(Y_X, Y_X.end()));
                    if (XY.empty() || Y_X.empty()) {
                        it++;
                        continue;
                    }
                    P.erase(it);
                    P.insert(XY);
                    P.insert(Y_X);
                    if (W.count(Y) != 0) {
                        W.erase(it);
                        W.insert(XY);
                        W.insert(Y_X);
                    } else {
                        if (XY.size() <= Y_X.size())
                            W.insert(XY);
                        else
                            W.insert(Y_X);
                    }
                    it = P.begin();
                }
            }
        }

        std::map<Transition, State> new_transitions;
        for (const auto &[tran, next] : transitions_) {
            State from_state, to_state = -1, current_class = 0;
            bool got_from = false, got_to = false;
            if (next == -1)
                got_to = true;
            for (const auto &set_st : P) {
                if (got_from && got_to)
                    break;
                if (!got_from && set_st.count(tran.first)) {
                    from_state = current_class;
                    got_from = true;
                }
                if (!got_to && set_st.count(next)) {
                    to_state = current_class;
                    got_to = true;
                }
                current_class++;
            }
            new_transitions[{from_state, tran.second}] = to_state;
        }
        int new_initial = 0;
        for (const StateSet &class_p : P) {
            if (class_p.count(initial_state_) != 0) break;
            new_initial++;
        }
        StateSet new_accepting;
        StateSet new_states;
        if (!tokens_.empty()) {
            State current_class = 0;
            for (const StateSet &class_p : P) {
                Token token;
                for (const int state : accepting_states_) {
                    if (class_p.count(state) != 0) {
                        token = tokens_.at(state);
                        new_accepting.insert(current_class);
                    }
                }
                if (!token.empty())
                    new_tokens.insert({current_class, token});
                new_states.insert(current_class++);
            }
        }
        return DFA(new_states, alphabet_, new_transitions, new_initial, new_accepting, new_tokens);
    }

} //namespace compiler::automata
