#!/bin/sh
# build freetype library on mingw
# TODO: this simply does not work anymore. Have to try newer freetype.

export CFLAGS="-O3 -s -Wall"
export CXXFLAGS="-O3 -s -Wall"

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

export LIB_TARGET=${TOOL_FOLDER}/../libs/libfreetype/Win32-gcc
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libfreetype/freetype-2.5.2

mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
${LIB_SOURCE}/configure --prefix=$LIB_TARGET --without-zlib --without-png --enable-shared=no
# when mingw detection fails we must copy manually (but seems to work by now)
#rm -f config.mk
#cp ${LIB_SOURCE}/builds/win32/w32-mingw32.mk config.mk
make
make install
