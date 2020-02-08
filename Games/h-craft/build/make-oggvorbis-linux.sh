export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

#libogg
export LIB_TARGET=${TOOL_FOLDER}/../libs/libogg/linux
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libogg/libogg-1.3.1

rm -rf ${LIB_TARGET}/build
mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
$LIB_SOURCE/configure --prefix=$LIB_TARGET --enable-shared=no 
make
make install

#libvorbis
export CPPFLAGS="-I${LIB_TARGET}/include"
export LDFLAGS="-L${LIB_TARGET}/lib"
export LIB_TARGET=${TOOL_FOLDER}/../libs/libvorbis/linux
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libvorbis/libvorbis-1.3.4

rm -rf ${LIB_TARGET}/build
mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
$LIB_SOURCE/configure --prefix=$LIB_TARGET --enable-shared=no 
make
make install
