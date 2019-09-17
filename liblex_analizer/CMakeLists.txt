PROJECT(liblex_analyzer VERSION 0.1 LANGUAGES CXX)

add_library(lib src/DFA.cpp src/NFA.cpp src/Regex.cpp src/Parser.cpp)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(lib Boost::serialization)

# Every library has unit tests, of course
add_executable(testlib
        test/testlex_analyzer.cpp)

target_link_libraries(testlib
        lib)

add_test(testlib testlib)