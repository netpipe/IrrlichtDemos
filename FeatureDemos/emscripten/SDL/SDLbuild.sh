#!/bin/bash

export WORKSPACE=$PWD
source /home/Dev/libs/Scripting/emsdk/emsdk_env.sh
#./emsdk construct_env
#. ./emsdk_set_env.sh
#####################################################################################################################
download=0

SDL2=0
libmad=0
ogg=0
libmodplug=0
vorbis=1
buildmixer=0

# Verify

#emcc -v
#####################################################################################################################

# some general flags
export CFLAGS="-g0 -O2"
export CXXFLAGS="$CFLAGS"
export EM_CFLAGS="-Wno-warn-absolute-paths"
export EMMAKEN_CFLAGS="$EM_CFLAGS"
export PKG_CONFIG_PATH="$WORKSPACE/lib/pkgconfig"
export MAKEFLAGS="-j2"

# flags to fake emconfigure and emmake
export CC="emcc"
export CXX="em++"
export LD="$CC"
export LDSHARED="$LD"
export RANLIB="emranlib"
export AR="emar"


# libogg
#####################################################################################################################
	
if (( $ogg )); then
cd $WORKSPACE
cd libogg-1.3.2
	if (( $download )); then

	rm -rf libogg-1.3.2 [ -f libogg-1.3.2.tar.xz ] || wget -nv http://downloads.xiph.org/releases/ogg/libogg-1.3.2.tar.xz
	tar xf libogg-1.3.2.tar.xz
fi
cd libogg-1.3.2
#sed 's/-O20/-g0 -O2/g' -i configure
EMMAKEN_JUST_CONFIGURE=1 ./configure --prefix=$WORKSPACE --disable-shared --enable-static 

make install
cd ..
fi

# libvorbis
#####################################################################################################################

if (( $vorbis )); then
cd $WORKSPACE
cd libvorbis-1.3.5
	if (( $download )); then

	rm -rf libvorbis-1.3.5/
	[ -f libvorbis-1.3.5.tar.xz ] || wget -nv http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.xz
	tar xf libvorbis-1.3.5.tar.xz
fi
cd libvorbis-1.3.5
EMMAKEN_JUST_CONFIGURE=1 ./configure --prefix=$WORKSPACE --disable-shared --enable-static
make install
cd ..
fi

# libmodplug
#####################################################################################################################

if (( $libmodplug )); then
cd $WORKSPACE

	if (( $download )); then
	rm -rf libmodplug-0.8.8.5/
	[ -f libmodplug-0.8.8.5.tar.gz ] || wget -nv http://downloads.sourceforge.net/project/modplug-xmms/libmodplug/0.8.8.5/libmodplug-0.8.8.5.tar.gz
	tar xf libmodplug-0.8.8.5.tar.gz
fi
cd libmodplug-0.8.8.5
cd libmodplug-0.8.8.5
EMMAKEN_JUST_CONFIGURE=1 ./configure --prefix=$WORKSPACE --disable-shared --enable-static 
make install
cd ..
fi

# libmad (with some patches from debian+arch)
#####################################################################################################################

if (( $libmad )); then
cd $WORKSPACE

	if (( $download )); then
		[ -d libmad-0.15.1b ] || PATCH_MAD=1
		wget -nv -N http://download.sourceforge.net/mad/libmad-0.15.1b.tar.gz
		wget -nv -N http://http.debian.net/debian/pool/main/libm/libmad/libmad_0.15.1b-8.diff.gz
		wget -nv -N https://projects.archlinux.org/svntogit/packages.git/plain/trunk/libmad.patch?h=packages/libmad -O libmad-pkgconfig.patch
		tar xf libmad-0.15.1b.tar.gz
		cd libmad-0.15.1b
		if [ ! -z ${PATCH_MAD+x} ]; then
		  zcat ../libmad_0.15.1b-8.diff.gz | patch -p1
		  patch -p1 -i ../libmad-pkgconfig.patch
		  patch -p1 -i debian/patches/frame_length.diff
		  patch -p1 -i debian/patches/amd64-64bit.diff
		  patch -p1 -i debian/patches/optimize.diff
		fi
	fi
cd libmad-0.15.1b
EMMAKEN_JUST_CONFIGURE=1 emconfigure ./configure --prefix=$WORKSPACE --disable-shared --enable-static 
make install
cd ..
fi

# SDL2
#####################################################################################################################

if (( $SDL2 )); then

if (( $download )); then
	[ -d SDL2/.git ] && PULL_SDL2=1 || git clone https://github.com/emscripten-ports/SDL2.git
fi
cd SDL2
[ -z ${PULL_SDL2+x} ] || git pull origin master
git clean -dxf
EMMAKEN_JUST_CONFIGURE=1 ./configure --prefix=$WORKSPACE --host=asmjs-unknown-emscripten --disable-assembly --disable-threads --enable-cpuinfo=false
make install
cd ..
fi

if (( $buildmixer )); then
# SDL2_mixer
#####################################################################################################################
	cd $WORKSPACE
	if (( $download )); then
		[ -d SDL_mixer/.hg ] && PULL_SDLMIXER=1 || hg clone https://hg.libsdl.org/SDL_mixer
	fi
	cd SDL_mixer
	[ -z ${PULL_SDLMIXER+x} ] || hg pull -u
	hg up -C
	hg --config "extensions.purge=" purge --all

	#cat <<-EOF > mixer.patch

	patch < mixer.patch
	EMMAKEN_JUST_CONFIGURE=1 ./configure --prefix=$WORKSPACE CPPFLAGS="-I$WORKSPACE/include -L$WORKSPACE/lib" --disable-shared --disable-sdltest \
	   --enable-music-mp3-mad-gpl --enable-music-ogg --disable-music-ogg-shared --enable-music-mod-modplug --disable-music-mod-modplug-shared \
	   --disable-music-midi-fluidsynth --disable-music-midi-fluidsynth-shared
	make install

fi


