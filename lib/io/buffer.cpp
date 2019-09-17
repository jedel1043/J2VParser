#include <cstring>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <io/buffer.h>
#include <io/common.h>
#include <misc.h>

using namespace std;

char  EOFChar       = 0x7F;
bool  listFlag      = true;

InputBaseBuffer::InputBaseBuffer(const string fname, AbortCode ac){
  this->fileName = fname;
  file.open(this->fileName, fstream::in);
  if(!file.good()) AbortTranslation(ac);
}

char InputBaseBuffer::fetchChar(){
  extern int inputPosition;
  char c;
  if(*currentChar == EOFChar) return EOFChar;
  else if(*currentChar == '\0') c = getLine();
  else{
    ++currentChar;
    ++inputPosition;
    c = *currentChar;
  }

  if(c == '\t') inputPosition += TAB_SIZE - (inputPosition % TAB_SIZE);
  return c;
}

char InputBaseBuffer::putBackChar(){
  extern int inputPosition;
  --currentChar;
  --inputPosition;
  return *currentChar;
}

TextSourceBuffer::TextSourceBuffer(const string fname) : InputBaseBuffer(fname, SourceFileOpenFailed){
  if(listFlag) list.init(fname);
  getLine();
}

char TextSourceBuffer::getLine(){
  extern int currentLineNumber, inputPosition;
  if(file.eof()) currentChar = &EOFChar;
  else{
    file.getline(text, MAX_BUFFER_SIZE);
    currentChar = text;

    if(listFlag) list.putLine(text, ++currentLineNumber);
  }

  inputPosition = 0;
  return *currentChar;
}

ListPrinterBuffer list;

void ListPrinterBuffer::printPageHeader(){
  cout << "\fPage " << ++pageNumber << "  " << sourceFileName << "  " << date << endl << endl;
  lineCounter = 0;
}

void ListPrinterBuffer::init(const string fname){
  text[0] = '\0';
  pageNumber = 0;
  sourceFileName = fname;
  time_t timer;
  time(&timer);
  strcpy(date, asctime(localtime(&timer)));
  date[strlen(date) - 1] = '\0';
  printPageHeader();
}

void ListPrinterBuffer::putLine(){
  if(listFlag && (lineCounter == MAX_LINES_PER_PAGE)) printPageHeader();
  text[MAX_PRINT_LINE_LENGTH] = '\0';
  cout << text << endl;
  text[0] = '\0';
  ++lineCounter;
}
