#!/bin/bash

if [ ! $1 ];
		then cmake .; make
fi

if [ "$1" = "verbose" ];
		then cmake .;make VERBOSE=1
fi

if [ "$1" = "clean" ];
		then cmake .;
make clean;
rm -rvf CMakeFiles;
rm -rvf cmake_install.cmake;
rm -rvf CMakeCache.txt;
rm -rvf Makefile;

rm -rvf tests/CMakeFiles;
rm -rvf tests/cmake_install.cmake;
rm -rvf tests/Makefile;

rm -rvf engine/CMakeFiles;
rm -rvf engine/Makefile;
rm -rvf engine/cmake_install.cmake;

rm -rvf engine/src/CMakeFiles;
rm -rvf engine/src/Makefile;
rm -rvf engine/src/cmake_install.cmake
fi
