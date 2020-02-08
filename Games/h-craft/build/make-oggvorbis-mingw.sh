#!/bin/sh

# Make the ogg-vorbis libs on MinGW

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

export CFLAGS="-O3 -s -Wall"
export CXXFLAGS="-O3 -s -Wall"

#libogg
export LIB_TARGET=${TOOL_FOLDER}/../libs/libogg/Win32-gcc
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libogg/libogg-1.3.1

mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
# fails here - run autogen in the $LIB_SOURCE folder. 
# Sorry, my knowledge of autotools ain't sufficient to figure out a solution 
# that works on Linux+Windows with the same sources it seems.
$LIB_SOURCE/configure --prefix=$LIB_TARGET --enable-shared=no
if [ -ne 0]
then
	$LIB_SOURCE/autoconf.sh
	$LIB_SOURCE/configure --prefix=$LIB_TARGET --enable-shared=no
fi
 
make
make install

#libvorbis
export CPPFLAGS="-I${LIB_TARGET}/include"
export LDFLAGS="-L${LIB_TARGET}/lib"
export LIB_TARGET=${TOOL_FOLDER}/../libs/libvorbis/Win32-gcc
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libvorbis/libvorbis-1.3.4
    
mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
# same as above - when it fails run autogen
$LIB_SOURCE/configure --prefix=$LIB_TARGET --enable-shared=no
make
make install
