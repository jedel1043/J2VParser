# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\msys64\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\msys64\mingw64\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Coding\Programones\Cpprogramones\lexical-analyzer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug

# Include any dependencies generated for this target.
include CMakeFiles/test_parser.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_parser.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_parser.dir/flags.make

CMakeFiles/test_parser.dir/main4.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/main4.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/main4.cpp.obj: ../main4.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_parser.dir/main4.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\main4.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\main4.cpp

CMakeFiles/test_parser.dir/main4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/main4.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\main4.cpp > CMakeFiles\test_parser.dir\main4.cpp.i

CMakeFiles/test_parser.dir/main4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/main4.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\main4.cpp -o CMakeFiles\test_parser.dir\main4.cpp.s

CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.obj: ../lib/automata/DFA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\automata\DFA.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\automata\DFA.cpp

CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\automata\DFA.cpp > CMakeFiles\test_parser.dir\lib\automata\DFA.cpp.i

CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\automata\DFA.cpp -o CMakeFiles\test_parser.dir\lib\automata\DFA.cpp.s

CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.obj: ../lib/automata/NFA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\automata\NFA.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\automata\NFA.cpp

CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\automata\NFA.cpp > CMakeFiles\test_parser.dir\lib\automata\NFA.cpp.i

CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\automata\NFA.cpp -o CMakeFiles\test_parser.dir\lib\automata\NFA.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.obj: ../lib/compiler/Grammar_Scanner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\Grammar_Scanner.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\Grammar_Scanner.cpp

CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\Grammar_Scanner.cpp > CMakeFiles\test_parser.dir\lib\compiler\Grammar_Scanner.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\Grammar_Scanner.cpp -o CMakeFiles\test_parser.dir\lib\compiler\Grammar_Scanner.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.obj: ../lib/compiler/LL1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\LL1.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\LL1.cpp

CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\LL1.cpp > CMakeFiles\test_parser.dir\lib\compiler\LL1.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\LL1.cpp -o CMakeFiles\test_parser.dir\lib\compiler\LL1.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.obj: ../lib/compiler/grammar_parser/grammar_array.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\grammar_parser\grammar_array.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\grammar_parser\grammar_array.cpp

CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\grammar_parser\grammar_array.cpp > CMakeFiles\test_parser.dir\lib\compiler\grammar_parser\grammar_array.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\grammar_parser\grammar_array.cpp -o CMakeFiles\test_parser.dir\lib\compiler\grammar_parser\grammar_array.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.obj: ../lib/compiler/grammar_parser/grammar_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\grammar_parser\grammar_parser.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\grammar_parser\grammar_parser.cpp

CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\grammar_parser\grammar_parser.cpp > CMakeFiles\test_parser.dir\lib\compiler\grammar_parser\grammar_parser.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\grammar_parser\grammar_parser.cpp -o CMakeFiles\test_parser.dir\lib\compiler\grammar_parser\grammar_parser.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.obj: ../lib/compiler/macros.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\macros.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\macros.cpp

CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\macros.cpp > CMakeFiles\test_parser.dir\lib\compiler\macros.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\macros.cpp -o CMakeFiles\test_parser.dir\lib\compiler\macros.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.obj: ../lib/compiler/parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\parser.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\parser.cpp

CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\parser.cpp > CMakeFiles\test_parser.dir\lib\compiler\parser.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\parser.cpp -o CMakeFiles\test_parser.dir\lib\compiler\parser.cpp.s

CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.obj: ../lib/compiler/scanner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\compiler\scanner.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\scanner.cpp

CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\scanner.cpp > CMakeFiles\test_parser.dir\lib\compiler\scanner.cpp.i

CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\compiler\scanner.cpp -o CMakeFiles\test_parser.dir\lib\compiler\scanner.cpp.s

CMakeFiles/test_parser.dir/lib/error.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/error.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/error.cpp.obj: ../lib/error.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/test_parser.dir/lib/error.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\error.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\error.cpp

CMakeFiles/test_parser.dir/lib/error.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/error.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\error.cpp > CMakeFiles\test_parser.dir\lib\error.cpp.i

CMakeFiles/test_parser.dir/lib/error.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/error.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\error.cpp -o CMakeFiles\test_parser.dir\lib\error.cpp.s

CMakeFiles/test_parser.dir/lib/io/buffer.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/io/buffer.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/io/buffer.cpp.obj: ../lib/io/buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/test_parser.dir/lib/io/buffer.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\io\buffer.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\io\buffer.cpp

CMakeFiles/test_parser.dir/lib/io/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/io/buffer.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\io\buffer.cpp > CMakeFiles\test_parser.dir\lib\io\buffer.cpp.i

CMakeFiles/test_parser.dir/lib/io/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/io/buffer.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\io\buffer.cpp -o CMakeFiles\test_parser.dir\lib\io\buffer.cpp.s

CMakeFiles/test_parser.dir/lib/io/common.cpp.obj: CMakeFiles/test_parser.dir/flags.make
CMakeFiles/test_parser.dir/lib/io/common.cpp.obj: CMakeFiles/test_parser.dir/includes_CXX.rsp
CMakeFiles/test_parser.dir/lib/io/common.cpp.obj: ../lib/io/common.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/test_parser.dir/lib/io/common.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\test_parser.dir\lib\io\common.cpp.obj -c C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\io\common.cpp

CMakeFiles/test_parser.dir/lib/io/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_parser.dir/lib/io/common.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\io\common.cpp > CMakeFiles\test_parser.dir\lib\io\common.cpp.i

CMakeFiles/test_parser.dir/lib/io/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_parser.dir/lib/io/common.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Coding\Programones\Cpprogramones\lexical-analyzer\lib\io\common.cpp -o CMakeFiles\test_parser.dir\lib\io\common.cpp.s

# Object files for target test_parser
test_parser_OBJECTS = \
"CMakeFiles/test_parser.dir/main4.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/error.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/io/buffer.cpp.obj" \
"CMakeFiles/test_parser.dir/lib/io/common.cpp.obj"

# External object files for target test_parser
test_parser_EXTERNAL_OBJECTS =

test_parser.exe: CMakeFiles/test_parser.dir/main4.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/automata/DFA.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/automata/NFA.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/Grammar_Scanner.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/LL1.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_array.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/grammar_parser/grammar_parser.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/macros.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/parser.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/compiler/scanner.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/error.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/io/buffer.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/lib/io/common.cpp.obj
test_parser.exe: CMakeFiles/test_parser.dir/build.make
test_parser.exe: CMakeFiles/test_parser.dir/linklibs.rsp
test_parser.exe: CMakeFiles/test_parser.dir/objects1.rsp
test_parser.exe: CMakeFiles/test_parser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable test_parser.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\test_parser.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_parser.dir/build: test_parser.exe

.PHONY : CMakeFiles/test_parser.dir/build

CMakeFiles/test_parser.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\test_parser.dir\cmake_clean.cmake
.PHONY : CMakeFiles/test_parser.dir/clean

CMakeFiles/test_parser.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Coding\Programones\Cpprogramones\lexical-analyzer C:\Coding\Programones\Cpprogramones\lexical-analyzer C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug C:\Coding\Programones\Cpprogramones\lexical-analyzer\debug\CMakeFiles\test_parser.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_parser.dir/depend
