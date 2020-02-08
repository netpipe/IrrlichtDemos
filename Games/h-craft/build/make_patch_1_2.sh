#/bin/bash
# Make patch v1_2 from a release version
# (note: patches are only done for full version, so don't search for a demo script)

if test -z "$os"
then
	export target=all
fi

if [ $# -lt 2 ]
then
	echo "wrong syntax, use: $0 release_folder patch_folder [params]"
	echo "params are: linux windows (default is making one patch for both)"
	exit 1
fi

# check the command line parameters
for i in $@
do
	case $i in 
		linux) export target=$i ;;
		windows) export target=$i ;;

	esac
done

mkdir -p $2

# New files
mkdir -p $2/media/ui
cp $1/media/ui/hc_triplescreen01.jpg $2/media/ui/hc_triplescreen01.jpg
cp $1/media/ui/menu_intro.xml $2/media/ui/menu_intro.xml

if test "$target" = linux || test "$target" = all
then
	cp $1/swapsoundlibs.sh $2/swapsoundlibs.sh
fi

# Changed files
mkdir -p $2/doc
cp $1/doc/changes.txt $2/doc/changes.txt
if test "$target" = windows || test "$target" = all
then
	cp $1/hcraft.exe $2/hcraft.exe
	cp $1/Irrlicht.dll $2/Irrlicht.dll
fi
if test "$target" = linux || test "$target" = all
then
	cp $1/hcraft_bin $2/hcraft_bin
	mkdir -p $2/libs/linux_shared
	cp $1/libs/linux_shared/libopenal.so.1 $2/libs/linux_shared/libopenal.so.1
fi

cp $1/media/ui/menu_analog.xml $2/media/ui/menu_analog.xml
cp $1/media/ui/menu_arcade.xml $2/media/ui/menu_arcade.xml
cp $1/media/ui/menu_championship_progress.xml $2/media/ui/menu_championship_progress.xml
cp $1/media/ui/menu_championship.xml $2/media/ui/menu_championship.xml
cp $1/media/ui/menu_champ_winner.xml $2/media/ui/menu_champ_winner.xml
cp $1/media/ui/menu_controls.xml $2/media/ui/menu_controls.xml
cp $1/media/ui/menu_credits.xml $2/media/ui/menu_credits.xml
cp $1/media/ui/menu_graphic_setup.xml $2/media/ui/menu_graphic_setup.xml
cp $1/media/ui/menu_hcunlocked.xml $2/media/ui/menu_hcunlocked.xml
cp $1/media/ui/menu_highscores.xml $2/media/ui/menu_highscores.xml
cp $1/media/ui/menu_loadingscreen.xml $2/media/ui/menu_loadingscreen.xml
cp $1/media/ui/menu_main.xml $2/media/ui/menu_main.xml
cp $1/media/ui/menu_nagscreen.xml $2/media/ui/menu_nagscreen.xml
cp $1/media/ui/menu_newprofile.xml $2/media/ui/menu_newprofile.xml
cp $1/media/ui/menu_options.xml $2/media/ui/menu_options.xml
cp $1/media/ui/menu_profiles.xml $2/media/ui/menu_profiles.xml
cp $1/media/ui/menu_rivals_score.xml $2/media/ui/menu_rivals_score.xml
cp $1/media/ui/menu_rivals.xml $2/media/ui/menu_rivals.xml
cp $1/media/ui/menu_selecthover.xml $2/media/ui/menu_selecthover.xml
cp $1/media/ui/menu_timeattack.xml $2/media/ui/menu_timeattack.xml
cp $1/media/ui/menu_tutorial03.xml $2/media/ui/menu_tutorial03.xml
