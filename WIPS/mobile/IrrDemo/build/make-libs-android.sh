#!/bin/sh
# Call to rebuild all libraries which are needed for h-craft.
# Must be called from current directory (hover/build)
# Libraries will also be copied to the h-craft game folder.

./make-freetype-android.sh
./make-openal-android.sh
./make-oggvorbis-android.sh
./make-google-play-services-android.sh
