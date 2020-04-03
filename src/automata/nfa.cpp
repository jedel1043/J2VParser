#include "automata/nfa.h"

#include <algorithm>
#include <stack>
#include <cstring>
#include <queue>

namespace compiler::automata {

    int NFA::state_counter_ = 1;

    NFA::NFA() : initial_state_(-1) {}

    NFA::~NFA() = default;

    std::set<char> NFA::alphabet() {
        std::map<std::pair<int, char>, std::set<int >>::iterator it;
        std::set<char> alphabet;
        for (it = transitions_.begin(); it != transitions_.end(); ++it) {
            char c = it->first.second;
            if (c != '\0')
                alphabet.insert(c);
        }
        return alphabet;
    }

    void NFA::AddAcceptingValue(const std::string &value) {
        for (int accepting_state : accepting_states_)
            accepting_values_.insert(std::make_pair(accepting_state, value));
    }

    std::string NFA::GetAcceptingValue(int state) {
        return accepting_values_.count(state) ? accepting_values_[state] : "";
    }

    DFA NFA::ToDFA() {
        // Configuration for the equivalent DFA
        int n = 1;
        std::map<std::pair<int, char>, int> new_transition;
        std::set<char> new_alphabet = alphabet();
        std::set<int> new_final_states;
        std::map<int, std::string> new_tokens;

        // Preparing the algorithm
        std::vector<std::set<int>> old_states;
        old_states.push_back(this->CalculateEpsilonClosure(initial_state_));
        std::queue<int> pending_states;
        pending_states.push(n);

        while (!pending_states.empty()) {
            int dstate = pending_states.front();
            pending_states.pop();
            std::set<int> state_set = old_states[dstate - 1];
            for (const char c : new_alphabet) {
                char str[2];
                str[0] = c;
                str[1] = '\0';
                std::set<int> result = this->Compute(state_set, &str[0]);

                if (result.empty()) {
                    new_transition.insert(std::pair<std::pair<int, char>, int>(std::make_pair(dstate, c), -1));
                    continue;
                }

                auto it = find(old_states.begin(), old_states.end(), result);
                if (it != old_states.end()) {
                    int state_index = (int) distance(old_states.begin(), it) + 1;
                    new_transition.insert(std::pair<std::pair<int, char>, int>(std::make_pair(dstate, c), state_index));
                } else {
                    pending_states.push(++n);
                    old_states.push_back(result);
                    new_transition.insert(std::pair<std::pair<int, char>, int>(std::make_pair(dstate, c), n));
                }
            }

            std::vector<int> intersection;
            std::set_intersection(state_set.begin(), state_set.end(), accepting_states_.begin(),
                                  accepting_states_.end(),
                                  back_inserter(intersection));
            if (!intersection.empty()) {
                new_final_states.insert(dstate);
                std::string token;
                for (int state : intersection)
                    token = accepting_values_[state];
                if (!token.empty())
                    new_tokens.insert(std::make_pair(dstate, token));
            }
        }

        return DFA(n, new_alphabet, new_transition, 1, new_final_states, new_tokens);
    }

    NFA::NFA(int size, int initial_state, const std::map<std::pair<int, char>, std::set<int>> &transitions,
             const std::set<int> &accepting_states) {
        std::map<int, int> tmp_dic;
        for (int i = 0; i < size; i++) {
            this->states_.insert(NFA::state_counter_);
            tmp_dic.insert(std::pair<int, int>(std::make_pair(i, NFA::state_counter_)));
            NFA::state_counter_++;
        }

        for (const auto &transition : transitions) {
            std::set<int> transition_states;
            for (int const t : transition.second)
                transition_states.insert(tmp_dic[t]);
            this->transitions_.insert(
                    std::pair<std::pair<int, char>,
                            std::set<int >>(std::make_pair(tmp_dic[transition.first.first], transition.first.second),
                                            transition_states));
        }
        for (int const state : accepting_states)
            this->accepting_states_.insert(tmp_dic[state]);
        this->initial_state_ = tmp_dic[initial_state];
    }

