#include "J2VParser/analyzers/regex/regex_scanner.h"

#include <cctype>

namespace J2VParser::regex {

    void RegexScanner::SkipWhiteSpace() {
        char c;
        while (std::isspace(c = source_buffer_.GetChar()))
            source_buffer_.FetchChar();
        if (source_buffer_.eof())
            current_token_ = {TokenCodeRegex::EOS, c};
        else
            current_token_ = {kCharTokenMap[c], c};
    }

    RegexScanner::RegexToken RegexScanner::FetchToken() {
        char c = source_buffer_.FetchChar();
        if (source_buffer_.eof())
            current_token_ = {TokenCodeRegex::EOS, source_buffer_.GetChar()};

        return current_token_ = {kCharTokenMap[c], c};
    }

    io_buffer::TextSourceBuffer &RegexScanner::getSourceBuffer() const {
        return source_buffer_;
    }
}// namespace compiler::parsers::regex
