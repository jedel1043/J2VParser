#include <cctype>
#include <compiler/scanner.h>
#include <misc.h>
#include <io/buffer.h>

using namespace std;

TokenCode charCodeMap[128];

bool Token::operator==(TokenCode t) {
    return token == t;
}

bool Token::operator!=(TokenCode t) {
    return token != t;
}

Scanner::Scanner(TextSourceBuffer *buffer) :
        sourceBuffer(buffer), inQuote(false), escape(false), currentToken(EOS, '\0'), inMacro(0) {
    int i;
    for (i = 0; i <= 31; i++) charCodeMap[i] = TOKEN_ERROR;
    for (i = ' '; i <= '#'; i++) charCodeMap[i] = L;
    charCodeMap['$'] = AT_EOL;
    for (i = '%'; i <= '\''; i++) charCodeMap[i] = L;
    charCodeMap['('] = OPEN_PAREN;
    charCodeMap[')'] = CLOSE_PAREN;
    charCodeMap['*'] = CLOSURE;
    charCodeMap['+'] = PLUS_CLOSURE;
    charCodeMap[','] = L;
    charCodeMap['-'] = DASH;
    charCodeMap['.'] = ANY;
    for (i = '/'; i <= '>'; i++) charCodeMap[i] = L;
    charCodeMap['?'] = OPTIONAL;
    for (i = '@'; i <= 'Z'; i++) charCodeMap[i] = L;
    charCodeMap['['] = CCL_START;
    charCodeMap['\\'] = L;
    charCodeMap[']'] = CCL_END;
    charCodeMap['^'] = AT_BOL;
    for (i = '_'; i <= 'z'; i++) charCodeMap[i] = L;
    charCodeMap['{'] = OPEN_CURLY;
    charCodeMap['|'] = OR;
    charCodeMap['}'] = CLOSE_CURLY;
    charCodeMap[127] = TOKEN_ERROR;
}

void Scanner::skipWhiteSpace() {
    char c = sourceBuffer->getChar();
    while (isspace(c) && c != EOFChar)
        c = sourceBuffer->fetchChar();
}

char Scanner::escapeChar(char c) {
    if (!escape) return c;
    switch (c) {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        default:
            return c;
    }
}

Token Scanner::get() {
    char lexeme;

    if (currentToken == EOS) {
        if (inQuote) SyntaxError(InvalidNewLine);
        skipWhiteSpace();
        if (sourceBuffer->getChar() == EOFChar) {
            currentToken = Token(END_OF_INPUT, EOFChar);
            return currentToken;
        }
    }

    if (sourceBuffer->getChar() == '\0') {
        currentToken = Token(EOS, '\0');
        sourceBuffer->fetchChar();
        return currentToken;
    }

    if (sourceBuffer->getChar() == '"') {
        inQuote = !inQuote;
        if (sourceBuffer->fetchChar() == EOFChar || sourceBuffer->getChar() == '\0') {
            currentToken = Token(EOS, '\0');
            return currentToken;
        }
    }

    escape = (sourceBuffer->getChar() == '\\');
    if (escape) sourceBuffer->fetchChar();
    if (sourceBuffer->getChar() == EOFChar || sourceBuffer->getChar() == '\0')
        SyntaxError(InvalidNewLine);

    if (!inQuote) {
        if (isspace(sourceBuffer->getChar())) {
            currentToken = Token(EOS, '\0');
            return currentToken;
        }
        lexeme = sourceBuffer->getChar();
    } else {
        lexeme = sourceBuffer->getChar();
    }
    TokenCode newToken = (inQuote || escape) ? L : charCodeMap[lexeme];
    currentToken = Token(newToken, lexeme);
    sourceBuffer->fetchChar();
    return currentToken;
}
