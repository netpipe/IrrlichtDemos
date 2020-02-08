#!/bin/sh
# Call to rebuild all libraries which are needed for h-craft.
# Must be called from msys/mingw.
# Must be called from current directory (hover/build)

./make-freetype-mingw.sh
./make-sdl-mingw.sh
./make-openal-mingw.sh
./make-oggvorbis-mingw.sh
