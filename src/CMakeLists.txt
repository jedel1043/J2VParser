set(LibSources "")
# [[[cog
#    import cog, glob
#    for config in glob.glob('**/*.cpp', recursive=True):
#       cog.outl("set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/%s)" % config.replace("\\", "/"))
# ]]]
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/buffer.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/error.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/analyzers/lexical_analyzer_f.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/analyzers/lexical_analyzer_s.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/automata/dfa.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/automata/nfa.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/grammar_utils/grammar_analyzer.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/grammar_utils/grammar_array.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/grammar_utils/grammar_parser.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LALR.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LL1.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LR0.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/LR1.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/parser_algorithms/SLR1.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/regex_utils/regex_parser.cpp)
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/regex_utils/regex_scanner.cpp)
# [[[end]]]

set_property(GLOBAL PROPERTY GlobalLibSources ${LibSources})