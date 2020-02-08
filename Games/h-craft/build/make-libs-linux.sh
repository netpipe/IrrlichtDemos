#!/bin/sh
# Call to rebuild all libraries which are needed for h-craft.
# Must be called from current directory (hover/build)
# Libraries will also be copied to the h-craft game folder.

./make-freetype-linux.sh
./make-sdl-linux.sh
./make-oggvorbis-linux.sh
./make-openal-linux.sh
./make-shared-linux-libs.sh
