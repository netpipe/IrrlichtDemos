# build openal-soft and freealut on GNU/Linux
# (they are build together because freealut needs to know openal lib/include paths)
export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

#openal
export LIB_TARGET=${TOOL_FOLDER}/../libs/openal/linux
export LIB_SOURCE=${TOOL_FOLDER}/../libs/openal/openal-soft-1.12.854

rm -rf ${LIB_TARGET}/build
mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
cmake ${LIB_SOURCE} -D"LIBRARY_OUTPUT_PATH=${LIB_TARGET}/lib" -DPORTAUDIO=OFF
make
mkdir -p ${LIB_TARGET}/include
cp -r ${LIB_SOURCE}/include ${LIB_TARGET}

#freealut
export LIB_TARGET_ALUT=${TOOL_FOLDER}/../libs/freealut/linux
export LIB_SOURCE_ALUT=${TOOL_FOLDER}/../libs/freealut/freealut-1.1.0

rm -rf ${LIB_TARGET_ALUT}/build
mkdir -p ${LIB_TARGET_ALUT}/build
cd ${LIB_TARGET_ALUT}/build
cmake ${LIB_SOURCE_ALUT} -D"LIBRARY_OUTPUT_PATH=${LIB_TARGET_ALUT}/lib" -D"OPENAL_INCLUDE_DIR=${LIB_TARGET}/include" -D"OPENAL_LIBRARY=${LIB_TARGET}/lib"
make
mkdir -p ${LIB_TARGET_ALUT}/include
cp -r ${LIB_SOURCE_ALUT}/include ${LIB_TARGET_ALUT}

