#ifndef STRUCT_VARIABLEARRAY_H
#define STRUCT_VARIABLEARRAY_H

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <unordered_map>


class grammar_array {
private:
    std::string axiom;
    std::map<std::string, std::set< std::vector<std::string> > > rules_array;
    std::set<std::string> non_terminals;
    std::set<std::string> terminals;

    std::map<std::pair<std::string, std::vector<std::string>>, int> rules_index;

    std::set<std::string> follow(const std::string& expression, std::set<std::string>& calculated);
    bool can_derive_epsilon(const std::string &variable);
    void update_terminals();
    std::set<std::pair<std::string, std::vector<std::string>>> get_right_sides(const std::string &variable);

public:

    grammar_array();

    void insert(const std::string &variable, const std::vector<std::string> &rule);

    std::set<std::string> first(const std::vector<std::string>& expression_vector);
    std::set<std::string> first(const std::string &expression);

    std::set<std::string> follow(const std::string& expression);

    std::set<std::vector<std::string>> get_rules(const std::string &variable);

    int get_rule_index(const std::string &variable, const std::vector<std::string> &rule);

    [[nodiscard]] const std::string &get_axiom() const;
    [[nodiscard]] const std::set<std::string> &get_non_terminals() const;
    [[nodiscard]] const std::set<std::string> &get_terminals() const;

    friend std::ostream &operator<<(std::ostream &ostream1, const grammar_array &obj);
    std::set<std::vector<std::string>> operator[](const std::string& index);

};

std::ostream &operator<<(std::ostream &ostream, const grammar_array &obj);

#endif //STRUCT_VARIABLEARRAY_H
