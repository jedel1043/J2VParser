cmake_minimum_required (VERSION 2.8.11 FATAL_ERROR)
project (Compiler)

set (Compiler_VERSION_MAJOR 0)
set (Compiler_VERSION_MINOR 1)

include(GNUInstallDirs)

configure_file (
  "src/config.h.in"
  "${PROJECT_BINARY_DIR}/src/config.h"
)

include_directories(
  src/
        include
)

file(GLOB SOURCES
    lib/*.cpp
    lib/*/*.cpp
    test/*.cpp
)

add_executable(compiler main.cpp ${SOURCES})
