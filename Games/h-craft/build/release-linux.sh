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
	export RELEASE_PATH=${TOOL_FOLDER}/../../releases/"$lang"_"$distributor"/hcraft
fi


./make-shared-linux-libs.sh $@

rm -rf $RELEASE_PATH
mkdir -p $RELEASE_PATH
mkdir -p $RELEASE_PATH/doc
mkdir -p $RELEASE_PATH/ghosts

if [ -f $HOVER_PATH/hovercraft ];
then
   cp $HOVER_PATH/hovercraft $RELEASE_PATH/hcraft_bin
else
   cp $HOVER_PATH/hovercraft64 $RELEASE_PATH/hcraft_bin
fi
cp $HOVER_PATH/hcraft.sh $RELEASE_PATH/hcraft
cp $HOVER_PATH/swapsoundlibs.sh $RELEASE_PATH
cp $HOVER_PATH/open_browser.sh $RELEASE_PATH/open_browser.sh
cp $HOVER_PATH/doc/HCraft_Manual.pdf $RELEASE_PATH/HCraft_Manual.pdf
cp $HOVER_PATH/doc/changes.txt $RELEASE_PATH/doc

cp -r $MEDIA_PATH/media $RELEASE_PATH

#currently shaders are only for gles2.0 - which is not used in release	
#cp -r $HOVER_PATH/shaders $RELEASE_PATH	

# put one file in the folder, as otherwise some installers will remove the folder
cp $HOVER_PATH/ghosts/ghosts.txt $RELEASE_PATH/ghosts

mkdir -p $RELEASE_PATH/libs
cp -r $HOVER_PATH/libs/linux_shared $RELEASE_PATH/libs
	
# copy&modify language specific files
if test "$lang" = ru
then
	cp -r $MEDIA_PATH/media_i18n/ru/* $RELEASE_PATH
	sed -i "s/strings_..\.xml/strings_ru.xml/g" $RELEASE_PATH/media/config.xml
fi	
