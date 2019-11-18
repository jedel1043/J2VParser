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

#include "error.h"//!< Horizontal position of InputStreamBuffer at the file input (column number).

namespace compiler::io_buffer {
        extern char EOF_char;  /*!< End-Of-File character. */
        extern const bool list_flag; /*!< Global buffer listing for debugging purposes. */

        const int kMaxBufferSize = 256; //!< Max number of characters in a line in the file input.
        const int kTabSize = 4;   //!< Number of spaces equal to a tab.
        const int kMaxPrintLineLength = 80;  //!< Max number of characters in a line in the file output.
        const int kMaxLinesPerPage = 50;  //!< Number of lines per page in the file output.

/*!
* @brief   Abstract class for the input buffer.
* @details Reads a file line by line (for this purpose must define the method GetLine()) and gets
*          character by character until it reaches the end of the file. It has the option of putting
*          back a character on the buffer.
*/
        class InputStreamBuffer {
        protected:
            std::string file_name_;         /*!< File name of the input stream. */
            std::fstream file_;            /*!< Input stream. */
            char *current_char_;            /*!< The current character read from the input stream. */
            char text_[kMaxBufferSize];   /*!< Last line read from the input stream. */

            /*!
            * @brief   Abstract method that reads a line of the buffer.
            */
            virtual char GetLine() = 0;

        public:
            /*!
            * @brief    Constructor for InputStreamBuffer
            * @param    fname File name of the input stream.
            * @param    ac AbortCode in case there is an error opening the file.
            */
            InputStreamBuffer(const std::string &fname, error::AbortCode ac);

            /*!
            * @brief   Destructor for InputStreamBuffer. Close the input stream.
            */
            virtual ~InputStreamBuffer() { file_.close(); }

            /*!
            * @brief   Returns the current character from the text buffer.
            * @return  The current caracter.
            */
            char GetChar() const { return *current_char_; }

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


        class TextSourceBuffer : public InputStreamBuffer {
            char GetLine() override;

        public:
            explicit TextSourceBuffer(const std::string &fname);
        };


        class ListPrinterBuffer : public OutputStreamBuffer {
            std::string source_file_name_;
            char date_[26];
            int page_number_;
            int line_counter_;

            void PrintPageHeader();

        protected:
            void PutLine() override;

        public:
            void Init(const std::string &fname);

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
