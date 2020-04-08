#ifndef J2VPARSER_GRAMMAR_PARSER_H
#define J2VPARSER_GRAMMAR_PARSER_H

#include <string>

#include "J2VParser/buffer.h"
#include "J2VParser/parsers/grammar/grammar_scanner.h"
#include "J2VParser/parsers/grammar/grammar_array.h"

namespace J2VParser::grammar {
    GrammarArray ParseGrammar(io_buffer::TextSourceBuffer &input_file);

    GrammarArray ParseGrammar(GrammarScanner &grammarAnalyzer);
} // namespace compiler::grammar

#endif //J2VPARSER_GRAMMAR_PARSER_H
