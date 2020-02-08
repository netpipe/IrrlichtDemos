#!/bin/sh

###########################################################
# command line parameters are language and distributor
# currently supported are: us ru irrgheist
# default is: us irrgheist
###########################################################

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi


if test -z "$lang"
then
	export lang=us
fi
	
if test -z "$distributor"
then
	export distributor=irrgheist
fi

# check the command line parameters
# check the command line parameters
for i in $@
do
	case $i in 
		us) export lang=$i ;;
		ru) export lang=$i ;;
		irrgheist) export distributor=$i ;;
	esac
done

HOVER_PATH=${TOOL_FOLDER}/..
MEDIA_PATH=${HOVER_PATH}/../media_hc1

if test -z "$RELEASE_PATH"
then
	export RELEASE_PATH=${TOOL_FOLDER}/../../releases_mingw/"$lang"_"$distributor"/hcraft_demo
fi

# copy usual release files
./release-mingw.sh $@
	
# copy additional demo media
cp $RELEASE_PATH/hcraft.exe $RELEASE_PATH/hcraft_demo.exe
rm -f $RELEASE_PATH/hcraft.exe
#cp $HOVER_PATH/doc/readme*.txt $RELEASE_PATH
cp -r $MEDIA_PATH/media_demo/* $RELEASE_PATH/media

# delete media not used in demo
rm -f $RELEASE_PATH/media/levels/track0?*
rm -f $RELEASE_PATH/media/textures/lvl0?lm01.jpg
rm -f $RELEASE_PATH/media/levels/track10*
rm -f $RELEASE_PATH/media/textures/lvl10lm01.jpg
rm -f $RELEASE_PATH/media/levels/track11*
rm -f $RELEASE_PATH/media/textures/lvl11lm01.jpg
rm -f $RELEASE_PATH/media/levels/track12*
rm -f $RELEASE_PATH/media/textures/lvl12lm01.jpg
rm -f $RELEASE_PATH/media/levels/track13*
rm -f $RELEASE_PATH/media/textures/lvl13lm01.jpg
rm -f $RELEASE_PATH/media/levels/track14*
rm -f $RELEASE_PATH/media/textures/lvl14lm01.jpg
rm -f $RELEASE_PATH/media/levels/track15*	
rm -f $RELEASE_PATH/media/textures/lvl15lm01.jpg
rm -f $RELEASE_PATH/media/levels/track16*
rm -f $RELEASE_PATH/media/textures/lvl16lm01.jpg
rm -f $RELEASE_PATH/media/levels/track17*
rm -f $RELEASE_PATH/media/textures/lvl17lm01.jpg
rm -f $RELEASE_PATH/media/levels/track19*
rm -f $RELEASE_PATH/media/textures/lvl19lm01.jpg
rm -f $RELEASE_PATH/media/levels/track20*
rm -f $RELEASE_PATH/media/textures/lvl20lm01.jpg
rm -f $RELEASE_PATH/media/levels/track21*
rm -f $RELEASE_PATH/media/textures/lvl21lm01.jpg
rm -f $RELEASE_PATH/media/levels/track22*
rm -f $RELEASE_PATH/media/textures/lvl22lm01.jpg
rm -f $RELEASE_PATH/media/levels/track23*
rm -f $RELEASE_PATH/media/textures/lvl23lm01.jpg
rm -f $RELEASE_PATH/media/levels/track24*
rm -f $RELEASE_PATH/media/textures/lvl24lm01.jpg
rm -f $RELEASE_PATH/media/levels/track25*
rm -f $RELEASE_PATH/media/textures/lvl25lm01.jpg
rm -f $RELEASE_PATH/media/levels/track27*
rm -f $RELEASE_PATH/media/textures/lvl27lm01.jpg
rm -f $RELEASE_PATH/media/levels/track29*
rm -f $RELEASE_PATH/media/textures/lvl29lm01.jpg
rm -f $RELEASE_PATH/media/levels/track3?.dat
rm -f $RELEASE_PATH/media/textures/lvl3?lm01.jpg
#rm -f $RELEASE_PATH/media/sounds/music_level/Manolo Camp\ -\ Dream*

