#ifndef PARSER_H
#define PARSER_H

#include <utility>
#include <set>
#include <vector>

#include "J2VParser/analyzers/automata/nfa.h"
#include "J2VParser/buffer.h"
#include "J2VParser/analyzers/regex/regex_scanner.h"

namespace J2VParser::regex {

    std::vector<automata::NFA> ParseRegex(io_buffer::TextSourceBuffer &source_buffer);

    std::vector<automata::NFA> ParseRegex(RegexScanner &regex_scanner);
} // namespace compiler::regex

#endif
