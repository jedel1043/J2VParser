#ifndef SCANNER_H
#define SCANNER_H

#include <stack>
#include <io/buffer.h>
#include <misc.h>

using namespace std;

class Token {
    TokenCode token;
    char lexeme;
public:
    Token(TokenCode t, char v) : token(t), lexeme(v) {}

    bool operator==(TokenCode t);

    bool operator!=(TokenCode t);

    TokenCode getToken() const { return token; }

    char getLexeme() const { return lexeme; }
};

class Scanner {
    TextSourceBuffer *sourceBuffer;
    bool inQuote;
    bool escape;
    int inMacro;
    Token currentToken;
    stack<string> macros;

    char escapeChar(char c);

public:
    explicit Scanner(TextSourceBuffer *buffer);

    Token getCurrentToken() { return currentToken; }

    void skipWhiteSpace();

    Token yylex();

    TextSourceBuffer* get_pos();

    void pushBack() { sourceBuffer->putBackChar(); }

    void set_pos(TextSourceBuffer *new_pos);
};

extern TokenCode charCodeMap[128];

#endif
