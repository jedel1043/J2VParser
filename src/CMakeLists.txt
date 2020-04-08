#---------------------------------------------------------------------------------------------
#--------------------------------- FILE SEARCH USING COG -------------------------------------
#---------------------------------------------------------------------------------------------

set(HeaderList "")
# Get list of all headers in include/J2VParser
# [[[cog
#    import cog, glob
#    for config in glob.glob('../include/J2VParser/**/*.h', recursive=True):
#       cog.outl("set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}%s)" % config.replace("\\", "/").replace("..", ""))
# ]]]
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/buffer.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/error.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/lexical_analyzer.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/lexical_analyzer_f.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/lexical_analyzer_s.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/automata/automaton.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/automata/dfa.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/automata/nfa.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/regex/regex_parser.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/analyzers/regex/regex_scanner.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/botup_parser.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/conflict_man.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/items.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/LALR.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/LL1.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/LR0.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/LR1.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/parser.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/SLR1.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/grammar/grammar_array.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/grammar/grammar_parser.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/parsers/grammar/grammar_scanner.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/utils/bimap.h)
set(HeaderList ${HeaderList} ${PROJECT_SOURCE_DIR}/include/J2VParser/utils/utils.h)
# [[[end]]]

set(SourceList "")
# Get list of all sources in folder
# [[[cog
#    import cog, glob
#    for config in glob.glob('**/*.cpp', recursive=True):
#       cog.outl("set(SourceList ${SourceList} %s)" % config.replace("\\", "/"))
# ]]]
set(SourceList ${SourceList} buffer.cpp)
set(SourceList ${SourceList} error.cpp)
set(SourceList ${SourceList} analyzers/lexical_analyzer_f.cpp)
set(SourceList ${SourceList} analyzers/lexical_analyzer_s.cpp)
set(SourceList ${SourceList} analyzers/automata/dfa.cpp)
set(SourceList ${SourceList} analyzers/automata/nfa.cpp)
set(SourceList ${SourceList} analyzers/regex/regex_parser.cpp)
set(SourceList ${SourceList} analyzers/regex/regex_scanner.cpp)
set(SourceList ${SourceList} parsers/items.cpp)
set(SourceList ${SourceList} parsers/LALR.cpp)
set(SourceList ${SourceList} parsers/LL1.cpp)
set(SourceList ${SourceList} parsers/LR0.cpp)
set(SourceList ${SourceList} parsers/LR1.cpp)
set(SourceList ${SourceList} parsers/SLR1.cpp)
set(SourceList ${SourceList} parsers/grammar/grammar_array.cpp)
set(SourceList ${SourceList} parsers/grammar/grammar_parser.cpp)
set(SourceList ${SourceList} parsers/grammar/grammar_scanner.cpp)
# [[[end]]]

#---------------------------------------------------------------------------------------------
#--------------------------------- LIBRARY GENERATION ----------------------------------------
#---------------------------------------------------------------------------------------------

add_library(J2VParser_lib ${SourceList} ${HeaderList})

# We need this directory, and users of our library will need it too
target_include_directories(J2VParser_lib PUBLIC ../include)

# All users of this library will need at least C++17
target_compile_features(J2VParser_lib PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})


