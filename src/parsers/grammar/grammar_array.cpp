#include "J2VParser/parsers/grammar/grammar_array.h"

#include <algorithm>

namespace J2VParser::grammar {


    GrammarArray::VarListSet GrammarArray::operator[](const GrammarArray::Var &index) const {
        return rules_array_.at(index);
    }

    void GrammarArray::InsertRule(const Var &variable, const VarList &rule) {
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
                VarSet saver(rule.begin(), rule.end());
                std::set_difference(saver.begin(), saver.end(), non_terminals_.begin(), non_terminals_.end(),
                                    std::inserter(terminals_, terminals_.end()));
            }
        }
        terminals_.erase("#");
    }

    void GrammarArray::InsertTerminal(const Var &new_symbol) {
        terminals_.insert(new_symbol);
    }

    GrammarArray::RuleSet GrammarArray::GetRightSides(const Var &variable) const {
        RuleSet left_vars;
        for (const auto &rules : rules_array_) {
            for (auto rule : rules.second) {
                if (std::find(rule.begin(), rule.end(), variable) != rule.end())
                    left_vars.insert({rules.first, rule});
            }
        }
        return left_vars;
    }

    GrammarArray::VarListSet GrammarArray::GetVariableRules(const Var &variable) const {
        return rules_array_.at(variable);
    }

    bool GrammarArray::canGenerateEpsilon(const Var &variable, VarSet &calculated) const {
        if (calculated.count(variable))
            return false;
        calculated.insert(variable);
        if (non_terminals_.count(variable)) {
            for (const auto &rule : GetVariableRules(variable)) {
                if (rule.front() == "#" || canGenerateEpsilon(rule.front(), calculated))
                    return true;
            }
            return false;
        }
        return false;
    }

    bool GrammarArray::canGenerateEpsilon(const Var &variable) const {
        VarSet empty_set;
        return canGenerateEpsilon(variable, empty_set);
    }

    GrammarArray::VarSet GrammarArray::First(const VarList &expression_vector) const {
        VarSet calculated = {};
        Var front_elem = expression_vector.front();
        if (terminals_.count(front_elem) || front_elem == "#")
            return {front_elem};
        VarSet first_result;
        for (const auto &str : expression_vector) {
            auto first_saver = First(str, calculated);
            first_result.insert(first_saver.begin(), first_saver.end());
            if (!first_saver.count("#")) {
                first_result.erase("#");
                break;
            }
        }
        return first_result;
    }

    GrammarArray::VarSet GrammarArray::First(const Var &expression, VarSet &calculated) const {
        if (calculated.count(expression))
            return {};
        if (terminals_.count(expression) || expression == "#")
            return {expression};
        calculated.insert(expression);
        VarSet first_result;
        VarListSet rules = GetVariableRules(expression);
        if (canGenerateEpsilon(expression)) {
            first_result.insert("#");
            rules.erase({"#"});
        }
        for (const auto &rule : rules) {
            int i = 0;
            for (const auto &var : rule) {
                VarSet aux = First(var, calculated);
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

    GrammarArray::VarSet GrammarArray::Follow(const Var &variable, VarSet &calculated) const {
        if (calculated.count(variable))
            return {};
        VarSet follow_result;
        calculated.insert(variable);
        if (variable == axiom_)
            follow_result.insert("$");

        for (const auto &rule : GetRightSides(variable)) {
            if (rule.second.back() != variable) {
                VarList rule_saver = rule.second;
                auto next_expr = ++std::find(rule_saver.begin(), rule_saver.end(), variable);
                rule_saver.erase(rule_saver.begin(), next_expr);
                VarSet first_saver = First(rule_saver);
                follow_result.insert(first_saver.begin(), first_saver.end());
                if (first_saver.count("#")) {
                    follow_result.erase("#");
                    VarSet rec_follow = Follow(rule.first, calculated);
                    follow_result.insert(rec_follow.begin(), rec_follow.end());
                }
            } else {
                std::set<std::string> rec_follow = Follow(rule.first, calculated);
                follow_result.insert(rec_follow.begin(), rec_follow.end());
            }
        }
        return follow_result;
    }

    GrammarArray::VarSet GrammarArray::Follow(const Var &variable) const {
        VarSet empty_set;
        return Follow(variable, empty_set);
    }

    const GrammarArray::Var &GrammarArray::axiom() const {
        return axiom_;
    }

    const GrammarArray::VarSet &GrammarArray::non_terminals() const {
        return non_terminals_;
    }

    const GrammarArray::VarSet &GrammarArray::terminals() const {
        return terminals_;
    }

    int GrammarArray::GetRuleIndex(const Var &variable, const VarList &rule) const {
        return index_rule_.at({variable, rule});
    }

    GrammarArray::Rule GrammarArray::GetRuleFromIndex(int index) const {
        return index_rule_.at(index);
    }

    GrammarArray GrammarArray::GetAugmentedGrammar(Var new_axiom) const {
        GrammarArray new_grammar = *this;
        if (new_axiom.empty())
            new_axiom = axiom_ + "_p";

        while (terminals_.count(new_axiom))
            new_axiom += "_";
        new_grammar.InsertRule(new_axiom, {axiom_});
        new_grammar.set_axiom(new_axiom);
        new_grammar.terminals_.insert("$");

        return new_grammar;
    }

    void GrammarArray::set_axiom(const Var &new_axiom) {
        GrammarArray::axiom_ = new_axiom;
    }

    std::ostream &operator<<(std::ostream &ostream, const GrammarArray &obj) {
        ostream << "Axiom:\t" << obj.axiom_ << std::endl;
        ostream << "Rules:" << std::endl;
        for (int i = 0; i < obj.index_rule_.size(); ++i) {
            auto rule = obj.GetRuleFromIndex(i);
            ostream << "\t" << i << ".-\t" << rule.first << "\t:\t";
            for (const std::string &word : rule.second) {
                ostream << word << " ";
            }
            ostream << std::endl;
        }
        ostream << std::endl << "Terminal symbols:\t{";
        std::string out;
        for (const auto &symbol : obj.terminals_)
            out += symbol + ", ";
        out.pop_back();
        out.pop_back();
        ostream << out << "}" << std::endl;
        ostream << std::endl << "Non-terminal symbols:\t{";
        out = "";
        for (const auto &symbol : obj.non_terminals_)
            out += symbol + ", ";
        out.pop_back();
        out.pop_back();
        ostream << out << "}" << std::endl;
        return ostream;
    }
} // namespace compiler::grammar
