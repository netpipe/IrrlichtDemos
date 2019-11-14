#!/bin/bash
directory=/home/netpipe/public_html/emtests/irrlicht/videoPlayer

cp test.html test.js test.data test.wasm $directory

cd $directory
sed -i '/___buildEnvironment(__get_environ());/d' "./test.js"
#./fix


