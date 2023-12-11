#!/bin/bash
#set_env.sh

cd SDL2
#auto.sh
#this configure line should work ?  if not use the one in that came with sdl
emconfigure ./configure --host=asmjs-unknown-emscripten --disable-assembly --disable-threads --disable-mmx --disable-sdltest --disable-shared --enable-cpuinfo=false CFLAGS="-O3"
emmake make install DESTDIR=$PWD/../BUILD
cd ..

cd SDL2_image
#auto.sh
emconfigure ./configure
emmake make install DESTDIR=$PWD/../BUILD
cd ..

cd SDL2_mixer
#auto.sh
emconfigure ./configure
emmake make install DESTDIR=$PWD/../BUILD
cd ..

cd SDL2_net
#auto.sh
emconfigure ./configure
emmake make install DESTDIR=$PWD/../BUILD
cd ..

cd SDL2_ttf
#auto.sh
emconfigure ./configure
emmake make install DESTDIR=$PWD/../BUILD
#remove -lusr/lib from this line to finish the build
glfont_LDADD = libSDL2_ttf.la  -L/usr/lib -lGL -lm
cd ..

#cd BUILD
#cp ./* /emscripten/system/local/

