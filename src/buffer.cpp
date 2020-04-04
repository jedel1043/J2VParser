#include "J2VParser/buffer.h"

#include <string>
#include <iostream>


namespace compiler::io_buffer {

    char EOF_char = 0x7F;

    TextSourceBuffer::TextSourceBuffer(const std::string &fname) {
        this->file_name_ = fname;
        file_.open(this->file_name_, std::fstream::in);
        if (!file_.good()) AbortTranslation(error::SourceFileOpenFailed);
        FetchLine();
    }

    void TextSourceBuffer::FetchLine() {
        if (file_.eof()) {
            current_char_ = nullptr;
            return;
        }
        file_.getline(&current_line_[0], kMaxBufferSize);
        current_char_ = current_line_.begin();
    }

    bool TextSourceBuffer::IsEOL() {
        return current_char_ == current_line_.end() || *current_char_ == 0;
    }

    char TextSourceBuffer::FetchChar() {
        if (current_char_ == nullptr)
            return EOF_char;
        else if (IsEOL()) {
            current_char_i_ = 1;
            while (current_char_ != nullptr && IsEOL()) {
                FetchLine();
                ++current_line_i_;
            }
            if (current_char_ == nullptr) {
                current_char_i_ = 0;
                return EOF_char;
            }
            return *current_char_;
        } else {
            ++current_char_;
            ++current_char_i_;
            return *current_char_;
        }
    }

    char TextSourceBuffer::PutBackChar() {
        --current_char_;
        --current_char_i_;
        return *current_char_;
    }


//    void ListPrinterBuffer::PutLine() {
//        text[kMaxPrintLineLength] = '\0';
//        std::cout << text << std::endl;
//        text[0] = '\0';
//        ++line_counter_;
//    }
} //namespace compiler::io_buffer
