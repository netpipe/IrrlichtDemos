# copy the shared libs which we distribute with binary releases into SHARED_TARGET

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

LIBS_TARGET=../libs
SHARED_TARGET=$LIBS_TARGET/linux_shared

rm -rf $SHARED_TARGET
mkdir $SHARED_TARGET
cp $LIBS_TARGET/freealut/linux/lib/libalut.so.0 $SHARED_TARGET
cp $LIBS_TARGET/openal/linux/lib/libopenal.so.1 $SHARED_TARGET
cp $LIBS_TARGET/sdl/linux/lib/libSDL-1.2.so.0 $SHARED_TARGET

# That was for 32-bit, 64-bit libs are named a little different
# doesn't matter for now as I don't plan on releasing this version on Linux64 for now, but just has to run locally
#cp /usr/lib/libstdc++.so.6 $SHARED_TARGET
#cp /lib/libgcc_s.so.1 $SHARED_TARGET
#chmod -x $SHARED_TARGET/*
