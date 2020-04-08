cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(J2VParser
        VERSION "0.1"
        DESCRIPTION "Some popular parser implementations"
        LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)
    include(GoogleTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if (Doxygen_FOUND)
        add_subdirectory(docs)
    else ()
        message(STATUS "Doxygen not found, not building docs")
    endif ()
endif ()


# Using cog lib to update files in src folder
message("Cogging dir src")
execute_process(COMMAND python -m cogapp -r CMakeLists.txt
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Using cog lib to update files in app folder
message("Cogging dir app")
execute_process(COMMAND python -m cogapp -r CMakeLists.txt
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/app)

# Using cog lib to update files in tests folder
message("Cogging dir tests")
execute_process(COMMAND python -m cogapp -r CMakeLists.txt
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(app)


# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    message("Testing enabled.")
    add_subdirectory(tests)
endif ()