#ifndef J2VPARSER_PARSER_H
#define J2VPARSER_PARSER_H

#include <utility>
#include <set>
#include <vector>

#include "J2VParser/analyzers/automata/nfa.h"
#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/regex/regex_scanner.h"

namespace J2VParser::regex {

    automata::DFA ParseRegex(io_buffer::TextSourceBuffer &source_buffer);

    automata::DFA ParseRegex(RegexScanner &regex_scanner);
} // namespace compiler::regex

#endif // J2VPARSER_PARSER_H
