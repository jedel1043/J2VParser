//
// Created by Jedel on 06/10/2019.
//

#include "compiler/Grammar_Scanner.h"

Grammar_Scanner::Grammar_Scanner(TextSourceBuffer *buffer) : Scanner(buffer) {
    int i;
    for (i = 0; i <= 31; i++) charCodeMap[i] = TOKEN_ERROR;
    for (i = ' '; i <= 127; i++) charCodeMap[i] = L;
    charCodeMap[';'] = SEMICOLON;
    charCodeMap[':'] = COLON;
    charCodeMap['|'] = OR;
    charCodeMap[127] = TOKEN_ERROR;
}

Token Grammar_Scanner::get(){
    Scanner::skipWhiteSpace();
    Token t = Scanner::get();
    if(t == EOS)
        return Scanner::get();
    return t;
}