    std::set<int> NFA::ComputeNextStates(int state, char symbol) {
        std::set<int> next_states;
        std::pair<int, char> input(state, symbol);
        auto transition = this->transitions_.find(input);
        if (transition != this->transitions_.end())
            return transition->second;
        return next_states;
    }

    std::set<int> NFA::CalculateEpsilonClosure(int state) {
        std::set<int> closure_set;
        std::stack<int> states_saver;
        states_saver.push(state);

        while (!states_saver.empty()) {
            int check_state = states_saver.top();
            states_saver.pop();
            if (!closure_set.count(check_state))
                closure_set.insert(check_state);
            std::set<int> state_set = this->ComputeNextStates(check_state);
            for (int const element : state_set) {
                if (!closure_set.count(element)) {
                    closure_set.insert(element);
                    states_saver.push(element);
                }
            }
        }
        return closure_set;
    }

    std::set<int> NFA::CalculateEpsilonClosure(const std::set<int> &states_set) {
        std::set<int> result;
        for (int const state : states_set) {
            std::set<int> e_state = CalculateEpsilonClosure(state);
            result.insert(e_state.begin(), e_state.end());
        }
        return result;
    }

    std::set<int> NFA::Compute(const std::set<int> &states_set, char *str_input) {
        std::set<int> result;
        if (*str_input == '\0') {
            return CalculateEpsilonClosure(states_set);
        } else {
            for (int const state : states_set) {
                std::set<int> closure = CalculateEpsilonClosure(state);
                for (int const s : closure) {
                    std::set<int> s_states = this->ComputeNextStates(s, *str_input);
                    result.insert(s_states.begin(), s_states.end());
                }
            }
        }
        str_input++;
        return Compute(result, str_input);
    }

    bool NFA::ComputeString(const std::string &str) {
        std::set<int> configuration;
        configuration.insert(initial_state_);
        char cstr[str.size() + 1];
        strcpy(cstr, str.c_str());
        std::set<int> result = Compute(configuration, cstr);

        std::set<int> intersection;
        std::set_intersection(result.begin(), result.end(), accepting_states_.begin(), accepting_states_.end(),
                              inserter(intersection, intersection.begin()));

        return (!intersection.empty());
    }

    NFA NFA::CreateSimpleNFA(char c) {
        std::map<std::pair<int, char>, std::set<int>> transitions;
        std::set<int> accepting_states;

        std::set<int> t;
        t.insert(1);

        transitions.insert(std::pair<std::pair<int, char>, std::set<int >>(std::make_pair(0, c), t));
        accepting_states.insert(1);

        return NFA(2, 0, transitions, accepting_states);
    }

    NFA NFA::CreateSimpleNFA(char from, char to) {
        std::map<std::pair<int, char>, std::set<int>> transitions;
        std::set<int> accepting_states;

        std::set<int> t;
        t.insert(1);

        for (char c = from; c <= to; c++)
            transitions.insert(std::pair<std::pair<int, char>, std::set<int >>(std::make_pair(0, c), t));
        accepting_states.insert(1);

        return NFA(2, 0, transitions, accepting_states);
    }

    NFA NFA::CreateSimpleNFA(const std::set<char> &chars) {
        std::map<std::pair<int, char>, std::set<int>> transitions;
        std::set<int> accepting_states;

        std::set<int> t;
        t.insert(1);

        for (auto c : chars)
            transitions.insert(std::pair<std::pair<int, char>, std::set<int >>(std::make_pair(0, c), t));
        accepting_states.insert(1);

        return NFA(2, 0, transitions, accepting_states);
    }

    void NFA::AddTransition(int from, const std::set<int> &to, char symbol) {
        this->transitions_.insert(std::pair<std::pair<int, char>, std::set<int >>(std::make_pair(from, symbol), to));
    }

