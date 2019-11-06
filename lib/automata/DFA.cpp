#include <map>
#include <set>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <automata/DFA.h>
#include <vector>

using namespace std;

void DFA::printMap(vector<string> maping) {
    int groupingLen = 25;
    int numberOfGroups = (int) alphabet.size() / groupingLen;
    set<set<char>> groups;
    string out;

    for (int i = 0; i <= numberOfGroups; i++) {
        int j = 0;
        auto inicio = alphabet.begin();
        auto fin = alphabet.begin();
        set<char> temp;

        advance(inicio, i * groupingLen);

        if (i == numberOfGroups) {
            fin = alphabet.end();
        } else {
            advance(fin, (i + 1) * groupingLen);
        }

        temp.insert(inicio, fin);

        printf("     |");
        for (const char c : temp) {
            string t = maping[j];
            cout << " " << t << " |";
            j = j + 1;
        }

        printf("\n------");
        for (const char c : temp)
            printf("------");
        printf("\n");

        for (int i = 1; i <= size; i++) {
            if (accepting_states.count(i) != 0)
                printf("*");
            else
                printf(" ");
            if (initial_state == i)
                printf(">");
            else
                printf(" ");

            printf("%2d |", i);
            for (const char c : temp) {
                printf(" %3d |", transitions[make_pair(i, c)]);
            }
            printf("\n");
        }
        cout << "\n";
    }
}

void DFA::print() {
    int groupingLen = 25;
    int numberOfGroups = (int) alphabet.size() / groupingLen;
    set<set<char>> groups;
    string out;

    for (int i = 0; i <= numberOfGroups; i++) {
        auto inicio = alphabet.begin();
        auto fin = alphabet.begin();
        set<char> temp;

        advance(inicio, i * groupingLen);

        if (i == numberOfGroups) {
            fin = alphabet.end();
        } else {
            advance(fin, (i + 1) * groupingLen);
        }

        temp.insert(inicio, fin);

        printf("     |");
        for (const char c : temp)
            printf("  %c  |", c);
        printf("\n------");
        for (const char c : temp)
            printf("------");
        printf("\n");

        for (int i = 1; i <= size; i++) {
            if (accepting_states.count(i) != 0)
                printf("*");
            else
                printf(" ");
            if (initial_state == i)
                printf(">");
            else
                printf(" ");

            printf("%2d |", i);
            for (const char c : temp) {
                printf(" %3d |", transitions[make_pair(i, c)]);
            }
            printf("\n");
        }
        cout << "\n";
    }
}


void DFA::toFile(const string &filename) {
    ofstream file(filename);
    file << "     |";
    for (const char c : alphabet) {
        char buffer[7];
        sprintf(buffer, "  %c  |", c);
        file << buffer;
    }
    file << "\n------";
    for (const char c : alphabet)
        file << "------";
    file << "\n";

    for (int i = 1; i <= size; i++) {
        if (accepting_states.count(i) != 0)
            file << "*";
        else
            file << " ";
        if (initial_state == i)
            file << ">";
        else
            file << " ";

        char buffer2[5];
        sprintf(buffer2, "%2d |", i);
        file << buffer2;
        for (const char c : alphabet) {
            char buffer3[7];
            sprintf(buffer3, " %3d |", transitions[make_pair(i, c)]);
            file << buffer3;
        }
        file << "\n";
    }
    file.close();
}

set<int> DFA::inverse_transition(const set<int> &new_states, char c) {
    set<int> result;
    map<pair<int, char>, int>::iterator it;
    for (const int state : new_states) {
        for (it = transitions.begin(); it != transitions.end(); ++it) {
            if (it->second == state && it->first.second == c)
                result.insert(it->first.first);
        }
    }
    return result;
}

DFA DFA::minimize() {
    set<set<int>> P;
    map<int, string> new_tokens;
    P.insert({*accepting_states.begin()});
    for (int state : accepting_states) {
        if (tokens[state] != tokens[*accepting_states.begin()]) {
            auto it = P.begin();
            while (it != P.end()) {
                if (tokens[*((*it).begin())] == tokens[state]) {
                    set<int> a = *it;
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

    set<int> intersection;
    set_difference(states.begin(), states.end(), accepting_states.begin(), accepting_states.end(),
                   inserter(intersection, intersection.end()));
    if (!intersection.empty())
        P.insert(intersection);

    set<set<int>> W;
    W.insert(accepting_states);

    while (!W.empty()) {
        set<set<int>>::iterator it;
        it = W.begin();
        set<int> A = *it;
        W.erase(it);
        for (const char c : alphabet) {
            set<int> X = this->inverse_transition(A, c);
            for (it = P.begin(); it != P.end(); ++it) {
                set<int> Y = *it;
                set<int> XY;
                set<int> Y_X;
                set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), inserter(XY, XY.end()));
                set_difference(Y.begin(), Y.end(), X.begin(), X.end(), inserter(Y_X, Y_X.end()));
                if (XY.empty() || Y_X.empty()) continue;
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
            }
        }
    }

    int new_size = P.size();
    map<pair<int, char>, int> new_transitions;
    map<pair<int, char>, int>::iterator itt;
    for (itt = transitions.begin(); itt != transitions.end(); ++itt) {
        int from_state = 1, to_state = 1;
        bool from_flag = true, to_flag = true;
        for (const set<int> &class_p : P) {
            if (class_p.count(itt->first.first) != 0) from_flag = false;
            else if (from_flag) from_state++;

            if (itt->second == -1) {
                to_state = -1;
                to_flag = false;
            } else if (class_p.count(itt->second) != 0) to_flag = false;
            else if (to_flag) to_state++;
        }
        new_transitions.insert(pair<pair<int, char>, int>(make_pair(from_state, itt->first.second), to_state));
    }
    int new_initial = 1;
    for (const set<int> &class_p : P) {
        if (class_p.count(initial_state) != 0) break;
        else new_initial++;
    }
    set<int> new_accepting;
    int current_class = 1;
    for (const set<int> &class_p : P) {
        string token;
        for (const int state : accepting_states) {
            if (class_p.count(state) != 0) {
                token = tokens[state];
                new_accepting.insert(current_class);
            }
        }
        if (!token.empty())
            new_tokens.insert(make_pair(current_class, token));
        current_class++;
    }
    return DFA(new_size, alphabet, new_transitions, new_initial, new_accepting, new_tokens);
}

int DFA::compute(const string &str) {
    int current_state = initial_state;
    for (const char c : str) {
        current_state = transitions[make_pair(current_state, c)];
        if (current_state == -1) return -1;
    }
    cout << endl;
    return current_state;
}

string DFA::accept(const string &str) {
    int result = this->compute(str);
    return tokens.count(result) ? tokens[result] : "";
}
