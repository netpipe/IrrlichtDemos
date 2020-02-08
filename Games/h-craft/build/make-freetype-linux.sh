export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

export LIB_TARGET=${TOOL_FOLDER}/../libs/libfreetype/linux
export LIB_SOURCE=${TOOL_FOLDER}/../libs/libfreetype/freetype-2.5.2

mkdir -p ${LIB_TARGET}/build
cd ${LIB_TARGET}/build
make clean
# TODO: also --without-png ?
${LIB_SOURCE}/configure CPPFLAGS=-O2 --prefix=$LIB_TARGET --without-zlib --enable-shared=no
make
make install
