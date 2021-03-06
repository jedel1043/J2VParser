# References google test framework
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

# Hides Gtest variables from CMake GUI
mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# Macro definition for testing J2VP libraries
macro(package_add_test TestName)
    # create an executable in which the tests will be stored
    add_executable(${TestName} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TestName} gtest gmock gtest_main J2VParser_lib)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TestName}
            # set a working directory to your project root so that you can find test data via paths relative to the project root
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            )
    set_target_properties(${TestName} PROPERTIES FOLDER tests)
endmacro()

set(TestSources "")
# Get list of all sources in folder
# [[[cog
#    import cog, glob, os
#    for config in glob.glob('**/*.cpp', recursive=True):
#       if os.name == 'nt':
#           cog.outl("set(TestSources ${TestSources} %s)" % config.replace("\\", "/"))
#       else:
#           cog.outl("set(TestSources ${TestSources} %s)" % config)
# ]]]
set(TestSources ${TestSources} test_automata.cpp)
set(TestSources ${TestSources} test_regex.cpp)
## [[[end]]]

#package_add_test(TestAutomata test_automata.cpp)

foreach (TestFile ${TestSources})
    get_filename_component(TestName ${TestFile} NAME_WE)
    package_add_test(${TestName} ${TestFile})
endforeach ()