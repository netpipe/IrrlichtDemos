# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3

# Include any dependencies generated for this target.
include Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/depend.make

# Include the progress variables for this target.
include Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/progress.make

# Include the compile flags for this target's objects.
include Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/flags.make

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/flags.make
Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o: Horde3D/Source/Horde3DUtils/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o"
	cd /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Horde3DUtils.dir/main.cpp.o -c /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils/main.cpp

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Horde3DUtils.dir/main.cpp.i"
	cd /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils/main.cpp > CMakeFiles/Horde3DUtils.dir/main.cpp.i

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Horde3DUtils.dir/main.cpp.s"
	cd /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils/main.cpp -o CMakeFiles/Horde3DUtils.dir/main.cpp.s

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.requires:
.PHONY : Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.requires

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.provides: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.requires
	$(MAKE) -f Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/build.make Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.provides.build
.PHONY : Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.provides

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.provides.build: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o

# Object files for target Horde3DUtils
Horde3DUtils_OBJECTS = \
"CMakeFiles/Horde3DUtils.dir/main.cpp.o"

# External object files for target Horde3DUtils
Horde3DUtils_EXTERNAL_OBJECTS =

Horde3D/Source/Horde3DUtils/libHorde3DUtils.so: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o
Horde3D/Source/Horde3DUtils/libHorde3DUtils.so: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/build.make
Horde3D/Source/Horde3DUtils/libHorde3DUtils.so: Horde3D/Source/Horde3DEngine/libHorde3D.so
Horde3D/Source/Horde3DUtils/libHorde3DUtils.so: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libHorde3DUtils.so"
	cd /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Horde3DUtils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/build: Horde3D/Source/Horde3DUtils/libHorde3DUtils.so
.PHONY : Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/build

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/requires: Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/main.cpp.o.requires
.PHONY : Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/requires

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/clean:
	cd /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils && $(CMAKE_COMMAND) -P CMakeFiles/Horde3DUtils.dir/cmake_clean.cmake
.PHONY : Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/clean

Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/depend:
	cd /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3 /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3 /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils /home/Dev/libs/game/PeakEngines/PeakEngine/old/peakengine-luna/lib/Horde3/Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Horde3D/Source/Horde3DUtils/CMakeFiles/Horde3DUtils.dir/depend

