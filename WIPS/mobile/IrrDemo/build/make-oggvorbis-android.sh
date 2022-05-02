export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

#libogg
export LIB_TARGET=${TOOL_FOLDER}/../libs/libogg/android
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libogg/libogg-1.3.1

cd ${LIB_SOURCE}/jni
ndk-build

mkdir -p ${LIB_TARGET}/include
cp -r ${LIB_SOURCE}/include ${LIB_TARGET}
mkdir -p ${LIB_TARGET}/lib
cp -r ${LIB_SOURCE}/obj/local/* ${LIB_TARGET}/lib

#tremor (vorbis-decoding)
export LIB_TARGET_TREMOR=${TOOL_FOLDER}/../libs/tremor/android
export LIB_SOURCE_TREMOR=${TOOL_FOLDER}/../libs/tremor/tremor-1.0.2

cd ${LIB_SOURCE_TREMOR}/jni
ndk-build OGG_INCLUDES=${LIB_TARGET}/include OGG_LIB=${LIB_TARGET}/lib

mkdir -p ${LIB_TARGET_TREMOR}/include
cp -r ${LIB_SOURCE_TREMOR}/*.h ${LIB_TARGET_TREMOR}/include
mkdir -p ${LIB_TARGET_TREMOR}/lib
cp -r ${LIB_SOURCE_TREMOR}/obj/local/* ${LIB_TARGET_TREMOR}/lib
