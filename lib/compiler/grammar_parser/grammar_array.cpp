#include <algorithm>
#include "compiler/grammar_parser/grammar_array.h"

std::ostream &operator<<(std::ostream &ostream, const grammar_array &obj) {
    ostream << "Axiom:\t" << obj.axiom << std::endl;
    ostream << "Rules:" << std::endl;
    int i=0;
    for(const auto& rules : obj.rules_array){
        for(const auto& rule : rules.second){
            ostream << "\t" << i << ".-\t" << rules.first << "\t:\t";
            for(const std::string& word : rule){
                ostream << word << " ";
            }
            ostream << std::endl;
            i++;
        }
    }
    ostream << std::endl << "Terminal symbols:\t{";
    for(const auto& symbol : obj.terminals)
        ostream << symbol << " ";
    ostream << "}" << std::endl;
    ostream << std::endl << "Non-terminal symbols:\t{";
    for(const auto& symbol : obj.non_terminals)
        ostream << symbol << " ";
    ostream << "}" << std::endl;
    return ostream;
}

std::set<std::vector<std::string>> grammar_array::operator[](const std::string& index){
    return rules_array.at(index);
}

//insercion a la coleccion de reglas.
void grammar_array::insert(const std::string &variable, const std::vector<std::string> &rule) {
    non_terminals.insert(variable);
    if(rules_array.count(variable))
        rules_array[variable].insert(rule);
    else
        rules_array.insert({variable, {rule}});

    update_terminals();

    if(axiom.empty())
        axiom = variable;

    rules_index[{variable, rule}] = rules_index.size();
}

void grammar_array::update_terminals() {
    terminals.clear();
    for(const auto& rules : rules_array){
        for(const auto& rule : rules.second){
            std::set<std::string> saver(rule.begin(), rule.end());
            std::set_difference(saver.begin(), saver.end(), non_terminals.begin(), non_terminals.end(),
                    std::inserter(terminals, terminals.end()));
        }
    }
    terminals.erase("#");
}

std::set<std::pair<std::string, std::vector<std::string>>> grammar_array::get_right_sides(const std::string &variable) {
    std::set<std::pair<std::string, std::vector<std::string>>> left_vars;
    for(const auto& rules : rules_array){
        for(const auto& rule : rules.second){
            if(std::find(rule.begin(), rule.end(), variable) != rule.end())
                left_vars.insert({rules.first, rule});
        }
    }
    return left_vars;
}

std::set<std::vector<std::string>> grammar_array::get_rules(const std::string &variable){
    return rules_array.at(variable);
}

bool grammar_array::can_derive_epsilon(const std::string &variable){
    if (non_terminals.count(variable)){
        for(const auto& rule : get_rules(variable)){
            if(rule.front() == "#" || can_derive_epsilon(rule.front()))
                return true;
        }
        return false;
    }
    return false;
}

std::set<std::string> grammar_array::first(const std::vector<std::string>& expression_vector){
    std::string front_elem = expression_vector.front();
    if(terminals.count(front_elem) || front_elem == "#" )
        return {front_elem};
    std::set<std::string> first_result;
    for (const auto& str : expression_vector){
        auto first_saver = first(str);
        first_result.insert(first_saver.begin(), first_saver.end());
        if(!first_saver.count("#")) {
            first_result.erase("#");
            break;
        }
    }
    return first_result;
}

std::set<std::string> grammar_array::first(const std::string& expression){
    if(terminals.count(expression) || expression == "#")
        return {expression};
    std::set<std::string> first_result;
    std::set<std::vector<std::string>> rules = get_rules(expression);
    if(can_derive_epsilon(expression)) {
        first_result.insert("#");
        rules.erase({"#"});
    }
    for(const auto& rule : rules){
        int i = 0;
        for(const auto& var : rule){
            if(var == expression)
                continue;
            std::set<std::string> aux = first(var);
            first_result.insert(aux.begin(), aux.end());
            if(!can_derive_epsilon(var))
                break;
            i++;
        }
        if(i == rule.size())
            first_result.insert("#");
    }
    return first_result;
}

std::set<std::string> grammar_array::follow(const std::string& expression, std::set<std::string>& calculated){
    if(calculated.count(expression))
      return {};
    std::set<std::string> follow_result;
    calculated.insert(expression);
    if(expression == axiom)
        follow_result.insert("$");

    for (const auto& rule : get_right_sides(expression)){
        if(rule.second.back() != expression){
            std::vector<std::string> rule_saver = rule.second;
            auto next_expr = ++std::find(rule_saver.begin(), rule_saver.end(), expression);
            rule_saver.erase(rule_saver.begin(), next_expr);
            std::set<std::string> first_saver = first(rule_saver);
            follow_result.insert(first_saver.begin(), first_saver.end());
            if(first_saver.count("#")){
                follow_result.erase("#");
                std::set<std::string> rec_follow = follow(rule.first, calculated);
                follow_result.insert(rec_follow.begin(), rec_follow.end());
            }
        }
        else{
            std::set<std::string> rec_follow = follow(rule.first, calculated);
            follow_result.insert(rec_follow.begin(), rec_follow.end());
        }
    }
    return follow_result;
}

std::set<std::string> grammar_array::follow(const std::string &expression) {
    std::set<std::string> empty_set;
    return follow(expression, empty_set);
}


//std::set<std::string> grammar_array::follow(const std::string& expression, std::set<std::string>& calculated){
//    std::set<std::string> follow_result;
//
//    if(calculated.find(expression) != calculated.end())
//      return follow_result;
//    else
//      calculated.insert(expression);
//
//    if(expression == axiom){
//        follow_result.insert("$");
//    }
//    auto right_result = get_right_sides(expression);
//    for(const auto& rule_f : right_result){
//      string rule = collection[rule_f];
//
//      for(int i=0; i<(int)rule.length(); i++){
//        if(string(1, rule[i]) == expression){
//          set<pair<int, string>> e_result;
//          if(i != (rule.length()-1)){
//            set<string> first_result;
//            first_result = first(string(1, rule[i+1]));
//            for(const string& first_elem : first_result){
//              if(first_elem != "#"){
//                follow_result.insert(first_elem);
//              }else{
//                e_result = get_right_sides_e(expression);
//              }
//            }
//          }else{
//            e_result = get_right_sides_e(expression);
//          }
//
//          for(const auto& e_result_elem : e_result){
//            set<string> follow_e = follow(e_result_elem.second, calculated);
//            for(const string& e_r : follow_e)
//              follow_result.insert(e_r);
//          }
//        }
//      }
//    }
//    return follow_result;
//}

const std::string &grammar_array::get_axiom() const {
    return axiom;
}

const std::set<std::string> &grammar_array::get_non_terminals() const {
    return non_terminals;
}

const std::set<std::string> &grammar_array::get_terminals() const {
    return terminals;
}

int grammar_array::get_rule_index(const std::string &variable, const std::vector<std::string> &rule) {
    return rules_index.at({variable, rule});
}

grammar_array::grammar_array() {}










