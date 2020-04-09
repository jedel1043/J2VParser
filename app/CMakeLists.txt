set(AppSources "")
# Get list of all sources in folder
# [[[cog
#    import cog, glob, os
#    for config in glob.glob('**/*.cpp', recursive=True):
#       if os.name == 'nt':
#           cog.outl("set(AppSources ${AppSources} %s)" % config.replace("\\", "/"))
#       else:
#           cog.outl("set(AppSources ${AppSources} %s)" % config)
# ]]]
set(AppSources ${AppSources} LALR_parser.cpp)
set(AppSources ${AppSources} LL1_parser.cpp)
set(AppSources ${AppSources} LR0_parser.cpp)
set(AppSources ${AppSources} LR1_parser.cpp)
set(AppSources ${AppSources} SLR1_parser.cpp)
# [[[end]]]

foreach (SrcFile ${AppSources})
    get_filename_component(AppName ${SrcFile} NAME_WE)
    add_executable(${AppName} ${SrcFile})
    target_compile_features(${AppName} PRIVATE cxx_std_17)
    target_link_libraries(${AppName} PRIVATE J2VParser_lib)
endforeach ()

