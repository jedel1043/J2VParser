#include "parsers/grammar_utils/grammar_array.h"

#include <algorithm>

namespace compiler::grammar {

    std::ostream &operator<<(std::ostream &ostream, const GrammarArray &obj) {
        ostream << "Axiom:\t" << obj.axiom_ << std::endl;
        ostream << "Rules:" << std::endl;
        int i = 0;
        for (const auto &rules : obj.rules_array_) {
            for (const auto &rule : rules.second) {
                ostream << "\t" << i << ".-\t" << rules.first << "\t:\t";
                for (const std::string &word : rule) {
                    ostream << word << " ";
                }
                ostream << std::endl;
                i++;
            }
        }
        ostream << std::endl << "Terminal symbols:\t{";
        for (const auto &symbol : obj.terminals_)
            ostream << symbol << " ";
        ostream << "}" << std::endl;
        ostream << std::endl << "Non-terminal symbols:\t{";
        for (const auto &symbol : obj.non_terminals_)
            ostream << symbol << " ";
        ostream << "}" << std::endl;
        return ostream;
    }

    std::set<std::vector<std::string>> GrammarArray::operator[](const std::string &index) const {
        return rules_array_.at(index);
    }

//insercion a la coleccion de reglas.
    void GrammarArray::InsertRule(const std::string &variable, const std::vector<std::string> &rule) {
        non_terminals_.insert(variable);
        if (rules_array_.count(variable))
            rules_array_[variable].insert(rule);
        else
            rules_array_.insert({variable, {rule}});

        UpdateTerminals();

        if (axiom_.empty())
            axiom_ = variable;

        index_rule_.insert({variable, rule}, index_rule_.size());
    }

    void GrammarArray::UpdateTerminals() {
        terminals_.clear();
        for (const auto &rules : rules_array_) {
            for (const auto &rule : rules.second) {
                std::set<std::string> saver(rule.begin(), rule.end());
                std::set_difference(saver.begin(), saver.end(), non_terminals_.begin(), non_terminals_.end(),
                                    std::inserter(terminals_, terminals_.end()));
            }
        }
        terminals_.erase("#");
    }

    std::set<std::pair<std::string, std::vector<std::string>>>
    GrammarArray::GetRightSides(const std::string &variable) {
        std::set<std::pair<std::string, std::vector<std::string>>> left_vars;
        for (const auto &rules : rules_array_) {
            for (const auto &rule : rules.second) {
                if (std::find(rule.begin(), rule.end(), variable) != rule.end())
                    left_vars.insert({rules.first, rule});
            }
        }
        return left_vars;
    }

    std::set<std::vector<std::string>> GrammarArray::GetVariableRules(const std::string &variable) {
        return rules_array_.at(variable);
    }

    bool GrammarArray::canGenerateEpsilon(const std::string &variable) {
        if (non_terminals_.count(variable)) {
            for (const auto &rule : GetVariableRules(variable)) {
                if (rule.front() == "#" || canGenerateEpsilon(rule.front()))
                    return true;
            }
            return false;
        }
        return false;
    }

    std::set<std::string> GrammarArray::First(const std::vector<std::string> &expression_vector) {
        std::string front_elem = expression_vector.front();
        if (terminals_.count(front_elem) || front_elem == "#")
            return {front_elem};
        std::set<std::string> first_result;
        for (const auto &str : expression_vector) {
            auto first_saver = First(str);
            first_result.insert(first_saver.begin(), first_saver.end());
            if (!first_saver.count("#")) {
                first_result.erase("#");
                break;
            }
        }
        return first_result;
    }

    std::set<std::string> GrammarArray::First(const std::string &expression) {
        if (terminals_.count(expression) || expression == "#")
            return {expression};
        std::set<std::string> first_result;
        std::set<std::vector<std::string>> rules = GetVariableRules(expression);
        if (canGenerateEpsilon(expression)) {
            first_result.insert("#");
            rules.erase({"#"});
        }
        for (const auto &rule : rules) {
            int i = 0;
            for (const auto &var : rule) {
                if (var == expression)
                    continue;
                std::set<std::string> aux = First(var);
                first_result.insert(aux.begin(), aux.end());
                if (!canGenerateEpsilon(var))
                    break;
                i++;
            }
            if (i == rule.size())
                first_result.insert("#");
        }
        return first_result;
    }

    std::set<std::string> GrammarArray::Follow(const std::string &variable, std::set<std::string> &calculated) {
        if (calculated.count(variable))
            return {};
        std::set<std::string> follow_result;
        calculated.insert(variable);
        if (variable == axiom_)
            follow_result.insert("$");

        for (const auto &rule : GetRightSides(variable)) {
            if (rule.second.back() != variable) {
                std::vector<std::string> rule_saver = rule.second;
                auto next_expr = ++std::find(rule_saver.begin(), rule_saver.end(), variable);
                rule_saver.erase(rule_saver.begin(), next_expr);
                std::set<std::string> first_saver = First(rule_saver);
                follow_result.insert(first_saver.begin(), first_saver.end());
                if (first_saver.count("#")) {
                    follow_result.erase("#");
                    std::set<std::string> rec_follow = Follow(rule.first, calculated);
                    follow_result.insert(rec_follow.begin(), rec_follow.end());
                }
            } else {
                std::set<std::string> rec_follow = Follow(rule.first, calculated);
                follow_result.insert(rec_follow.begin(), rec_follow.end());
            }
        }
        return follow_result;
    }

    std::set<std::string> GrammarArray::Follow(const std::string &variable) {
        std::set<std::string> empty_set;
        return Follow(variable, empty_set);
    }

    const std::string &GrammarArray::axiom() const {
        return axiom_;
    }

    const std::set<std::string> &GrammarArray::non_terminals() const {
        return non_terminals_;
    }

    const std::set<std::string> &GrammarArray::terminals() const {
        return terminals_;
    }

    int GrammarArray::GetRuleIndex(const std::string &variable, const std::vector<std::string> &rule) {
        return index_rule_[{variable, rule}];
    }

    std::pair<std::string, std::vector<std::string>> GrammarArray::GetRuleFromIndex(int index) {
        return index_rule_[index];
    }

    void GrammarArray::ToAugmentedGrammar(std::string new_axiom) {
        if (new_axiom.empty())
            new_axiom = axiom_ + "_p";

        while (terminals_.count(new_axiom))
            new_axiom += "_";
        InsertRule(new_axiom, {axiom_, "$"});
        axiom_ = new_axiom;
    }

    GrammarArray GrammarArray::GetAugmentedGrammar(std::string new_axiom) {
        GrammarArray new_grammar = *this;
        if (new_axiom.empty())
            new_axiom = axiom_ + "_p";

        while (terminals_.count(new_axiom))
            new_axiom += "_";
        new_grammar.InsertRule(new_axiom, {axiom_, "$"});
        new_grammar.set_axiom(new_axiom);

        return new_grammar;
    }

    void GrammarArray::set_axiom(const std::string &new_axiom) {
        GrammarArray::axiom_ = new_axiom;
    }

    GrammarArray::GrammarArray() = default;
} // namespace compiler::grammar
