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
CMAKE_SOURCE_DIR = /home/netpipe/Desktop/irrtum

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/netpipe/Desktop/irrtum

# Include any dependencies generated for this target.
include CMakeFiles/irrtum.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/irrtum.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/irrtum.dir/flags.make

CMakeFiles/irrtum.dir/pngwrite.c.o: CMakeFiles/irrtum.dir/flags.make
CMakeFiles/irrtum.dir/pngwrite.c.o: pngwrite.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/irrtum.dir/pngwrite.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/irrtum.dir/pngwrite.c.o   -c /home/netpipe/Desktop/irrtum/pngwrite.c

CMakeFiles/irrtum.dir/pngwrite.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/irrtum.dir/pngwrite.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/netpipe/Desktop/irrtum/pngwrite.c > CMakeFiles/irrtum.dir/pngwrite.c.i

CMakeFiles/irrtum.dir/pngwrite.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/irrtum.dir/pngwrite.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/netpipe/Desktop/irrtum/pngwrite.c -o CMakeFiles/irrtum.dir/pngwrite.c.s

CMakeFiles/irrtum.dir/pngwrite.c.o.requires:

.PHONY : CMakeFiles/irrtum.dir/pngwrite.c.o.requires

CMakeFiles/irrtum.dir/pngwrite.c.o.provides: CMakeFiles/irrtum.dir/pngwrite.c.o.requires
	$(MAKE) -f CMakeFiles/irrtum.dir/build.make CMakeFiles/irrtum.dir/pngwrite.c.o.provides.build
.PHONY : CMakeFiles/irrtum.dir/pngwrite.c.o.provides

CMakeFiles/irrtum.dir/pngwrite.c.o.provides.build: CMakeFiles/irrtum.dir/pngwrite.c.o


CMakeFiles/irrtum.dir/graybitmap.cpp.o: CMakeFiles/irrtum.dir/flags.make
CMakeFiles/irrtum.dir/graybitmap.cpp.o: graybitmap.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/irrtum.dir/graybitmap.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irrtum.dir/graybitmap.cpp.o -c /home/netpipe/Desktop/irrtum/graybitmap.cpp

CMakeFiles/irrtum.dir/graybitmap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irrtum.dir/graybitmap.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/netpipe/Desktop/irrtum/graybitmap.cpp > CMakeFiles/irrtum.dir/graybitmap.cpp.i

CMakeFiles/irrtum.dir/graybitmap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irrtum.dir/graybitmap.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/netpipe/Desktop/irrtum/graybitmap.cpp -o CMakeFiles/irrtum.dir/graybitmap.cpp.s

CMakeFiles/irrtum.dir/graybitmap.cpp.o.requires:

.PHONY : CMakeFiles/irrtum.dir/graybitmap.cpp.o.requires

CMakeFiles/irrtum.dir/graybitmap.cpp.o.provides: CMakeFiles/irrtum.dir/graybitmap.cpp.o.requires
	$(MAKE) -f CMakeFiles/irrtum.dir/build.make CMakeFiles/irrtum.dir/graybitmap.cpp.o.provides.build
.PHONY : CMakeFiles/irrtum.dir/graybitmap.cpp.o.provides

CMakeFiles/irrtum.dir/graybitmap.cpp.o.provides.build: CMakeFiles/irrtum.dir/graybitmap.cpp.o


CMakeFiles/irrtum.dir/irrtum.cpp.o: CMakeFiles/irrtum.dir/flags.make
CMakeFiles/irrtum.dir/irrtum.cpp.o: irrtum.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/irrtum.dir/irrtum.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irrtum.dir/irrtum.cpp.o -c /home/netpipe/Desktop/irrtum/irrtum.cpp

CMakeFiles/irrtum.dir/irrtum.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irrtum.dir/irrtum.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/netpipe/Desktop/irrtum/irrtum.cpp > CMakeFiles/irrtum.dir/irrtum.cpp.i

