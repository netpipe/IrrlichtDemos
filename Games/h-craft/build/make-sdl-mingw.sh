#!/bin/sh

# Build the sdl library on MinGW from scratch

export CFLAGS="-O2 -s -Wall"
export CXXFLAGS="-O2 -s -Wall"

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

export LIB_TARGET=${TOOL_FOLDER}/../libs/sdl/Win32-gcc
export LIB_SOURCE=${TOOL_FOLDER}/../libs/sdl/SDL-1.2.15

mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
# Note: --disable-video not working in 1.2.15, otherwise I would set that additionally.
$LIB_SOURCE/configure --prefix=$LIB_TARGET --disable-static --disable-audio --disable-cdrom --disable-threads --disable-timers --disable-file --disable-loadso --disable-input-tslib --disable-directx
make 
make install
cp ${LIB_TARGET}/bin/SDL.dll ${TOOL_FOLDER}/..
