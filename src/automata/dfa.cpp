#include "J2VParser/automata/dfa.h"

#include <iostream>
#include <algorithm>
#include <fstream>


namespace compiler::automata {
    void DFA::Print() {
        std::cout << "Tokens:" << std::endl;
        for (const auto &token : tokens_)
            std::cout << "{" << token.first << ", " << token.second << "}, ";
        std::cout << std::endl;
        int groupingLen = 25;
        int numberOfGroups = (int) alphabet_.size() / groupingLen;
        std::set<std::set<char>> groups;
        std::string out;

        for (int i = 0; i <= numberOfGroups; i++) {
            auto begin_char = alphabet_.begin();
            auto end_char = alphabet_.begin();
            std::set<char> temp;

            advance(begin_char, i * groupingLen);

            if (i == numberOfGroups) {
                end_char = alphabet_.end();
            } else {
                advance(end_char, (i + 1) * groupingLen);
            }

            temp.insert(begin_char, end_char);

            printf("     |");
            for (const char c : temp)
                printf("  %c  |", c);
            printf("\n------");
            for (int j = 0; j < temp.size(); j++)
                printf("------");
            printf("\n");

            for (int j = 1; j <= size_; j++) {
                if (accepting_states_.count(j) != 0)
                    printf("*");
                else
                    printf(" ");
                if (initial_state_ == j)
                    printf(">");
                else
                    printf(" ");

                printf("%2d |", j);
                for (const char c : temp)
                    printf(" %3d |", transitions_[std::make_pair(j, c)]);
                printf("\n");
            }
            std::cout << "\n";
        }
    }

    void DFA::PrintToFile(const std::string &filename) {
        std::ofstream file(filename);
        file << "     |";
        for (const char c : alphabet_) {
            char buffer[7];
            sprintf(buffer, "  %c  |", c);
            file << buffer;
        }
        file << "\n------";
        for (int i = 0; i < alphabet_.size(); i++)
            file << "------";
        file << "\n";

        for (int i = 1; i <= size_; i++) {
            if (accepting_states_.count(i) != 0)
                file << "*";
            else
                file << " ";
            if (initial_state_ == i)
                file << ">";
            else
                file << " ";

            char buffer2[5];
            sprintf(buffer2, "%2d |", i);
            file << buffer2;
            for (const char c : alphabet_) {
                char buffer3[7];
                sprintf(buffer3, " %3d |", transitions_[std::make_pair(i, c)]);
                file << buffer3;
            }
            file << "\n";
        }
        file << std::endl;

        for (const auto &elem : tokens_)
            file << elem.first << ':' << elem.second << std::endl;
        file.close();
    }

    std::set<int> DFA::InverseTransition(const std::set<int> &new_states, char input_char) {
        std::set<int> result;
        std::map<std::pair<int, char>, int>::iterator
                it;
        for (const int state : new_states) {
            for (it = transitions_.begin(); it != transitions_.end(); ++it) {
                if (it->second == state && it->first.second == input_char)
                    result.insert(it->first.first);
            }
        }
        return result;
    }

    DFA DFA::Minimize() {
        std::set<std::set<int>> P;
        std::map<int, std::string> new_tokens;
        P.insert({*accepting_states_.begin()});
        for (int state : accepting_states_) {
            if (tokens_[state] != tokens_[*accepting_states_.begin()]) {
                auto it = P.begin();
                while (it != P.end()) {
                    if (tokens_[*((*it).begin())] == tokens_[state]) {
                        std::set<int> a = *it;
                        a.insert(state);
                        P.insert(a);
                        P.erase(*it);
                        break;
                    }
                    it++;
                }
                if (it == P.end())
                    P.insert({state});
            }
        }
        std::set<std::set<int>> W;
        W.insert(P.begin(), P.end());

        std::set<int> intersection;
        std::set_difference(states_.begin(), states_.end(), accepting_states_.begin(), accepting_states_.end(),
                            inserter(intersection, intersection.end()));
        if (!intersection.empty())
            P.insert(intersection);

        while (!W.empty()) {
            std::set<std::set<int >>::iterator
                    it;
            it = W.begin();
            std::set<int> A = *it;
            W.erase(it);
            for (const char c : alphabet_) {
                std::set<int> X = InverseTransition(A, c);
                for (it = P.begin(); it != P.end();) {
                    std::set<int> Y = *it;
                    std::set<int> XY;
                    std::set<int> Y_X;
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

        int new_size = P.size();
        std::map<std::pair<int, char>, int> new_transitions;
        std::map<std::pair<int, char>, int>::iterator
                itt;
        for (itt = transitions_.begin(); itt != transitions_.end(); ++itt) {
            int from_state, to_state, current_class = 1;
            bool got_from = false, got_to = false;
            if (itt->second == -1) {
                to_state = -1;
                got_to = true;
            }
            for (auto it = P.begin(); !(got_from && got_to); it++) {
                if (!got_from && (*it).count(itt->first.first)) {
                    from_state = current_class;
                    got_from = true;
                }
                if (!got_to && (*it).count(itt->second)) {
                    to_state = current_class;
                    got_to = true;
                }
                current_class++;
            }
            new_transitions.insert(std::pair<std::pair<int, char>,
                    int>(std::make_pair(from_state, itt->first.second), to_state));
        }
        int new_initial = 1;
        for (const std::set<int> &class_p : P) {
            if (class_p.count(initial_state_) != 0) break;
            else new_initial++;
        }
        std::set<int> new_accepting;
        int current_class = 1;
        for (const std::set<int> &class_p : P) {
            std::string token;
            for (const int state : accepting_states_) {
                if (class_p.count(state) != 0) {
                    token = tokens_[state];
                    new_accepting.insert(current_class);
                }
            }
            if (!token.empty())
                new_tokens.insert(make_pair(current_class, token));
            current_class++;
        }
        return DFA(new_size, alphabet_, new_transitions, new_initial, new_accepting, new_tokens);
    }

    int DFA::Compute(const std::string &string_input) {
        int current_state = initial_state_;
        for (const char c : string_input) {
            current_state = transitions_[std::make_pair(current_state, c)];
            if (current_state == -1) return -1;
        }
        std::cout << std::endl;
        return current_state;
    }

    int DFA::Compute(int state, char c) {
        if (!transitions_.count(std::make_pair(state, c)))
            return -1;
        return transitions_[std::make_pair(state, c)];
    }

    int DFA::initial_state() {
        return initial_state_;
    }

    const std::set<int> &DFA::accepting_states() {
        return accepting_states_;
    }

    const std::map<int, std::string> &DFA::tokens() {
        return tokens_;
    }

    std::string DFA::ComputeString(const std::string &str) {
        int result = Compute(str);
        return tokens_.count(result) ? tokens_[result] : "";
    }
} //namespace compiler::automata
