# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/carlrocket/Projects/chip8-emulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/carlrocket/Projects/chip8-emulator/build

# Include any dependencies generated for this target.
include CMakeFiles/testing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testing.dir/flags.make

CMakeFiles/testing.dir/src/main.cpp.o: CMakeFiles/testing.dir/flags.make
CMakeFiles/testing.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/carlrocket/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testing.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testing.dir/src/main.cpp.o -c /home/carlrocket/Projects/chip8-emulator/src/main.cpp

CMakeFiles/testing.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testing.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/carlrocket/Projects/chip8-emulator/src/main.cpp > CMakeFiles/testing.dir/src/main.cpp.i

CMakeFiles/testing.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testing.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/carlrocket/Projects/chip8-emulator/src/main.cpp -o CMakeFiles/testing.dir/src/main.cpp.s

CMakeFiles/testing.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/testing.dir/src/main.cpp.o.requires

CMakeFiles/testing.dir/src/main.cpp.o.provides: CMakeFiles/testing.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/testing.dir/build.make CMakeFiles/testing.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/testing.dir/src/main.cpp.o.provides

CMakeFiles/testing.dir/src/main.cpp.o.provides.build: CMakeFiles/testing.dir/src/main.cpp.o


CMakeFiles/testing.dir/src/Memory.cpp.o: CMakeFiles/testing.dir/flags.make
CMakeFiles/testing.dir/src/Memory.cpp.o: ../src/Memory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/carlrocket/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/testing.dir/src/Memory.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testing.dir/src/Memory.cpp.o -c /home/carlrocket/Projects/chip8-emulator/src/Memory.cpp

CMakeFiles/testing.dir/src/Memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testing.dir/src/Memory.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/carlrocket/Projects/chip8-emulator/src/Memory.cpp > CMakeFiles/testing.dir/src/Memory.cpp.i

CMakeFiles/testing.dir/src/Memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testing.dir/src/Memory.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/carlrocket/Projects/chip8-emulator/src/Memory.cpp -o CMakeFiles/testing.dir/src/Memory.cpp.s

CMakeFiles/testing.dir/src/Memory.cpp.o.requires:

.PHONY : CMakeFiles/testing.dir/src/Memory.cpp.o.requires

CMakeFiles/testing.dir/src/Memory.cpp.o.provides: CMakeFiles/testing.dir/src/Memory.cpp.o.requires
	$(MAKE) -f CMakeFiles/testing.dir/build.make CMakeFiles/testing.dir/src/Memory.cpp.o.provides.build
.PHONY : CMakeFiles/testing.dir/src/Memory.cpp.o.provides

CMakeFiles/testing.dir/src/Memory.cpp.o.provides.build: CMakeFiles/testing.dir/src/Memory.cpp.o


CMakeFiles/testing.dir/src/Logger.cpp.o: CMakeFiles/testing.dir/flags.make
CMakeFiles/testing.dir/src/Logger.cpp.o: ../src/Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/carlrocket/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/testing.dir/src/Logger.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testing.dir/src/Logger.cpp.o -c /home/carlrocket/Projects/chip8-emulator/src/Logger.cpp

CMakeFiles/testing.dir/src/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testing.dir/src/Logger.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/carlrocket/Projects/chip8-emulator/src/Logger.cpp > CMakeFiles/testing.dir/src/Logger.cpp.i

CMakeFiles/testing.dir/src/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testing.dir/src/Logger.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/carlrocket/Projects/chip8-emulator/src/Logger.cpp -o CMakeFiles/testing.dir/src/Logger.cpp.s

CMakeFiles/testing.dir/src/Logger.cpp.o.requires:

.PHONY : CMakeFiles/testing.dir/src/Logger.cpp.o.requires

CMakeFiles/testing.dir/src/Logger.cpp.o.provides: CMakeFiles/testing.dir/src/Logger.cpp.o.requires
	$(MAKE) -f CMakeFiles/testing.dir/build.make CMakeFiles/testing.dir/src/Logger.cpp.o.provides.build
.PHONY : CMakeFiles/testing.dir/src/Logger.cpp.o.provides

CMakeFiles/testing.dir/src/Logger.cpp.o.provides.build: CMakeFiles/testing.dir/src/Logger.cpp.o


# Object files for target testing
testing_OBJECTS = \
"CMakeFiles/testing.dir/src/main.cpp.o" \
"CMakeFiles/testing.dir/src/Memory.cpp.o" \
"CMakeFiles/testing.dir/src/Logger.cpp.o"

# External object files for target testing
testing_EXTERNAL_OBJECTS =

testing: CMakeFiles/testing.dir/src/main.cpp.o
testing: CMakeFiles/testing.dir/src/Memory.cpp.o
testing: CMakeFiles/testing.dir/src/Logger.cpp.o
testing: CMakeFiles/testing.dir/build.make
testing: CMakeFiles/testing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/carlrocket/Projects/chip8-emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable testing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testing.dir/build: testing

.PHONY : CMakeFiles/testing.dir/build

CMakeFiles/testing.dir/requires: CMakeFiles/testing.dir/src/main.cpp.o.requires
CMakeFiles/testing.dir/requires: CMakeFiles/testing.dir/src/Memory.cpp.o.requires
CMakeFiles/testing.dir/requires: CMakeFiles/testing.dir/src/Logger.cpp.o.requires

.PHONY : CMakeFiles/testing.dir/requires

CMakeFiles/testing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testing.dir/clean

CMakeFiles/testing.dir/depend:
	cd /home/carlrocket/Projects/chip8-emulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/carlrocket/Projects/chip8-emulator /home/carlrocket/Projects/chip8-emulator /home/carlrocket/Projects/chip8-emulator/build /home/carlrocket/Projects/chip8-emulator/build /home/carlrocket/Projects/chip8-emulator/build/CMakeFiles/testing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testing.dir/depend

