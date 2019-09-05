cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(lexical-analyzer VERSION 0.1 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)

# Targets that we develop here

enable_testing()
add_subdirectory(boost-cmake)
add_subdirectory(liblex_analizer)
add_subdirectory(app)