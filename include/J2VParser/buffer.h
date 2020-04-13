/*!
* @file         buffer.h
* @brief        Classes for the file control and listing utils.
* @details      The Template class BasicTextSourceBuffer is the base class for
*               reading file streams.
*/

#ifndef J2VPARSER_BUFFER_H
#define J2VPARSER_BUFFER_H

#include <string>
#include <fstream>
#include <array>
#include <tuple>
#include <algorithm>

#include "J2VParser/error.h"

namespace J2VParser::io_buffer {
/*!
 * @brief   Template class for the input buffer.
 * @details The class template BasicTextSourceBuffer reads an input file
 * char by char, according to the datatype specified. The class also
 * stores the current read position on the file, allowing to obtain
 * debug information when throwing parsing errors, for example.
 * Two typedefs for common character types are provided
 * - TextSourceBuffer (BasicTextSourceBuffer<char>)
 * - WTextSourceBuffer (BasicTextSourceBuffer<wchar_t>)
 */
    template<class CharT>
    class BasicTextSourceBuffer {
    private:
        using line = std::basic_string<CharT>;
        using buffer_info = std::tuple<line, int, int>;
        using line_iterator = typename line::const_iterator;

        static const CharT eofChar = std::char_traits<CharT>::eof();
        std::basic_ifstream<CharT> buffer_;    //!< Input stream.
        line current_line_;    //!< Current line read from the input stream.
        line_iterator current_char_;    //!< Current character read from the input stream.
        int current_line_i_;    //!< Vertical position of TextSourceBuffer at the file input (line number).
        int current_char_i_;    //!< Horizontal position of TextSourceBuffer at the file input (column number).

    public:
        /*!
        * @brief    Constructor for TextSourceBuffer
        * @param    fname File name of the input stream.
        */
        explicit BasicTextSourceBuffer(const std::string &fname) :
                current_line_i_(0), current_char_i_(0) {
            buffer_.open(fname, std::fstream::in);
            if (!buffer_) AbortTranslation(error::SourceFileOpenFailed);
            current_line_ = "\n";
            current_char_ = current_line_.begin();
        }

        /*!
        * @brief   Return the current character from the text buffer.
        * @return  The current caracter on the text buffer.
        */
        CharT GetChar() const {
            return !eof() ? *current_char_ : eofChar;
        };

        /*!
         * @brief   Get a new character from the text buffer.
         * @details  If the next character of text buffer is the null character, fetch a new line and
         * update the value of the text buffer.
         * @see FetchLine()
         * @see PutBackChar()
         * @return  The next caracter on the text buffer.
        */
        CharT FetchChar() {
            if (eof())
                return eofChar;
            if (++current_char_ == current_line_.end()) {
                if (buffer_.eof()) {
                    ++current_char_i_;
                    return eofChar;
                }
                FetchLine();
            } else
                ++current_char_i_;
            return *current_char_;
        };

        /*!
         * @brief   Undo fetching a char.
         * @warning Calling PutBackChar() after FetchLine() will cause undefined behavior.
         * @warning Calling PutBackChar() after initialization will cause undefined behavior.
         * @return  The previous character on the text buffer.
         */
        CharT PutBackChar() {
            --current_char_;
            --current_char_i_;
            return *current_char_;
        };

        /*!
         * @brief   Get info of the current buffer status.
         * @return  A tuple containing current_line_, current_line_i
         * and current_char_i.
         */
        buffer_info GetBufferStatus() const {
            return {current_line_, current_line_i_, current_char_i_};
        };

        [[nodiscard]] bool eof() const {
            return current_char_ == current_line_.end() && buffer_.eof();
        }

    private:
        /*!
         * @brief Get a new line from the text buffer.
         * @details current_char_ will always be set to the beginning of the fetched line
         * excluding when fetching the last line and subsequent calls, where FetchLine() will set
         * current_char_ = nullptr .
         */
        void FetchLine() {
            if (buffer_.eof())
                return;
            std::getline(buffer_, current_line_);
            if (!buffer_.eof())
                current_line_ += '\n';
            ++current_line_i_;
            current_char_i_ = 1;
            current_char_ = current_line_.begin();
        }

    };

    using TextSourceBuffer = BasicTextSourceBuffer<char>;
    using WTextSourceBuffer = BasicTextSourceBuffer<wchar_t>;

//
//        class OutputStreamBuffer {
//
//        protected:
//            char text[kMaxBufferSize + 16];
//
//            virtual void PutLine() = 0;
//
//            virtual void PutLine(const std::string textOutput) {
//                strcpy(text, textOutput);
//                PutLine();
//            }
//
//        };
//
//        class ListPrinterBuffer : public OutputStreamBuffer {
//            std::string source_file_name_;
//            char date_[26];
//            int page_number_;
//            int line_counter_;
//
//        protected:
//            void PutLine() override;
//
//        public:
//
//            void PutLine(const char *textOutput) override {
//                OutputStreamBuffer::PutLine(textOutput);
//            }
//
//            void PutLine(const char *textOutput, int lineNumber) {
//                sprintf(text, "%4d : %s", lineNumber, textOutput);
//                PutLine();
//            }
//        };
//
//        extern ListPrinterBuffer list;
} //namespace compiler
#endif //J2VPARSER_BUFFER_H
