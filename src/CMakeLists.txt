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
set(LibSources ${LibSources} ${CMAKE_CURRENT_LIST_DIR}/parsers/regex_utils/regex_parser.cpp)
# [[[end]]]

set_property(GLOBAL PROPERTY GlobalLibSources ${LibSources})