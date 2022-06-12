#!/bin/bash
name=OpenSteer
directory=/home/netpipe/public_html/emtests/irrlicht/opensteer

cp $name.html $name.js $name.data $name.wasm $directory

cd $directory
sed -i '/___buildEnvironment(__get_environ());/d' "./$name.js"
#./fix


