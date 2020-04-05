#include "J2VParser/parsers/items.h"

namespace J2VParser::parsers {

    LR0Item::LR0Item(std::string variable, std::vector<std::string> rule, int point) :
            variable_(std::move(variable)), rule_(std::move(rule)), point_(point) {}

    std::string LR0Item::GetFullString() const {
        auto result = variable_;
        int i = 0;
        for (const auto &symbol : rule_) {
            if (point_ == i++)
                result += ".";
            result += symbol;
        }
        if (point_ == rule_.size())
            result += ".";
        return result;
    }

    std::string LR0Item::to_string() const {
        std::string output;
        output += ".- " + variable_ + " -> ";
        int i = 0;
        for (const auto &str : rule_) {
            if (point_ == i)
                output += ". ";
            output += str + " ";
            i++;
        }
        output.pop_back();
        if (point_ == rule_.size())
            output += " .";
        return output;
    }

    LR0Item::LR0Item(std::string var_input, std::vector<std::string> rule_input) :
            variable_(std::move(var_input)), rule_(std::move(rule_input)), point_(0) {}

    bool LR0Item::operator==(const LR0Item &obj) const {
        return GetFullString() == obj.GetFullString();
    }

    bool LR0Item::operator<(const LR0Item &obj) const {
        return GetFullString() < obj.GetFullString();
    }

    const std::string &LR0Item::PointSymbol() const { return rule_.at(point_); }

    std::ostream &operator<<(std::ostream &ostream, const LR0Item &obj) {
        std::string output;
        output += ".- " + obj.variable_ + " -> ";
        int i = 0;
        for (const auto &str : obj.rule_) {
            if (obj.point_ == i)
                output += ". ";
            output += str + " ";
            i++;
        }
        output.pop_back();
        if (obj.point_ == obj.rule_.size())
            output += " .";
        ostream << output;
        return ostream;
    }

    bool LR1Item::operator<(const LR1Item &obj) const {
        return lr0_item_.GetFullString() + token_ < obj.lr0_item_.GetFullString() + obj.token_;
    }

    std::ostream &operator<<(std::ostream &ostream, const LR1Item &obj) {
        ostream << obj.lr0_item_ << " [" << obj.token_ << "]";
        return ostream;
    }
} // namespace J2VParser::parsers

