#ifndef PARSER_H
#define PARSER

#include <map>
#include <string>

enum TokenCode{
  EOS = 1,          // END OF STRING
  ANY,              // .
  WHITE_SPACE,      
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

class Parser{


    public:
        static bool machine();
        static bool rule();
        static bool action();
        static bool expr();
        static bool exprp();
        static bool catExpr();
        static bool catExprp();
        static bool factor();
        static bool term();
        static bool whiteSpace();
        static bool character();
        static bool str();
        static bool strp();
    
};






#endif