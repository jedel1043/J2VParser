#ifndef J2VP_ITEMS_H
#define J2VP_ITEMS_H

#include <string>
#include <vector>
#include <iostream>
#include <set>

namespace J2VParser::parsers {

    struct LR0Item {
        std::string variable_;
        std::vector<std::string> rule_ = {};
        int point_{};

        LR0Item(std::string variable, std::vector<std::string> rule, int point);

        LR0Item(std::string var_input, std::vector<std::string> rule_input);

        bool operator==(const LR0Item &obj) const;

        bool operator<(const LR0Item &obj) const;

        [[nodiscard]] const std::string &PointSymbol() const;

        [[nodiscard]] std::string GetFullString() const;

        [[nodiscard]] std::string to_string() const;

        friend std::ostream &operator<<(std::ostream &ostream, const LR0Item &obj);

    };

    std::ostream &operator<<(std::ostream &ostream, const LR0Item &obj);

    struct LR1Item {
        LR0Item lr0_item_;
        std::string token_;

        bool operator<(const LR1Item &obj) const;

        friend std::ostream &operator<<(std::ostream &ostream, const LR1Item &obj);
    };

    std::ostream &operator<<(std::ostream &ostream, const LR1Item &obj);
} // namespace J2VParser::parsers
#endif //J2VP_ITEMS_H
