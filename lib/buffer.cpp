#include "buffer.h"

#include <string>
#include <ctime>
#include <iostream>

#include "common.h"

namespace compiler::io_buffer {

    char EOF_char = 0x7F;
    const bool list_flag = false;

    InputStreamBuffer::InputStreamBuffer(const std::string &fname, error::AbortCode ac) {
        this->file_name_ = fname;
        file_.open(this->file_name_, std::fstream::in);
        if (!file_.good()) AbortTranslation(ac);
    }

    char InputStreamBuffer::FetchChar() {
        char c;
        if (*current_char_ == EOF_char) return EOF_char;
        else if (*current_char_ == '\0') c = GetLine();
        else {
            ++current_char_;
            ++input_position;
            c = *current_char_;
        }

        if (c == '\t') input_position += kTabSize - (input_position % kTabSize);
        return c;
    }

    char InputStreamBuffer::PutBackChar() {

        --current_char_;
        --input_position;
        return *current_char_;
    }

    TextSourceBuffer::TextSourceBuffer(const std::string &fname) : InputStreamBuffer(fname, error::SourceFileOpenFailed) {
        if (list_flag) list.Init(fname);
        GetLine();
    }

    char TextSourceBuffer::GetLine() {
        if (file_.eof()) current_char_ = &EOF_char;
        else {
            file_.getline(text_, kMaxBufferSize);
            current_char_ = text_;

            if (list_flag) list.PutLine(text_, ++current_line_number);
        }

        input_position = 0;
        return *current_char_;
    }

    ListPrinterBuffer list;

    void ListPrinterBuffer::PrintPageHeader() {
        std::cout << "\fPage " << ++page_number_ << "  " << source_file_name_ << "  " << date_ << std::endl << std::endl;
        line_counter_ = 0;
    }

    void ListPrinterBuffer::Init(const std::string &fname) {
        text[0] = '\0';
        page_number_ = 0;
        source_file_name_ = fname;
        time_t timer;
        time(&timer);
        strcpy(date_, asctime(localtime(&timer)));
        date_[strlen(date_) - 1] = '\0';
        PrintPageHeader();
    }

    void ListPrinterBuffer::PutLine() {
        if (list_flag && (line_counter_ == kMaxLinesPerPage)) PrintPageHeader();
        text[kMaxPrintLineLength] = '\0';
        std::cout << text << std::endl;
        text[0] = '\0';
        ++line_counter_;
    }
} //namespace compiler::io_buffer