    NFA NFA::Concatenation(NFA concat_obj) {
        // New NFA
        NFA result;
        // New composite states_
        result.states_.insert(this->states_.begin(), this->states_.end());
        result.states_.insert(concat_obj.states_.begin(), concat_obj.states_.end());
        // New initial state
        result.initial_state_ = this->initial_state_;
        // New accepting states_
        result.accepting_states_.insert(concat_obj.accepting_states_.begin(), concat_obj.accepting_states_.end());
        // New transition function
        result.transitions_.insert(this->transitions_.begin(), this->transitions_.end());
        result.transitions_.insert(concat_obj.transitions_.begin(), concat_obj.transitions_.end());
        std::set<int> thompson_concat;
        thompson_concat.insert(concat_obj.initial_state_);
        for (int const state : this->accepting_states_) {
            result.AddTransition(state, thompson_concat, '\0');
        }

        return result;
    }

    NFA NFA::Union(NFA union_automata) {
        // New NFA
        NFA result;
        // New composite states_
        result.states_.insert(this->states_.begin(), this->states_.end());
        result.states_.insert(union_automata.states_.begin(), union_automata.states_.end());
        // New initial state
        result.initial_state_ = NFA::state_counter();
        // New accepting states_
        int new_final_state = NFA::state_counter();
        result.accepting_states_.insert(new_final_state);
        // Adding the new states_
        result.states_.insert(result.initial_state_);
        result.states_.insert(new_final_state);
        // New transition function
        result.transitions_.insert(this->transitions_.begin(), this->transitions_.end());
        result.transitions_.insert(union_automata.transitions_.begin(), union_automata.transitions_.end());
        std::set<int> thompson_union;
        thompson_union.insert(this->initial_state_);
        thompson_union.insert(union_automata.initial_state_);
        result.AddTransition(result.initial_state_, thompson_union, '\0');
        std::set<int> thompson_union2;
        thompson_union2.insert(new_final_state);
        for (int const state : this->accepting_states_)
            result.AddTransition(state, thompson_union2, '\0');
        for (int const state : union_automata.accepting_states_)
            result.AddTransition(state, thompson_union2, '\0');

        return result;
    }

    NFA NFA::CalculateLexicalUnion(const std::vector<NFA> &union_set) {
        NFA result;
        result.initial_state_ = NFA::state_counter();
        std::set<int> start_states;
        for (auto nfa : union_set) {
            result.states_.insert(nfa.states_.begin(), nfa.states_.end());
            result.accepting_states_.insert(nfa.accepting_states_.begin(), nfa.accepting_states_.end());
            result.transitions_.insert(nfa.transitions_.begin(), nfa.transitions_.end());
            start_states.insert(nfa.initial_state_);
            result.accepting_values_.insert(nfa.accepting_values_.begin(), nfa.accepting_values_.end());
        }
        result.states_.insert(result.initial_state_);
        result.AddTransition(result.initial_state_, start_states, '\0');

        return result;
    }

    NFA NFA::KleeneClosure() {
        // New NFA
        NFA result;
        // New states_
        result.states_.insert(this->states_.begin(), this->states_.end());
        // New initial state
        result.initial_state_ = NFA::state_counter();
        // New final states_
        int new_final_state = NFA::state_counter();
        result.accepting_states_.insert(new_final_state);
        // Adding the new states_
        result.states_.insert(result.initial_state_);
        result.states_.insert(new_final_state);
        // New transitions_
        result.transitions_.insert(this->transitions_.begin(), this->transitions_.end());
        std::set<int> initial_trans_state = result.accepting_states_;
        initial_trans_state.insert(this->initial_state_);
        result.AddTransition(result.initial_state_, initial_trans_state, '\0');
        std::set<int> new_final_states;
        new_final_states.insert(this->initial_state_);
        new_final_states.insert(new_final_state);
        for (int const state : this->accepting_states_)
            result.AddTransition(state, new_final_states, '\0');
        // Returning the result
        return result;
    }

