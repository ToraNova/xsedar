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
include src/toralib/CMakeFiles/toralib.dir/depend.make

# Include the progress variables for this target.
include src/toralib/CMakeFiles/toralib.dir/progress.make

# Include the compile flags for this target's objects.
include src/toralib/CMakeFiles/toralib.dir/flags.make

src/toralib/CMakeFiles/toralib.dir/simplesock.c.o: src/toralib/CMakeFiles/toralib.dir/flags.make
src/toralib/CMakeFiles/toralib.dir/simplesock.c.o: ../src/toralib/simplesock.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cjason/Desktop/secure-dist-computing/xsedar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/toralib/CMakeFiles/toralib.dir/simplesock.c.o"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/toralib.dir/simplesock.c.o   -c /home/cjason/Desktop/secure-dist-computing/xsedar/src/toralib/simplesock.c

src/toralib/CMakeFiles/toralib.dir/simplesock.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/toralib.dir/simplesock.c.i"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cjason/Desktop/secure-dist-computing/xsedar/src/toralib/simplesock.c > CMakeFiles/toralib.dir/simplesock.c.i

src/toralib/CMakeFiles/toralib.dir/simplesock.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/toralib.dir/simplesock.c.s"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cjason/Desktop/secure-dist-computing/xsedar/src/toralib/simplesock.c -o CMakeFiles/toralib.dir/simplesock.c.s

# Object files for target toralib
toralib_OBJECTS = \
"CMakeFiles/toralib.dir/simplesock.c.o"

# External object files for target toralib
toralib_EXTERNAL_OBJECTS =

src/toralib/libtoralib.a: src/toralib/CMakeFiles/toralib.dir/simplesock.c.o
src/toralib/libtoralib.a: src/toralib/CMakeFiles/toralib.dir/build.make
src/toralib/libtoralib.a: src/toralib/CMakeFiles/toralib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cjason/Desktop/secure-dist-computing/xsedar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtoralib.a"
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib && $(CMAKE_COMMAND) -P CMakeFiles/toralib.dir/cmake_clean_target.cmake
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/toralib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/toralib/CMakeFiles/toralib.dir/build: src/toralib/libtoralib.a

.PHONY : src/toralib/CMakeFiles/toralib.dir/build

src/toralib/CMakeFiles/toralib.dir/clean:
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib && $(CMAKE_COMMAND) -P CMakeFiles/toralib.dir/cmake_clean.cmake
.PHONY : src/toralib/CMakeFiles/toralib.dir/clean

src/toralib/CMakeFiles/toralib.dir/depend:
	cd /home/cjason/Desktop/secure-dist-computing/xsedar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cjason/Desktop/secure-dist-computing/xsedar /home/cjason/Desktop/secure-dist-computing/xsedar/src/toralib /home/cjason/Desktop/secure-dist-computing/xsedar/build /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib /home/cjason/Desktop/secure-dist-computing/xsedar/build/src/toralib/CMakeFiles/toralib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/toralib/CMakeFiles/toralib.dir/depend
