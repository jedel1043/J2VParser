cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(J2VParser
        VERSION "0.1"
        DESCRIPTION "Some popular parser implementations"
        LANGUAGES CXX)
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

#include(GNUInstallDirs)

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
    message("Testing enabled.")
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

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(app)


# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()


#get_property(LibHeaders GLOBAL PROPERTY GlobalLibHeaders)
#message("\nLibrary headers list:")
#foreach (HeaderF ${LibHeaders})
#    message("${HeaderF}")
#endforeach ()

#get_property(LibSources GLOBAL PROPERTY GlobalLibSources)
#message("\nLibrary sources list:")
#foreach (SourceF ${LibSources})
#    message("${SourceF}")
#endforeach ()

#add_library(J2VParser ${LibSources} ${LibHeaders})
#target_include_directories(J2VParser PRIVATE include)
#
#get_property(AppSources GLOBAL PROPERTY GlobalAppSources)
#message("\nApp sources list:")
#foreach (SrcFile ${AppSources})
#    message("${SrcFile}")
#    get_filename_component(AppName ${SrcFile} NAME_WE)
#    add_executable(${AppName} ${SrcFile})
#    target_include_directories(${AppName} PRIVATE include)
#    target_link_libraries(${AppName} J2VParser)
#endforeach()
#
#
#if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
#    option(PackageTests "Build the tests" ON)
#else()
#    option(PackageTests "Build the tests" OFF)
#endif()
