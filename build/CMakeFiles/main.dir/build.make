# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cbaron/Projects/chip8-emulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cbaron/Projects/chip8-emulator/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/src/Interpreter.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/Interpreter.cpp.o: ../src/Interpreter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cbaron/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/src/Interpreter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/Interpreter.cpp.o -c /home/cbaron/Projects/chip8-emulator/src/Interpreter.cpp

CMakeFiles/main.dir/src/Interpreter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/Interpreter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cbaron/Projects/chip8-emulator/src/Interpreter.cpp > CMakeFiles/main.dir/src/Interpreter.cpp.i

CMakeFiles/main.dir/src/Interpreter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/Interpreter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cbaron/Projects/chip8-emulator/src/Interpreter.cpp -o CMakeFiles/main.dir/src/Interpreter.cpp.s

CMakeFiles/main.dir/src/Interpreter.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/src/Interpreter.cpp.o.requires

CMakeFiles/main.dir/src/Interpreter.cpp.o.provides: CMakeFiles/main.dir/src/Interpreter.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/src/Interpreter.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/src/Interpreter.cpp.o.provides

CMakeFiles/main.dir/src/Interpreter.cpp.o.provides.build: CMakeFiles/main.dir/src/Interpreter.cpp.o


CMakeFiles/main.dir/src/Logger.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/Logger.cpp.o: ../src/Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cbaron/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/src/Logger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/Logger.cpp.o -c /home/cbaron/Projects/chip8-emulator/src/Logger.cpp

CMakeFiles/main.dir/src/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cbaron/Projects/chip8-emulator/src/Logger.cpp > CMakeFiles/main.dir/src/Logger.cpp.i

CMakeFiles/main.dir/src/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cbaron/Projects/chip8-emulator/src/Logger.cpp -o CMakeFiles/main.dir/src/Logger.cpp.s

CMakeFiles/main.dir/src/Logger.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/src/Logger.cpp.o.requires

CMakeFiles/main.dir/src/Logger.cpp.o.provides: CMakeFiles/main.dir/src/Logger.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/src/Logger.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/src/Logger.cpp.o.provides

CMakeFiles/main.dir/src/Logger.cpp.o.provides.build: CMakeFiles/main.dir/src/Logger.cpp.o


CMakeFiles/main.dir/src/Memory.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/Memory.cpp.o: ../src/Memory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cbaron/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/src/Memory.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/Memory.cpp.o -c /home/cbaron/Projects/chip8-emulator/src/Memory.cpp

CMakeFiles/main.dir/src/Memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/Memory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cbaron/Projects/chip8-emulator/src/Memory.cpp > CMakeFiles/main.dir/src/Memory.cpp.i

CMakeFiles/main.dir/src/Memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/Memory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cbaron/Projects/chip8-emulator/src/Memory.cpp -o CMakeFiles/main.dir/src/Memory.cpp.s

CMakeFiles/main.dir/src/Memory.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/src/Memory.cpp.o.requires

CMakeFiles/main.dir/src/Memory.cpp.o.provides: CMakeFiles/main.dir/src/Memory.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/src/Memory.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/src/Memory.cpp.o.provides

CMakeFiles/main.dir/src/Memory.cpp.o.provides.build: CMakeFiles/main.dir/src/Memory.cpp.o


CMakeFiles/main.dir/src/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cbaron/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/src/main.cpp.o -c /home/cbaron/Projects/chip8-emulator/src/main.cpp

CMakeFiles/main.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cbaron/Projects/chip8-emulator/src/main.cpp > CMakeFiles/main.dir/src/main.cpp.i

CMakeFiles/main.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cbaron/Projects/chip8-emulator/src/main.cpp -o CMakeFiles/main.dir/src/main.cpp.s

CMakeFiles/main.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/src/main.cpp.o.requires

CMakeFiles/main.dir/src/main.cpp.o.provides: CMakeFiles/main.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/src/main.cpp.o.provides

CMakeFiles/main.dir/src/main.cpp.o.provides.build: CMakeFiles/main.dir/src/main.cpp.o


# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/src/Interpreter.cpp.o" \
"CMakeFiles/main.dir/src/Logger.cpp.o" \
"CMakeFiles/main.dir/src/Memory.cpp.o" \
"CMakeFiles/main.dir/src/main.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/src/Interpreter.cpp.o
main: CMakeFiles/main.dir/src/Logger.cpp.o
main: CMakeFiles/main.dir/src/Memory.cpp.o
main: CMakeFiles/main.dir/src/main.cpp.o
main: CMakeFiles/main.dir/build.make
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cbaron/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main

.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/requires: CMakeFiles/main.dir/src/Interpreter.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/src/Logger.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/src/Memory.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/main.dir/requires

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /home/cbaron/Projects/chip8-emulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cbaron/Projects/chip8-emulator /home/cbaron/Projects/chip8-emulator /home/cbaron/Projects/chip8-emulator/build /home/cbaron/Projects/chip8-emulator/build /home/cbaron/Projects/chip8-emulator/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

