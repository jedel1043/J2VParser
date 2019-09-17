#ifndef MISC_H
#define MISC_H

#define MAX_BUFFER_SIZE         256
#define TAB_SIZE                4
#define MAX_PRINT_LINE_LENGTH   80
#define MAX_LINES_PER_PAGE      50
#define MACRO_NAME_MAX          34
#define MACRO_TEXT_MAX          80

#define NCOLS       10
#define YY_TYPE     "YY_TYPE"

enum TokenCode{
  EOS = 1,          // END OF STRING
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
  TOKEN_ERROR       // Error
};

#endif
