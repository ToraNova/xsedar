# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cjason/Desktop/secure-dist-computing/xsedar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cjason/Desktop/secure-dist-computing/xsedar/build

# Include any dependencies generated for this target.
include src/CMakeFiles/xsedar.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/xsedar.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/xsedar.dir/flags.make

src/CMakeFiles/xsedar.dir/xsedar.cpp.o: src/CMakeFiles/xsedar.dir/flags.make
src/CMakeFiles/xsedar.dir/xsedar.cpp.o: ../src/xsedar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cjason/Desktop/secure-dist-computing/xsedar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/xsedar.dir/xsedar.cpp.o"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/xsedar.dir/xsedar.cpp.o -c /home/cjason/Desktop/secure-dist-computing/xsedar/src/xsedar.cpp

src/CMakeFiles/xsedar.dir/xsedar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/xsedar.dir/xsedar.cpp.i"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cjason/Desktop/secure-dist-computing/xsedar/src/xsedar.cpp > CMakeFiles/xsedar.dir/xsedar.cpp.i

src/CMakeFiles/xsedar.dir/xsedar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/xsedar.dir/xsedar.cpp.s"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cjason/Desktop/secure-dist-computing/xsedar/src/xsedar.cpp -o CMakeFiles/xsedar.dir/xsedar.cpp.s

# Object files for target xsedar
xsedar_OBJECTS = \
"CMakeFiles/xsedar.dir/xsedar.cpp.o"

# External object files for target xsedar
xsedar_EXTERNAL_OBJECTS =

src/libxsedar.a: src/CMakeFiles/xsedar.dir/xsedar.cpp.o
src/libxsedar.a: src/CMakeFiles/xsedar.dir/build.make
src/libxsedar.a: src/CMakeFiles/xsedar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cjason/Desktop/secure-dist-computing/xsedar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libxsedar.a"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src && $(CMAKE_COMMAND) -P CMakeFiles/xsedar.dir/cmake_clean_target.cmake
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/xsedar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/xsedar.dir/build: src/libxsedar.a

.PHONY : src/CMakeFiles/xsedar.dir/build

src/CMakeFiles/xsedar.dir/clean:
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src && $(CMAKE_COMMAND) -P CMakeFiles/xsedar.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/xsedar.dir/clean

src/CMakeFiles/xsedar.dir/depend:
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cjason/Desktop/secure-dist-computing/xsedar /home/cjason/Desktop/secure-dist-computing/xsedar/src /home/cjason/Desktop/secure-dist-computing/xsedar/build /home/cjason/Desktop/secure-dist-computing/xsedar/build/src /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/CMakeFiles/xsedar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/xsedar.dir/depend

