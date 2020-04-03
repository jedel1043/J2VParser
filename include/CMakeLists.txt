set(LibHeaders "")
# [[[cog
#    import cog, glob
#    for config in glob.glob('**/*.h', recursive=True):
#       cog.outl("set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/%s)" % config.replace("\\", "/"))
# ]]]
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/buffer.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/common.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/error.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/analyzers/lexical_analyzer.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/analyzers/lexical_analyzer_f.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/analyzers/lexical_analyzer_s.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/automata/dfa.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/automata/nfa.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/grammar_utils/grammar_analyzer.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/grammar_utils/grammar_array.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/grammar_utils/grammar_parser.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/conflict_man.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LALR.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LL1.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LR0.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LR1.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/parser.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/SLR1.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/regex_utils/regex_parser.h)
set(LibHeaders ${LibHeaders} ${CMAKE_CURRENT_LIST_DIR}/parsers/regex_utils/regex_scanner.h)
# [[[end]]]

set_property(GLOBAL PROPERTY GlobalLibHeaders ${LibHeaders})