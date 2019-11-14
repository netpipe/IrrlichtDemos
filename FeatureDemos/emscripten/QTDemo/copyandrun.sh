#!/bin/bash
#html directory for local server

directory=/home/netpipe/public_html/emtests/irrlicht/calculate
name=test

cp $name.html $name.js $name.data $name.wasm qtlogo.svg qtloader.js $directory

cd $directory


#patch for running irrlicht after
sed -i '/___buildEnvironment(__get_environ());/d' "./$name.js"
#./fix