CMakeFiles/irrtum.dir/irrtum.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irrtum.dir/irrtum.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/netpipe/Desktop/irrtum/irrtum.cpp -o CMakeFiles/irrtum.dir/irrtum.cpp.s

CMakeFiles/irrtum.dir/irrtum.cpp.o.requires:

.PHONY : CMakeFiles/irrtum.dir/irrtum.cpp.o.requires

CMakeFiles/irrtum.dir/irrtum.cpp.o.provides: CMakeFiles/irrtum.dir/irrtum.cpp.o.requires
	$(MAKE) -f CMakeFiles/irrtum.dir/build.make CMakeFiles/irrtum.dir/irrtum.cpp.o.provides.build
.PHONY : CMakeFiles/irrtum.dir/irrtum.cpp.o.provides

CMakeFiles/irrtum.dir/irrtum.cpp.o.provides.build: CMakeFiles/irrtum.dir/irrtum.cpp.o


CMakeFiles/irrtum.dir/intervallist.cpp.o: CMakeFiles/irrtum.dir/flags.make
CMakeFiles/irrtum.dir/intervallist.cpp.o: intervallist.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/irrtum.dir/intervallist.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irrtum.dir/intervallist.cpp.o -c /home/netpipe/Desktop/irrtum/intervallist.cpp

CMakeFiles/irrtum.dir/intervallist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irrtum.dir/intervallist.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/netpipe/Desktop/irrtum/intervallist.cpp > CMakeFiles/irrtum.dir/intervallist.cpp.i

CMakeFiles/irrtum.dir/intervallist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irrtum.dir/intervallist.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/netpipe/Desktop/irrtum/intervallist.cpp -o CMakeFiles/irrtum.dir/intervallist.cpp.s

CMakeFiles/irrtum.dir/intervallist.cpp.o.requires:

.PHONY : CMakeFiles/irrtum.dir/intervallist.cpp.o.requires

CMakeFiles/irrtum.dir/intervallist.cpp.o.provides: CMakeFiles/irrtum.dir/intervallist.cpp.o.requires
	$(MAKE) -f CMakeFiles/irrtum.dir/build.make CMakeFiles/irrtum.dir/intervallist.cpp.o.provides.build
.PHONY : CMakeFiles/irrtum.dir/intervallist.cpp.o.provides

CMakeFiles/irrtum.dir/intervallist.cpp.o.provides.build: CMakeFiles/irrtum.dir/intervallist.cpp.o


CMakeFiles/irrtum.dir/color.cpp.o: CMakeFiles/irrtum.dir/flags.make
CMakeFiles/irrtum.dir/color.cpp.o: color.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/irrtum.dir/color.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irrtum.dir/color.cpp.o -c /home/netpipe/Desktop/irrtum/color.cpp

CMakeFiles/irrtum.dir/color.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irrtum.dir/color.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/netpipe/Desktop/irrtum/color.cpp > CMakeFiles/irrtum.dir/color.cpp.i

CMakeFiles/irrtum.dir/color.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irrtum.dir/color.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/netpipe/Desktop/irrtum/color.cpp -o CMakeFiles/irrtum.dir/color.cpp.s

CMakeFiles/irrtum.dir/color.cpp.o.requires:

.PHONY : CMakeFiles/irrtum.dir/color.cpp.o.requires

CMakeFiles/irrtum.dir/color.cpp.o.provides: CMakeFiles/irrtum.dir/color.cpp.o.requires
	$(MAKE) -f CMakeFiles/irrtum.dir/build.make CMakeFiles/irrtum.dir/color.cpp.o.provides.build
.PHONY : CMakeFiles/irrtum.dir/color.cpp.o.provides

CMakeFiles/irrtum.dir/color.cpp.o.provides.build: CMakeFiles/irrtum.dir/color.cpp.o


CMakeFiles/irrtum.dir/main.cpp.o: CMakeFiles/irrtum.dir/flags.make
CMakeFiles/irrtum.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/irrtum.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irrtum.dir/main.cpp.o -c /home/netpipe/Desktop/irrtum/main.cpp

