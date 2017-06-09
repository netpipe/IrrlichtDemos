#/bin/sh
# install script for Ubuntu (should be easy to adept for other distros)
# the game is installed in the build directory, start with ./IrrBombix in the build dir, you can copy the hole build directory  whereever you want


sudo apt-get install libirrlicht-dev g++ libopenal-dev libogg-dev libvorbis-dev libglu1-mesa-dev libXxf86vm-dev libcommoncpp2-dev cmake

# create symlink for cAudio
ln -s cAudio2.1.0-Linux_x86 cAudio

# register irrlicht and cAudio
sudo cp *.cmake /usr/share/cmake-2.8/Modules/

# cAudio hasnt support for cmake yet (to come in 2.2)
# make cAudio
cd cAudio/Source
make
sudo make install

cd ../../build
cmake ..
make

# run it
#./IrrBombix

echo
echo
echo "run it with cd build && ./IrrBombix" 
echo



