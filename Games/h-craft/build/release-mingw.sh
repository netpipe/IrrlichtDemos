#!/bin/sh

###########################################################
# command line parameters are language and distributor
# currently supported are: us ru irrgheist
# default is: us irrgheist
###########################################################

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    exit -1
    echo must be started from build folder
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
for i in $@
do
	case $i in 
		us) export lang=$i ;;
		ru) export lang=$i ;;
		irrgheist) export distributor=$i ;;
	esac
done
	
echo Using language $lang
echo Using distributor $distributor

HOVER_PATH=${TOOL_FOLDER}/..
MEDIA_PATH=${HOVER_PATH}/../media_hc1

if test -z "$RELEASE_PATH"
then
	export RELEASE_PATH=${TOOL_FOLDER}/../../releases_mingw/"$lang"_"$distributor"/hcraft
fi

rm -rf $RELEASE_PATH
mkdir -p $RELEASE_PATH
mkdir -p $RELEASE_PATH/doc
mkdir -p $RELEASE_PATH/ghosts

cp $HOVER_PATH/*.dll $RELEASE_PATH
cp $HOVER_PATH/hovercraft.exe $RELEASE_PATH/hcraft.exe
cp $HOVER_PATH/doc/HCraft_Manual.pdf $RELEASE_PATH/HCraft_Manual.pdf
cp $HOVER_PATH/doc/changes.txt $RELEASE_PATH/doc
cp -r ${MEDIA_PATH}/media $RELEASE_PATH

#currently shaders are only for gles2.0 - which is not used in release
#cp -r $HOVER_PATH/shaders $RELEASE_PATH

# put one file in the folder, as otherwise some installers will remove the folder
cp $HOVER_PATH/ghosts/ghosts.txt $RELEASE_PATH/ghosts

# copy&modify language specific files
if test "$lang" = ru
then
	cp -r ${MEDIA_PATH}/media_i18n/ru/* $RELEASE_PATH

	# sed in mingw does not yet know -i 
	cp $RELEASE_PATH/media/config.xml dummy.xml
	sed -e "s/strings_..\.xml/strings_ru.xml/g" dummy.xml > $RELEASE_PATH/media/config.xml
	rm -f dummy.xml
fi	
