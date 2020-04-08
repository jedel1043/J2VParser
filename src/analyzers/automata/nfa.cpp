#include "J2VParser/analyzers/automata/nfa.h"

#include <algorithm>
#include <stack>
#include <queue>
#include <utility>

#include "J2VParser/utils/bimap.h"
#include "J2VParser/utils/utils.h"

namespace J2VParser::automata {

    NFA::NFA(Symbol c) : NFA({0, 1}, 0, {{{0, c}, {1}}}, {1}) {}

    NFA::NFA(Symbol c_begin, Symbol c_end) : NFA({0, 1}, 0, {}, {1}) {
        for (Symbol c = c_begin; c <= c_end; c++)
            transitions_[{0, c}] = {1};
    }

    NFA::NFA(const SymbolSet &charset) : NFA({0, 1}, 0, {}, {1}) {
        for (auto c: charset)
            transitions_[{0, c}] = {1};
    }

    NFA::NFA(const std::string &input_str) {
        if (!input_str.empty()) {
            std::string::const_iterator it = input_str.begin();
            NFA result(*it++);
            while (it != input_str.end())
                result = result.Concat(NFA(*it++));
            states_ = result.states_;
            transitions_ = result.transitions_;
            accepting_states_ = result.accepting_states_;
        }
    }

    std::set<char> NFA::alphabet() const {
        std::set<char> alphabet;
        for (const auto &transition : transitions_) {
            char c = transition.first.second;
            if (c != '\0')
                alphabet.insert(c);
        }
        return alphabet;
    }

    void NFA::AddAcceptingValue(const std::string &value) {
        for (State accepting_state : accepting_states_)
            tokens_[accepting_state] = value;
    }

    std::string NFA::GetAcceptingValue(State state) const {
        return tokens_.at(state);
    }

    NFA::StateSet NFA::CalculateEpsilonClosure(State state) const {
        StateSet closure_set;
        std::stack<State> states_saver;
        states_saver.push(state);

        while (!states_saver.empty()) {
            int check_state = states_saver.top();
            states_saver.pop();
            if (!closure_set.count(check_state))
                closure_set.insert(check_state);
            StateSet state_set = at({check_state, 0});
            for (int const element : state_set) {
                if (!closure_set.count(element)) {
                    closure_set.insert(element);
                    states_saver.push(element);
                }
            }
        }
        return closure_set;
    }

    NFA::StateSet NFA::CalculateEpsilonClosure(const StateSet &states_set) const {
        StateSet result;
        for (int const state : states_set) {
            StateSet e_state = CalculateEpsilonClosure(state);
            result.insert(e_state.begin(), e_state.end());
        }
        return result;
    }

    NFA::StateSet NFA::at(Transition input_tran) const {
        StateSet empty_set;
        return (transitions_.count(input_tran) ? transitions_.at(input_tran) : empty_set);
    }

    NFA::StateSet NFA::at(const StateSet &states_set, Symbol symbol) const {
        if (states_set.empty())
            return {};
        StateSet result;
        for (auto const &state : states_set) {
            StateSet closure = CalculateEpsilonClosure(state);
            StateSet s_states;
            for (auto const &s : closure) {
                s_states = at({s, symbol});
                result.insert(s_states.begin(), s_states.end());
            }
        }
        return CalculateEpsilonClosure(result);
    }

    NFA::StateSet NFA::ComputeString(const std::string &str) const {
        StateSet result = CalculateEpsilonClosure(initial_state_);
        std::string::const_iterator it = str.begin();

        while (it != str.end() && !result.empty())
            result = at(result, *it++);

        return result;
    }

    bool NFA::AcceptsString(const std::string &str) const {
        StateSet result = CalculateEpsilonClosure(initial_state_);
        std::string::const_iterator it = str.begin();

        while (it != str.end() && !result.empty())
            result = at(result, *it++);

        if (result.empty())
            return false;

        StateSet intersection;
        std::set_intersection(result.begin(), result.end(), accepting_states_.begin(), accepting_states_.end(),
                              inserter(intersection, intersection.end()));

        return (!intersection.empty());
    }

