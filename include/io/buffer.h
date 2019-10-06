/*!
* @file         buffer.h
* @brief        Classes for the file control and listing utils.
* @detail       The abstract classes InputBaseBuffer and OutputBaseBuffer are the base classes for
*               reading and writing extern streams. TextSourceBuffer is a simple class, that inherits
*               of InputBaseBuffer, which reads ofstream buffers. ListPrinterBuffer prints a line with
*               its number line and groups lines in pages.
* @author       José Luis Castro García
* @date         $Date: 2019/09/16 22:40:00 $
*/

#ifndef IO_BUFFER_H
#define IO_BUFFER_H

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <error.h>
#include <misc.h>

using namespace std;

extern char EOFChar;  /*!< End-Of-File character. */
extern bool listFlag; /*!< Global buffer listing for debugging purposes. */


/*!
* @class   InputBaseBuffer
* @brief   Abstract class for the input buffer.
* @detail  Reads a file line by line (for this purpose must define the method getLine()) and gets
*          character by character until it reaches the end of the file. It has the option of putting
*          back a character on the buffer.
*/
class InputBaseBuffer {
protected:
    string fileName;              /*!< File name of the input stream. */
    fstream file;                 /*!< Input stream. */
    char *currentChar;            /*!< The current character that was read from the input stream. */
    char text[MAX_BUFFER_SIZE];   /*!< Last line that was read from the input stream. */

    /*!
    * @brief   Abstract method that reads a line of the buffer.
    */
    virtual char getLine() = 0;

public:
    /*!
    * @brief    Constructor for InputBaseBuffer.
    * @param    fname File name of the input stream.
    * @param    ac Abort code in case there is an error opening the file.
    */
    InputBaseBuffer(const string &fname, AbortCode ac);

    /*!
    * @brief   Destructor for InputBaseBuffer. Close the input stream.
    */
    virtual ~InputBaseBuffer() { file.close(); }

    /*!
    * @brief   Returns the current character from the text buffer.
    * @return  The current caracter.
    */
    char getChar() const { return *currentChar; }

    /*!
    * @brief   Gets a new character from the text buffer.
    * @detail  If the next character of text buffer is the null character, it reads a new line and
    *          update the value of the text buffer.
    * @see     getLine()
    * @return  The current caracter on the text buffer.
    */
    char fetchChar();

    /*!
    * @brief   Gets a new character from the text buffer.
    * @detail  If the next character of text buffer is the null character, it reads a new line and
    *          update the value of the text buffer.
    * @warning If we call putBackChar() after fetchChar() we will get a trash buffer.
    * @see     getLine()
    * @return  The previous character on text buffer.
    */
    char putBackChar();

};


class OutputBaseBuffer {

protected:
    char text[MAX_BUFFER_SIZE + 16];

    virtual void putLine() = 0;

    virtual void putLine(const char *textOutput) {
        strcpy(text, textOutput);
        putLine();
    }

};


class TextSourceBuffer : public InputBaseBuffer {
    char getLine() override;

public:
    explicit TextSourceBuffer(string fname);
};


class ListPrinterBuffer : public OutputBaseBuffer {
    string sourceFileName;
    char date[26];
    int pageNumber;
    int lineCounter;

    void printPageHeader();

protected:
    void putLine() override;

public:
    void init(string fname);

    void putLine(const char *textOutput) override {
        OutputBaseBuffer::putLine(textOutput);
    }

    void putLine(const char *textOutput, int lineNumber) {
        sprintf(text, "%4d : %s", lineNumber, textOutput);
        putLine();
    }
};

extern ListPrinterBuffer list;

#endif
