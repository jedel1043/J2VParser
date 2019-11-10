cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Compiler)

set(Compiler_VERSION_MAJOR 0)
set(Compiler_VERSION_MINOR 1)
set (CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)

configure_file(
        "src/config.h.in"
        "${PROJECT_BINARY_DIR}/src/config.h"
)

include_directories(
        include
)

file(GLOB SOURCES
        lib/*.cpp
        lib/*/*.cpp
        lib/*/*/*.cpp
        )

add_executable(test_lexical test/main.cpp ${SOURCES})
add_executable(test_grammar_array test/main3.cpp ${SOURCES})
add_executable(LL1_parser app/src/LL1_parser.cpp ${SOURCES})
