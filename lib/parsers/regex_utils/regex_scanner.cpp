#include "parsers/regex_utils/regex_scanner.h"

#include <cctype>

namespace compiler::regex {

    RegexScanner::RegexScanner(io_buffer::TextSourceBuffer *buffer) :
            source_buffer_(buffer), in_quote_(false), escape_(false), current_token_({TokenCodeRegex::EOS, '\0'}) {
        int i;
        for (i = 0; i <= 31; i++) char_code_map_[i] = TokenCodeRegex::TOKEN_ERROR;
        for (i = ' '; i <= '#'; i++) char_code_map_[i] = TokenCodeRegex::L;
        char_code_map_['$'] = TokenCodeRegex::AT_EOL;
        for (i = '%'; i <= '\''; i++) char_code_map_[i] = TokenCodeRegex::L;
        char_code_map_['('] = TokenCodeRegex::OPEN_PAREN;
        char_code_map_[')'] = TokenCodeRegex::CLOSE_PAREN;
        char_code_map_['*'] = TokenCodeRegex::CLOSURE;
        char_code_map_['+'] = TokenCodeRegex::PLUS_CLOSURE;
        char_code_map_[','] = TokenCodeRegex::L;
        char_code_map_['-'] = TokenCodeRegex::DASH;
        char_code_map_['.'] = TokenCodeRegex::ANY;
        for (i = '/'; i <= '>'; i++) char_code_map_[i] = TokenCodeRegex::L;
        char_code_map_['?'] = TokenCodeRegex::OPTIONAL;
        for (i = '@'; i <= 'Z'; i++) char_code_map_[i] = TokenCodeRegex::L;
        char_code_map_['['] = TokenCodeRegex::CCL_START;
        char_code_map_['\\'] = TokenCodeRegex::L;
        char_code_map_[']'] = TokenCodeRegex::CCL_END;
        char_code_map_['^'] = TokenCodeRegex::AT_BOL;
        for (i = '_'; i <= 'z'; i++) char_code_map_[i] = TokenCodeRegex::L;
        char_code_map_['{'] = TokenCodeRegex::OPEN_CURLY;
        char_code_map_['|'] = TokenCodeRegex::OR;
        char_code_map_['}'] = TokenCodeRegex::CLOSE_CURLY;
        char_code_map_[';'] = TokenCodeRegex::SEMICOLON;
        char_code_map_[':'] = TokenCodeRegex::COLON;
        char_code_map_[127] = TokenCodeRegex::TOKEN_ERROR;
    }

    void RegexScanner::SkipWhiteSpace() {
        char c = source_buffer_->GetChar();
        while (isspace(c) && c != io_buffer::EOF_char)
            c = source_buffer_->FetchChar();
    }

    RegexToken RegexScanner::GetNextToken() {
        char lexeme;

        if (current_token_ == TokenCodeRegex::EOS) {
            if (in_quote_) SyntaxError(error::InvalidNewLine);
            SkipWhiteSpace();
            if (source_buffer_->GetChar() == io_buffer::EOF_char) {
                current_token_ = {TokenCodeRegex::END_OF_INPUT, io_buffer::EOF_char};
                return current_token_;
            }
        }

        if (source_buffer_->GetChar() == '\0') {
            current_token_ = {TokenCodeRegex::EOS, '\0'};
            source_buffer_->FetchChar();
            return current_token_;
        }

        if (source_buffer_->GetChar() == '"') {
            in_quote_ = !in_quote_;
            if (source_buffer_->FetchChar() == io_buffer::EOF_char || source_buffer_->GetChar() == '\0') {
                current_token_ = {TokenCodeRegex::EOS, '\0'};
                return current_token_;
            }
        }

        escape_ = (source_buffer_->GetChar() == '\\');
        if (escape_) source_buffer_->FetchChar();
        if (source_buffer_->GetChar() == io_buffer::EOF_char || source_buffer_->GetChar() == '\0')
            SyntaxError(error::InvalidNewLine);

        if (!in_quote_) {
            if (isspace(source_buffer_->GetChar())) {
                current_token_ = {TokenCodeRegex::EOS, '\0'};
                return current_token_;
            }
            lexeme = source_buffer_->GetChar();
        } else {
            lexeme = source_buffer_->GetChar();
        }
        TokenCodeRegex newToken = (in_quote_ || escape_) ? TokenCodeRegex::L : char_code_map_[lexeme];
        if ((!in_quote_ || !escape_) && isspace(source_buffer_->GetChar())) {
            SkipWhiteSpace();
            return GetNextToken();
        }
        current_token_ = {newToken, lexeme};
        source_buffer_->FetchChar();
        return current_token_;
    }
}// namespace compiler::parsers::regex
