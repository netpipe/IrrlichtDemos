#!/bin/sh
#build openal-soft from scratch on Mingw
export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

#openal
export LIB_TARGET=${TOOL_FOLDER}/../libs/openal/Win32-gcc
export LIB_SOURCE=${TOOL_FOLDER}/../libs/openal/openal-soft-1.12.854

# Probably can't distribute dsound.h legally (thx MS for making it hard to support their OS...).
# So if you want sound you have to install the dx_sdk.
# Then this should copy it to a place where it can be used to compile openal with sound.
# No idea if this will still work on Windows 8.
mkdir -p "${TOOL_FOLDER}/../libs/directx/include"
export DX_INCLUDE="${TOOL_FOLDER}/../libs/directx/include"
cp "${DXSDK_DIR}/include/dsound.h" "${DX_INCLUDE}"

rm -rf ${LIB_TARGET}/build
mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
cmake ${LIB_SOURCE} -G"MSYS Makefiles" -D"CMAKE_INSTALL_PREFIX:PATH=${LIB_TARGET}" -D"CMAKE_REQUIRED_INCLUDES=${DX_INCLUDE}" -D"OpenAL_BINARY_DIR=${DX_INCLUDE}"
make
make install
mkdir -p ${LIB_TARGET}/include
cp -r ${LIB_SOURCE}/include ${LIB_TARGET}

#freealut
export LIB_TARGET_ALUT=${TOOL_FOLDER}/../libs/freealut/Win32-gcc
export LIB_SOURCE_ALUT=${TOOL_FOLDER}/../libs/freealut/freealut-1.1.0

rm -rf ${LIB_TARGET_ALUT}/build
mkdir -p ${LIB_TARGET_ALUT}/build
cd ${LIB_TARGET_ALUT}/build
cmake ${LIB_SOURCE_ALUT} -G"MSYS Makefiles" -D"CMAKE_INSTALL_PREFIX:PATH=${LIB_TARGET_ALUT}" -D"OPENAL_INCLUDE_DIR=${LIB_TARGET}/include" -D"OPENAL_LIBRARY=${LIB_TARGET}/lib/libOpenAL32.dll.a" -D"BUILD_STATIC=OFF"
make
make install
