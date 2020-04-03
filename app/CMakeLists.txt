set(AppSources "")
# [[[cog
#    import cog, glob
#    for config in glob.glob('**/*.cpp', recursive=True):
#       cog.outl("set(AppSources ${AppSources} ${CMAKE_CURRENT_LIST_DIR}/%s)" % config.replace("\\", "/"))
# ]]]
set(AppSources ${AppSources} ${CMAKE_CURRENT_LIST_DIR}/src/LALR_parser.cpp)
set(AppSources ${AppSources} ${CMAKE_CURRENT_LIST_DIR}/src/LL1_parser.cpp)
set(AppSources ${AppSources} ${CMAKE_CURRENT_LIST_DIR}/src/LR0_parser.cpp)
set(AppSources ${AppSources} ${CMAKE_CURRENT_LIST_DIR}/src/LR1_parser.cpp)
set(AppSources ${AppSources} ${CMAKE_CURRENT_LIST_DIR}/src/SLR1_parser.cpp)
# [[[end]]]
set_property(GLOBAL PROPERTY GlobalAppSources ${AppSources})

