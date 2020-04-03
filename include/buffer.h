/*!
* @file         buffer.h
* @brief        Classes for the file control and listing utils.
* @details      The abstract classes InputBaseBuffer and OutputBaseBuffer are the base classes for
*               reading and writing extern streams. TextSourceBuffer is a simple class, that inherits
*               of InputBaseBuffer, which reads ofstream buffers. ListPrinterBuffer prints a line with
*               its number line and groups lines in pages.
* @author       José Luis Castro García
* @date         2019/09/16 22:40:00
*/

#ifndef IO_BUFFER_H
#define IO_BUFFER_H

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <array>
#include <tuple>
#include <algorithm>

#include "error.h"

namespace compiler::io_buffer {
    extern char EOF_char;  /*!< End-Of-File character. */

    const int kMaxBufferSize = 256; //!< Max number of characters in a line in the file input.
    const int kMaxPrintLineLength = 80;  //!< Max number of characters in a line in the file output.
    const int kMaxLinesPerPage = 50;  //!< Number of lines per page in the file output.

/*!
* @brief   Abstract class for the input buffer.
* @details Reads a file line by line (for this purpose must define the method GetLine()) and gets
*          character by character until it reaches the end of the file. It has the option of putting
*          back a character on the buffer.
*/
    class TextSourceBuffer {
    protected:
        using line_array = std::array<char, kMaxBufferSize>;
        using line_data = std::tuple<std::string, int, int>;

        std::string file_name_;         //!< File name of the input stream.
        std::fstream file_;             //!< Input stream.
        line_array current_line_{};
        line_array::const_iterator current_char_{}; //!< Current character read from the input stream.
        int current_line_i_ = 1;    //!< Vertical position of TextSourceBuffer at the file input (line number).
        int current_char_i_ = 1;         //!< Horizontal position of TextSourceBuffer at the file input (column number).

    public:
        /*!
        * @brief    Constructor for TextSourceBuffer
        * @param    fname File name of the input stream.
        * @param    ac AbortCode in case there is an error opening the file.
        */
        explicit TextSourceBuffer(const std::string &fname);

        /*!
        * @brief   Destructor for TextSourceBuffer. Close the input stream.
        */
        virtual ~TextSourceBuffer() { file_.close(); }

        /*!
        * @brief   Returns the current character from the text buffer.
        * @return  The current caracter.
        */
        char GetChar() const { return current_char_ != nullptr ? *current_char_ : EOF_char; }

            /*!
            * @brief   Gets a new character from the text buffer.
            * @details  If the next character of text buffer is the null character, it reads a new line and
            *          updates the value of the text buffer.
            * @see     GetLine()
            * @return  The current caracter on the text buffer.
            */
            char FetchChar();

        /*!
        * @brief   Gets a new character from the text buffer.
        * @details  If the next character of text buffer is the null character, it reads a new line and
        *          updates the value of the text buffer.
        * @warning If we call PutBackChar() after FetchChar() we will get a trash buffer.
        * @see     GetLine()
        * @return  The previous character on text buffer.
        */
        char PutBackChar();

        line_data GetLineData() {
            std::string out(std::begin(current_line_),
                            std::find(current_line_.begin(), current_line_.end(), 0));
            return {out, current_char_i_, current_line_i_};
        }

    private:

        void FetchLine();

        bool IsEOL();

    };


        class OutputStreamBuffer {

        protected:
            char text[kMaxBufferSize + 16];

            virtual void PutLine() = 0;

            virtual void PutLine(const char *textOutput) {
                strcpy(text, textOutput);
                PutLine();
            }

        };

        class ListPrinterBuffer : public OutputStreamBuffer {
            std::string source_file_name_;
            char date_[26];
            int page_number_;
            int line_counter_;

        protected:
            void PutLine() override;

        public:

            void PutLine(const char *textOutput) override {
                OutputStreamBuffer::PutLine(textOutput);
            }

            void PutLine(const char *textOutput, int lineNumber) {
                sprintf(text, "%4d : %s", lineNumber, textOutput);
                PutLine();
            }
        };

        extern ListPrinterBuffer list;
    } //namespace compiler
#endif
