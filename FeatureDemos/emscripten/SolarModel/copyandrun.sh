#!/bin/bash
directory=/home/netpipe/public_html/emtests/irrlicht/SolarModel

name=SolarSystem
cp $name.html $name.js $name.data $name.wasm $directory


cd $directory
sed -i '/___buildEnvironment(__get_environ());/d' "./test.js"
#./fix


