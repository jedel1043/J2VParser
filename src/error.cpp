#include "J2VParser/error.h"

#include <iostream>

namespace J2VParser::error {

    void AbortTranslation(AbortCode error_code) {
        std::cerr << "Fatal translation error: " << abort_msg[-error_code] << std::endl;
        exit(error_code);
    }

    void SyntaxError(AbortCode syntax_error_code, std::tuple<std::string, int, int> vals) {
        std::string err_str = std::get<0>(vals);
        unsigned long long int char_err_i = std::get<1>(vals);
        int line_err_i = std::get<2>(vals);
        std::cerr << err_str << std::endl;
        std::cerr << std::string(char_err_i, ' ');
        std::cerr << "^~~~" << std::endl;
        std::cerr << "Fatal syntax error (line " << line_err_i
                  << " col " << char_err_i << "): "
                  << abort_msg[-syntax_error_code]
                  << std::endl;
        exit(syntax_error_code);
    }
} //namespace compiler::error
