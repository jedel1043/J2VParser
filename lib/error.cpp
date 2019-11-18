#include "error.h"

#include <iostream>

#include "common.h"

namespace compiler::error {

    void AbortTranslation(AbortCode error_code) {
        std::cerr << "Fatal translation error: " << abort_msg[-error_code] << std::endl;
        exit(error_code);
    }

    void SyntaxError(AbortCode syntax_error_code) {
        std::cerr << "       ";
        for (int i = 0; i < input_position; i++)
            std::cerr << " ";
        std::cerr << "^~~~" << std::endl;
        std::cerr << "Fatal syntax error (line " << current_line_number << " col " << input_position << "): "
                  << abort_msg[-syntax_error_code]
                  << std::endl;
        exit(syntax_error_code);
    }
} //namespace compiler::error