    NFA NFA::PlusClosure() {
        // New NFA
        NFA result;
        // New states_
        result.states_.insert(this->states_.begin(), this->states_.end());
        // New initial state
        result.initial_state_ = NFA::state_counter();
        // New final states_
        int new_final_state = NFA::state_counter();
        result.accepting_states_.insert(new_final_state);
        // Adding the new states_
        result.states_.insert(result.initial_state_);
        result.states_.insert(new_final_state);
        // New transitions_
        result.transitions_.insert(this->transitions_.begin(), this->transitions_.end());
        std::set<int> initial_trans_state;
        initial_trans_state.insert(this->initial_state_);
        result.AddTransition(result.initial_state_, initial_trans_state, '\0');
        std::set<int> new_final_states;
        new_final_states.insert(this->initial_state_);
        new_final_states.insert(new_final_state);
        for (int const state : this->accepting_states_)
            result.AddTransition(state, new_final_states, '\0');
        // Returning the result
        return result;
    }

    NFA NFA::Optional() {
        // New NFA
        NFA result;
        // New states_
        result.states_.insert(this->states_.begin(), this->states_.end());
        // New initial state
        result.initial_state_ = NFA::state_counter();
        // New final states_
        int new_final_state = NFA::state_counter();
        result.accepting_states_.insert(new_final_state);
        // Adding the new states_
        result.states_.insert(result.initial_state_);
        result.states_.insert(new_final_state);
        // New transitions_
        result.transitions_.insert(this->transitions_.begin(), this->transitions_.end());
        std::set<int> initial_trans_state;
        initial_trans_state.insert(this->initial_state_);
        initial_trans_state.insert(new_final_state);
        result.AddTransition(result.initial_state_, initial_trans_state, '\0');
        std::set<int> new_final_states;
        new_final_states.insert(new_final_state);
        for (int const state : this->accepting_states_)
            result.AddTransition(state, new_final_states, '\0');
        // Returning the result
        return result;
    }

    std::string NFA::LexicalAccept(char *str, std::string &token, std::string &lexeme, std::string &str_result) {
        std::set<int> configuration;
        configuration.insert(this->initial_state_);

        size_t input_pos = std::string(str).size() - 1;
        std::set<int> intersection;
        char text[std::string(str).size()];
        strcpy(text, str);

        if (*text == '\0')
            return "";
        while (*text != '\0') {
            std::set<int> result = Compute(configuration, text);
            if (!result.empty()) {
                configuration = result;
                intersection.clear();
                std::set_intersection(result.begin(), result.end(), accepting_states_.begin(),
                                      accepting_states_.end(), inserter(intersection, intersection.begin()));
                if (!intersection.empty()) {
                    for (int last_state : intersection)
                        token = accepting_values_[last_state];
                    for (int i = 0; i <= input_pos; i++, str++);
                    lexeme = std::string(text);
                    str_result = std::string(str);
                    //cout << str << ": (" << Token << ", " << lexeme << ")" << std::endl;
                    return token;
                }
            } else {
                text[input_pos] = '\0';
                input_pos--;
            }
        }
        //cout << "(" << -1 << ", " << "" << ")" << std::endl;
        token = "";
        lexeme = "";
        return "";
    }

    std::ostream &operator<<(std::ostream &ostream1, const NFA &obj) {
        ostream1 << "------------------------\n";
        ostream1 << "States: ";
        for (int const state : obj.states_)
            ostream1 << state << ", ";
        ostream1 << std::endl;

        ostream1 << "Transition function: \n";
        std::map<std::pair<int, char>, std::set<int >>::const_iterator
                it;
        for (it = obj.transitions_.begin(); it != obj.transitions_.end(); ++it) {
            if (it->first.second == '\0')
                ostream1 << "\t(" << it->first.first << ", ) => {";
            else
                ostream1 << "\t(" << it->first.first << ", " << it->first.second << ") => { ";
            for (int const element : it->second)
                ostream1 << element << ", ";
            ostream1 << "}\n";
        }

        ostream1 << "Initial state: " << obj.initial_state_ << std::endl;
        ostream1 << "Accepting states_: ";
        for (int const element : obj.accepting_states_)
            ostream1 << element << ", ";
        ostream1 << "\n";

        ostream1 << "------------------------\n";

        return ostream1;
    }
} //namespace compiler::automata
