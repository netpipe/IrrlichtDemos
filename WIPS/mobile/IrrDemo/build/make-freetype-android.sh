export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

export LIB_TARGET=${TOOL_FOLDER}/../libs/libfreetype/android
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libfreetype/freetype-2.5.2

# Need to set up a cross-platform environment
if [ -z "$ANDROID_NDK" ]
then 
	echo "ANDROID_NDK must be set to your local Android-ndk path"
	exit 1
fi
if [ -z "$NDK_PLATFORM" ]
then 
	export NDK_PLATFORM=android-10
fi
$ANDROID_NDK/build/tools/make-standalone-toolchain.sh  --platform=$NDK_PLATFORM --install-dir=$LIB_TARGET/ndk
PATH=$LIB_TARGET/ndk/bin:$PATH
echo $PATH

mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
make clean
${LIB_SOURCE}/configure CPPFLAGS=-O2 --prefix=$LIB_TARGET --without-zlib --without-png --host=arm-linux-androideabi
make
make install
