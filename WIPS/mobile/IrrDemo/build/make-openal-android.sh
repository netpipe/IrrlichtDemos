# build openal-soft and freealut for Android 
# (they are build together because freealut needs to know openal lib/include paths)
export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

if test -z "$TARGET_ARCH_ABI"
then
	export TARGET_ARCH_ABI=armeabi
	echo TARGET_ARCH_ABI is $TARGET_ARCH_ABI
fi

if test -z "$ANDROID_NDK_ROOT"
then
	export ANDROID_NDK_ROOT=$ANDROID_NDK
	echo ANDROID_NDK_ROOT is $ANDROID_NDK_ROOT
fi

#openal
export LIB_TARGET=${TOOL_FOLDER}/../libs/openal/android
export LIB_SOURCE=${TOOL_FOLDER}/../libs/openal/openal-soft-apportable

cd ${LIB_SOURCE}/jni
ndk-build POST_FROYO=yes MAX_SOURCES_LOW=32 MAX_SOURCES_START=32 MAX_SOURCES_HIGH=32

mkdir -p ${LIB_TARGET}/include
cp -r ${LIB_SOURCE}/jni/OpenAL/include ${LIB_TARGET}
mkdir -p ${LIB_TARGET}/lib
cp -r ${LIB_SOURCE}/libs/* ${LIB_TARGET}/lib


#freealut
export LIB_TARGET_ALUT=${TOOL_FOLDER}/../libs/freealut/android
export LIB_SOURCE_ALUT=${TOOL_FOLDER}/../libs/freealut/freealut-1.1.0

cd ${LIB_SOURCE_ALUT}/jni
ndk-build OPENAL_INCLUDES=${LIB_TARGET}/include OPENAL_LIB=${LIB_TARGET}/lib

mkdir -p ${LIB_TARGET_ALUT}/include
cp -r ${LIB_SOURCE_ALUT}/include ${LIB_TARGET_ALUT}
mkdir -p ${LIB_TARGET_ALUT}/lib
cp -r ${LIB_SOURCE_ALUT}/libs/* ${LIB_TARGET_ALUT}/lib
