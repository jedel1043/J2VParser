//
// Created by Jedel on 06/10/2019.
//

#ifndef COMPILER_GRAMMAR_SCANNER_H
#define COMPILER_GRAMMAR_SCANNER_H

#include "scanner.h"

class Grammar_Scanner : public Scanner {
public:
    explicit Grammar_Scanner(TextSourceBuffer *buffer);

    Token get();
};


#endif //COMPILER_GRAMMAR_SCANNER_H
