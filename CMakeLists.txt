cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Compiler)

set(Compiler_VERSION_MAJOR 0)
set(Compiler_VERSION_MINOR 1)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

include(GNUInstallDirs)

set(SubdirList src
        include
        app)

foreach (DirName ${SubdirList})
    message("In dir ${DirName}")
    message("COMMAND: python -m cogapp -r CMakeLists.txt")
    execute_process(COMMAND python -m cogapp -r CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DirName})
    add_subdirectory(${DirName})
endforeach ()

get_property(LibHeaders GLOBAL PROPERTY GlobalLibHeaders)
message("\nLibrary headers list:")
foreach (HeaderF ${LibHeaders})
    message("${HeaderF}")
endforeach ()

get_property(LibSources GLOBAL PROPERTY GlobalLibSources)
message("\nLibrary sources list:")
foreach (SourceF ${LibSources})
    message("${SourceF}")
endforeach ()

add_library(J2VParser ${LibSources} ${LibHeaders})
target_include_directories(J2VParser PRIVATE include)
enable_testing()

get_property(AppSources GLOBAL PROPERTY GlobalAppSources)
message("\nApp sources list:")
foreach (SrcFile ${AppSources})
    message("${SrcFile}")
    get_filename_component(AppName ${SrcFile} NAME_WE)
    add_executable(${AppName} ${SrcFile})
    target_include_directories(${AppName} PRIVATE include)
    target_link_libraries(${AppName} J2VParser)
endforeach ()