CMakeFiles/irrtum.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irrtum.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/netpipe/Desktop/irrtum/main.cpp > CMakeFiles/irrtum.dir/main.cpp.i

CMakeFiles/irrtum.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irrtum.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/netpipe/Desktop/irrtum/main.cpp -o CMakeFiles/irrtum.dir/main.cpp.s

CMakeFiles/irrtum.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/irrtum.dir/main.cpp.o.requires

CMakeFiles/irrtum.dir/main.cpp.o.provides: CMakeFiles/irrtum.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/irrtum.dir/build.make CMakeFiles/irrtum.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/irrtum.dir/main.cpp.o.provides

CMakeFiles/irrtum.dir/main.cpp.o.provides.build: CMakeFiles/irrtum.dir/main.cpp.o


# Object files for target irrtum
irrtum_OBJECTS = \
"CMakeFiles/irrtum.dir/pngwrite.c.o" \
"CMakeFiles/irrtum.dir/graybitmap.cpp.o" \
"CMakeFiles/irrtum.dir/irrtum.cpp.o" \
"CMakeFiles/irrtum.dir/intervallist.cpp.o" \
"CMakeFiles/irrtum.dir/color.cpp.o" \
"CMakeFiles/irrtum.dir/main.cpp.o"

# External object files for target irrtum
irrtum_EXTERNAL_OBJECTS =

bin/irrtum: CMakeFiles/irrtum.dir/pngwrite.c.o
bin/irrtum: CMakeFiles/irrtum.dir/graybitmap.cpp.o
bin/irrtum: CMakeFiles/irrtum.dir/irrtum.cpp.o
bin/irrtum: CMakeFiles/irrtum.dir/intervallist.cpp.o
bin/irrtum: CMakeFiles/irrtum.dir/color.cpp.o
bin/irrtum: CMakeFiles/irrtum.dir/main.cpp.o
bin/irrtum: CMakeFiles/irrtum.dir/build.make
bin/irrtum: /usr/lib64/libz.so
bin/irrtum: /usr/lib64/libpng.so
bin/irrtum: /usr/lib64/libz.so
bin/irrtum: /usr/lib64/libfreetype.so
bin/irrtum: /usr/lib64/libpopt.so
bin/irrtum: /usr/lib64/libpng.so
bin/irrtum: /usr/lib64/libfreetype.so
bin/irrtum: /usr/lib64/libpopt.so
bin/irrtum: CMakeFiles/irrtum.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/netpipe/Desktop/irrtum/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable bin/irrtum"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/irrtum.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/irrtum.dir/build: bin/irrtum

.PHONY : CMakeFiles/irrtum.dir/build

CMakeFiles/irrtum.dir/requires: CMakeFiles/irrtum.dir/pngwrite.c.o.requires
CMakeFiles/irrtum.dir/requires: CMakeFiles/irrtum.dir/graybitmap.cpp.o.requires
CMakeFiles/irrtum.dir/requires: CMakeFiles/irrtum.dir/irrtum.cpp.o.requires
CMakeFiles/irrtum.dir/requires: CMakeFiles/irrtum.dir/intervallist.cpp.o.requires
CMakeFiles/irrtum.dir/requires: CMakeFiles/irrtum.dir/color.cpp.o.requires
CMakeFiles/irrtum.dir/requires: CMakeFiles/irrtum.dir/main.cpp.o.requires

.PHONY : CMakeFiles/irrtum.dir/requires

CMakeFiles/irrtum.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/irrtum.dir/cmake_clean.cmake
.PHONY : CMakeFiles/irrtum.dir/clean

CMakeFiles/irrtum.dir/depend:
	cd /home/netpipe/Desktop/irrtum && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/netpipe/Desktop/irrtum /home/netpipe/Desktop/irrtum /home/netpipe/Desktop/irrtum /home/netpipe/Desktop/irrtum /home/netpipe/Desktop/irrtum/CMakeFiles/irrtum.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/irrtum.dir/depend

