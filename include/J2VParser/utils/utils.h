#ifndef J2VPARSER_UTILS_H
#define J2VPARSER_UTILS_H

#include <string>
#include <sstream>

namespace J2VParser::utils {
    template<class Container>
    std::string ElemDivider(const Container &input_obj, const std::string &separator = ", ") {
        std::stringstream output;
        std::string saver;
        for (const auto &elem : input_obj) {
            output << saver << elem;
            saver = separator;
        }
        return output.str();
    }
} // namespace J2VParser::utils

#endif //J2VPARSER_UTILS_H
