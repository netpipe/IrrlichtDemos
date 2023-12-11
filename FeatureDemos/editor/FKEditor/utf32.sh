#!/bin/sh

#use this on irrlicht meshes exported from 64bit blender or irrlicht itself to be able to easily import into blender or irrlicht. iirc

if test $# = 0
then
        echo usage: utf32to8 source [target]
        exit 1
fi
iconv -f UTF-32 -t UTF-8 $1 > new.irr
#then
#        mv .utf32to8dummy ${2:-1$1}
#        rm -f .utf32to8dummy
#fi