    NFA::Token NFA::Tokenize(const std::string &str) const {
        const auto &last_state = ComputeString(str);
        StateSet intersection;
        std::set_intersection(last_state.begin(), last_state.end(),
                              accepting_states_.begin(), accepting_states_.end(),
                              std::inserter(intersection, intersection.begin()));
        if (!intersection.empty()) {
            Token token;
            for (const auto &state : intersection) {
                if (tokens_.count(state))
                    token = tokens_.at(state);
            }
            if (!token.empty())
                return token;
        }
        return "";
    }

    DFA NFA::ToDFA() const {
        // Configuration for the equivalent DFA
        StateSet new_states{0};
        std::map<Transition, State> new_transitions;
        SymbolSet new_alphabet = alphabet();
        StateSet new_final_states;
        std::map<State, Token> new_tokens;

        // Preparing the algorithm
        utils::Bimap<State, StateSet> old_states{{{0, CalculateEpsilonClosure(initial_state_)}}};
        std::queue<State> pending_states;
        pending_states.push(0);

        while (!pending_states.empty()) {
            State dstate = pending_states.front();
            pending_states.pop();
            const auto &curr_state = old_states.at(dstate);
            for (const char c : new_alphabet) {
                State new_state = old_states.size();
                StateSet result = at(curr_state, c);

                if (result.empty()) {
                    new_transitions[{dstate, c}] = -1;
                    continue;
                }

                if (old_states.count(result))
                    new_transitions[{dstate, c}] = old_states.at(result);
                else {
                    pending_states.push(new_state);
                    old_states.insert(new_state, result);
                    new_states.insert(new_state);
                    new_transitions[{dstate, c}] = new_state;
                }
            }

            StateSet intersection;
            std::set_intersection(curr_state.begin(), curr_state.end(),
                                  accepting_states_.begin(), accepting_states_.end(),
                                  std::inserter(intersection, intersection.begin()));
            if (!intersection.empty()) {
                new_final_states.insert(dstate);
                Token token;
                for (const auto &state : intersection) {
                    if (tokens_.count(state))
                        token = tokens_.at(state);
                }
                if (!token.empty())
                    new_tokens[dstate] = token;
            }
        }

        return DFA{new_states, new_alphabet, new_transitions, 0, new_final_states, new_tokens};
    }

    NFA NFA::ShiftStates(State shift_len) const {
        StateSet states_acc;
        StateSet states_r;
        std::map<Transition, StateSet> tran_f_r;
        std::map<State, Token> acc_tkn;

        // Displace all states by shift_len
        std::for_each(states_.begin(), states_.end(),
                      [&states_r, shift_len](auto n) { states_r.insert(n + shift_len); }
        );

        // Displace accepting states by shift_len
        std::for_each(accepting_states_.begin(), accepting_states_.end(),
                      [&states_acc, shift_len](auto n) { states_acc.insert(n + shift_len); }
        );

        // Displace transition function states
        std::for_each(transitions_.begin(), transitions_.end(),
                      [&tran_f_r, shift_len](const auto &pair) {
                          StateSet res_s;
                          std::for_each(pair.second.begin(), pair.second.end(),
                                        [&res_s, shift_len](auto set_int) {
                                            res_s.insert(set_int + shift_len);
                                        }
                          );
                          tran_f_r[{pair.first.first + shift_len, pair.first.second}] = res_s;
                      }
        );

        // Displace accepting tokens states
        std::for_each(tokens_.begin(), tokens_.end(),
                      [&acc_tkn, shift_len](const auto &pair) {
                          acc_tkn[pair.first + shift_len] = pair.second;
                      }
        );

        return NFA(states_r, initial_state_ + shift_len, tran_f_r, states_acc, acc_tkn);
    }

    NFA NFA::Concat(const NFA &nfa_in) const {
        State old_size = states_.size();

        NFA result = nfa_in.ShiftStates(old_size - 1);
        result.states_.insert(states_.begin(), --states_.end());
        result.transitions_.insert(transitions_.begin(), transitions_.end());

        result.initial_state_ = 0;
        return result;
    }

    NFA NFA::Union(const NFA &nfa_in) const {
        // New NFA
        NFA result = ShiftStates(1);
        NFA aux_nfa = nfa_in.ShiftStates(states_.size() + 1);

        result.initial_state_ = 0;
        result.states_.insert(0);
        result.states_.insert(aux_nfa.states_.begin(), aux_nfa.states_.end());

        State size = result.states_.size();

        result.states_.insert(size);
        result.transitions_.insert(aux_nfa.transitions_.begin(), aux_nfa.transitions_.end());

        result.transitions_[{0, 0}] = {1, aux_nfa.initial_state_};

        result.transitions_[{*aux_nfa.accepting_states_.begin(), 0}] = {size};
        result.transitions_[{*result.accepting_states_.begin(), 0}] = {size};

        result.accepting_states_ = {size};

        return result;
    }

