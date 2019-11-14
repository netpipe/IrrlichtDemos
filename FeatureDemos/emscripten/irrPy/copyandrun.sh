#!/bin/bash
name=irrPy
directory=/home/netpipe/public_html/emtests/irrlicht/pythontest

cp $name.html $name.js $name.data $name.wasm $directory

cd $directory
sed -i '/___buildEnvironment(__get_environ());/d' "./$name.js"
#./fix


