/*!
 * @file regex_scanner.h
 * @brief Definitions of components needed to scan a file with regex expressions.
 */

#ifndef J2VP_REGEX_SCANNER_H
#define J2VP_REGEX_SCANNER_H

#include <array>

#include "J2VParser/buffer.h"

namespace J2VParser::regex {

    enum class TokenCodeRegex {
        EOS = 1,          // END OF STREAM
        ANY,              // .
        AT_BOL,           // ^
        AT_EOL,           // $
        CCL_END,          // ]
        CCL_START,        // [
        CLOSE_CURLY,      // }
        CLOSE_PAREN,      // )
        CLOSURE,          // *
        DASH,             // -
        END_OF_INPUT,     // EOF
        L,                // LITERAL CHARACTER
        OPEN_CURLY,       // {
        OPEN_PAREN,       // (
        OPTIONAL,         // ?
        OR,               // |
        PLUS_CLOSURE,     // +
        SEMICOLON,        // +
        COLON,            // +
        TOKEN_ERROR       // Error
    };

    struct RegexToken {
        TokenCodeRegex token;
        char lexeme;

        bool operator==(const TokenCodeRegex &t) const { return token == t; };
        bool operator!=(const TokenCodeRegex &t) const { return token != t; };
    };

    class RegexScanner {
    private:
        io_buffer::TextSourceBuffer &source_buffer_;
        bool in_quote_;
        bool escape_;
        RegexToken current_token_;
        std::array<TokenCodeRegex, 128> char_code_map_{};

    public:
        explicit RegexScanner(io_buffer::TextSourceBuffer &buffer);

        RegexToken current_token() { return current_token_; }

        void SkipWhiteSpace();

        virtual RegexToken GetNextToken();

        [[nodiscard]] io_buffer::TextSourceBuffer &getSourceBuffer() const;

    };
}// namespace compiler::regex
#endif //J2VP_REGEX_SCANNER_H
