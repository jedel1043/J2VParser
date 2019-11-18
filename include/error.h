/*!
 * @file error.h
 * @brief Utilities for throwing errors at execution.
 * @details Some utilities used to throw errors in the parsing algorithms of regex_parser and
 * grammar_parser.
 */

#ifndef IO_ERROR_H
#define IO_ERROR_H

#include <string>

namespace compiler::error {

/*!
 * @brief Error codes available to throw with functions AbortTranslation() and SyntaxError().
 *
 * @details Used as arguments to call functions AbortTranslation() and SyntaxError() to output the error message.
 * Every code has its corresponding description in abortMsg.
 *
 */
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
        MissingApostrophe = -12,
        MissingSemicolon = -13,
        MissingColon = -14,
        UnknownSymbol = -15,
        MissingSymbol = -16,
        MissingRuleName = -17
    };

    static const std::string abort_msg[18] = {
            "",
            "Invalid command line arguments",
            "Failed to open source file",
            "Malformed regular expression",
            "Missing close parenthesis",
            "Missing [ in character class",
            "+ ? or * must Follow an expression or subexpression",
            "^ must be at start of expression or after [",
            "Newline in quoted string, use \\n to get newline token into expression",
            "Missing } in macro expansion",
            "Macro doesn't exist",
            "Macro expansions nested too deeply",
            "Missing apostrophe in terminal Symbol",
            "Missing semicolon in end of rules",
            "Missing colon in beginning of rules",
            "Found unknown expression when parsing symbol",
            "Missing symbol between apostrophes",
            "Missing rule name"
    };; /*!< Stores the extended description for every code in AbortCode*/

/*!
 * @brief Throws the @p error_code defined in abortMsg.
 *
 * Stops program execution immediately.
 * @remark Recommended only for throwing before-parsing errors e.g. InvalidCommandLineArgs.
 * @param error_code Code in AbortCode to throw.
 */
    void AbortTranslation(AbortCode error_code);

/*!
 * @brief Throws the @p error_code defined in abortMsg.
 *
 * Stops program execution immediately. Outputs column and line where the error was found.
 * @remark Recommended only for throwing parsing errors, since this function outputs column and line position in
 * the input file e.g. MissingCloseParenthesis.
 * @param error_code Code in AbortCode to throw.
 */
    void SyntaxError(AbortCode syntax_error_code);

} //namespace compiler::error
#endif
