cmake_minimum_required(VERSION 3.13)
project(lexical-analyzer)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(boost-cmake)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main PUBLIC Boost::serialization)