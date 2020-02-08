# Create a release for the (heavily patched) Irrlicht library which we are using.
# Will create a tar.gz to put on the web.

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

ENGINE_SOURCE_PATH=${TOOL_FOLDER}/../libs/irrlicht
ENGINE_NAME=irrlicht_hcraft_v1_3
ENGINE_DEST_PATH=${TOOL_FOLDER}/../../releases/$ENGINE_NAME

rm -rf $ENGINE_DEST_PATH
mkdir -p $ENGINE_DEST_PATH

cp -r $ENGINE_SOURCE_PATH/include $ENGINE_DEST_PATH
cp -r $ENGINE_SOURCE_PATH/source $ENGINE_DEST_PATH
find $ENGINE_DEST_PATH -name *.o -exec rm {} \;
rm -f $ENGINE_DEST_PATH/source/Irrlicht/*.txt
rm -f $ENGINE_DEST_PATH/source/Irrlicht/*.vcproj
rm -f $ENGINE_DEST_PATH/source/Irrlicht/*.depend
rm -f $ENGINE_DEST_PATH/source/Irrlicht/*.dsp
rm -f $ENGINE_DEST_PATH/source/Irrlicht/*.dev
rm -f $ENGINE_DEST_PATH/source/Irrlicht/Makefile*
rm -rf $ENGINE_DEST_PATH/source/Irrlicht/.objs
rm -rf $ENGINE_DEST_PATH/source/Irrlicht/jpeglib
rm -rf $ENGINE_DEST_PATH/source/Irrlicht/libpng
rm -rf $ENGINE_DEST_PATH/source/Irrlicht/zlib	
rm -rf $ENGINE_DEST_PATH/source/Irrlicht/arch

cd $ENGINE_DEST_PATH/..
tar cvzf $ENGINE_NAME.tar.gz $ENGINE_NAME
