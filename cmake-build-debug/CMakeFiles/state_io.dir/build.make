# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\programs\CLion 2021.3.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\programs\CLion 2021.3.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\project1400\project-asemanehnafe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\project1400\project-asemanehnafe\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/state_io.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/state_io.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/state_io.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/state_io.dir/flags.make

CMakeFiles/state_io.dir/src/main.c.obj: CMakeFiles/state_io.dir/flags.make
CMakeFiles/state_io.dir/src/main.c.obj: CMakeFiles/state_io.dir/includes_C.rsp
CMakeFiles/state_io.dir/src/main.c.obj: ../src/main.c
CMakeFiles/state_io.dir/src/main.c.obj: CMakeFiles/state_io.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\project1400\project-asemanehnafe\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/state_io.dir/src/main.c.obj"
	"D:\programs\CLion 2021.3.1\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/state_io.dir/src/main.c.obj -MF CMakeFiles\state_io.dir\src\main.c.obj.d -o CMakeFiles\state_io.dir\src\main.c.obj -c D:\project1400\project-asemanehnafe\src\main.c

CMakeFiles/state_io.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/state_io.dir/src/main.c.i"
	"D:\programs\CLion 2021.3.1\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\project1400\project-asemanehnafe\src\main.c > CMakeFiles\state_io.dir\src\main.c.i

CMakeFiles/state_io.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/state_io.dir/src/main.c.s"
	"D:\programs\CLion 2021.3.1\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\project1400\project-asemanehnafe\src\main.c -o CMakeFiles\state_io.dir\src\main.c.s

# Object files for target state_io
state_io_OBJECTS = \
"CMakeFiles/state_io.dir/src/main.c.obj"

# External object files for target state_io
state_io_EXTERNAL_OBJECTS =

state_io.exe: CMakeFiles/state_io.dir/src/main.c.obj
state_io.exe: CMakeFiles/state_io.dir/build.make
state_io.exe: ../sdl2/sdl2-lib/libSDL2.a
state_io.exe: ../sdl2/sdl2-lib/libSDL2main.a
state_io.exe: ../sdl2/sdl2-gfx-lib/libsdl-gfx.a
state_io.exe: CMakeFiles/state_io.dir/linklibs.rsp
state_io.exe: CMakeFiles/state_io.dir/objects1.rsp
state_io.exe: CMakeFiles/state_io.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\project1400\project-asemanehnafe\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable state_io.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\state_io.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/state_io.dir/build: state_io.exe
.PHONY : CMakeFiles/state_io.dir/build

CMakeFiles/state_io.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\state_io.dir\cmake_clean.cmake
.PHONY : CMakeFiles/state_io.dir/clean

CMakeFiles/state_io.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\project1400\project-asemanehnafe D:\project1400\project-asemanehnafe D:\project1400\project-asemanehnafe\cmake-build-debug D:\project1400\project-asemanehnafe\cmake-build-debug D:\project1400\project-asemanehnafe\cmake-build-debug\CMakeFiles\state_io.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/state_io.dir/depend