    NFA NFA::KleeneClose() const {
        NFA result = ShiftStates(1);
        result.states_.insert(0);

        State old_last_st = *result.accepting_states_.begin();

        result.states_.insert(old_last_st + 1);

        result.transitions_[{0, 0}] = {1, old_last_st + 1};
        result.transitions_[{old_last_st, 0}] = {1, old_last_st + 1};

        result.initial_state_ = 0;
        result.accepting_states_ = {old_last_st + 1};
        return result;
    }

    NFA NFA::PlusClose() const {
        NFA result = ShiftStates(1);
        result.states_.insert(0);

        State old_last_st = *result.accepting_states_.begin();

        result.states_.insert(old_last_st + 1);

        result.transitions_[{0, 0}] = {1};
        result.transitions_[{old_last_st, 0}] = {1, old_last_st + 1};

        result.initial_state_ = 0;
        result.accepting_states_ = {old_last_st + 1};
        return result;
    }

    NFA NFA::Optionalize() const {
        NFA result = ShiftStates(1);
        result.states_.insert(0);

        State old_last_st = *result.accepting_states_.begin();

        result.states_.insert(old_last_st + 1);

        result.transitions_[{0, 0}] = {1, old_last_st + 1};
        result.transitions_[{old_last_st, 0}] = {old_last_st + 1};

        result.initial_state_ = 0;
        result.accepting_states_ = {old_last_st + 1};
        return result;
    }

    NFA NFA::CalculateLexicalUnion(const std::vector<NFA> &union_set) {
        NFA result;
        StateSet second_states;
        result.states_.insert(0);
        result.initial_state_ = 0;
        for (const auto &nfa : union_set) {
            NFA saver = nfa.ShiftStates(result.states_.size());
            result.states_.insert(saver.states_.begin(), saver.states_.end());
            result.accepting_states_.insert(saver.accepting_states_.begin(), saver.accepting_states_.end());
            result.transitions_.insert(saver.transitions_.begin(), saver.transitions_.end());
            result.tokens_.insert(saver.tokens_.begin(), saver.tokens_.end());
            second_states.insert(saver.initial_state_);
        }
        result.transitions_[{0, 0}] = second_states;
        return result;
    }

    void NFA::print(std::ostream &ostream) const {
        std::vector<std::string> str_states, str_acc_states, str_tokens;
        std::for_each(states_.begin(), states_.end(),
                      [&str_states](auto n) { str_states.emplace_back(std::to_string(n)); });
        std::for_each(accepting_states_.begin(), accepting_states_.end(),
                      [&str_acc_states](auto n) { str_acc_states.emplace_back(std::to_string(n)); });
        std::for_each(tokens_.begin(), tokens_.end(),
                      [&str_tokens](const std::pair<Automaton::State, Automaton::Token> &input) {
                          str_tokens.emplace_back(std::to_string(input.first) + " -> " + input.second);
                      }
        );
        ostream << std::endl;
        ostream << "States: {" << utils::ElemDivider(str_states) << '}' << std::endl;
        ostream << "Initial state: " << std::to_string(initial_state_) << std::endl;
        ostream << "Accepting states: {" << utils::ElemDivider(str_acc_states) << '}' << std::endl;
        if (!str_tokens.empty())
            ostream << "Tokens: {" << utils::ElemDivider(str_tokens) << '}' << std::endl;
        ostream << "Transition function: {" << std::endl;
        std::set<std::string> saver;
        for (const auto &[trans, next_set] : transitions_) {
            std::for_each(next_set.begin(), next_set.end(),
                          [&saver](auto n) { saver.insert(std::to_string(n)); });
            ostream << "\t(" << std::to_string(trans.first) << ", ";
            if (trans.second != 0)
                ostream << trans.second;
            ostream << ") => {" << utils::ElemDivider(saver) << "}" << std::endl;
            saver.clear();
        }
        ostream << '}' << std::endl;
    }

} //namespace compiler::automata
