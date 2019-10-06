//  *************************************************************************************
//                                ERROR (HEADER)                                        *
//  FILE:   IO/error.h                                                                  *
//  MODULE: ERROR                                                                       *
//  Copyright (c) 2019 Copyright Holder All Rights Reserved.                            *
//  *************************************************************************************

#ifndef IO_ERROR_H
#define IO_ERROR_H

using namespace std;

extern int errorCounter;

enum AbortCode {
    InvalidCommandLineArgs = -1,
    SourceFileOpenFailed = -2,
    BadRegularExpression = -3,
    MissingCloseParenthesis = -4,
    MissingBracket = -5,
    MissingExpression = -6,
    MissingStartExpression = -7,
    InvalidNewLine = -8,
    BadMacro = -9,
    MissingMacro = -10,
    BadMacroExpansion = -11,
};

void AbortTranslation(AbortCode ac);

void SyntaxError(AbortCode ac);

#endif
