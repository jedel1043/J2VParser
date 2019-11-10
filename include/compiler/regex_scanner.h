#ifndef SCANNER_H
#define SCANNER_H

#include <stack>
#include <io/buffer.h>
#include <misc.h>

using namespace std;

class Regex_Token {
    TokenCodeRegex token;
    char lexeme;
public:
    Regex_Token(TokenCodeRegex t, char v) : token(t), lexeme(v) {}

    bool operator==(TokenCodeRegex t);

    bool operator!=(TokenCodeRegex t);

    TokenCodeRegex getToken() const { return token; }

    char getLexeme() const { return lexeme; }
};

class Regex_Scanner {
    TextSourceBuffer *sourceBuffer;
    bool inQuote;
    bool escape;
    int inMacro;
    Regex_Token currentToken;
    stack<string> macros;

    char escapeChar(char c);

public:
    explicit Regex_Scanner(TextSourceBuffer *buffer);

    Regex_Token getCurrentToken() { return currentToken; }

    void skipWhiteSpace();

    virtual Regex_Token getToken();

    TextSourceBuffer* get_pos();

    void pushBack() { sourceBuffer->putBackChar(); }

    void set_pos(TextSourceBuffer *new_pos);
};

extern TokenCodeRegex charCodeMap[128];

#endif
