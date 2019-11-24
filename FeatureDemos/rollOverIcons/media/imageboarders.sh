#!/bin/bash
convert ./1.jpg -bordercolor black -border 20 -bordercolor white -border 1 2.jpg


convert -border 2x2 old.jpg new.jpg

convert -border 2x2 -bordercolor "#FF0000" old.jpg new.jpg

#command to scale images to proper sizes to use in gallerys and to not display as non power of 2 or black
mkdir converted
#find *.jpg | while read i; do name=$(echo $i| cut -d "." -f 1); convert "$i" -geometry 256x256 -extent 256x256 converted/$name.png ; done  
#find *.png | while read i; do name=$(echo $i| cut -d "." -f 1); convert "$i" -geometry 256x256 -extent 256x256 converted/$name.png ; done  

#mogrify -format jpg *.png

#convert -delay 10 -loop 0 *.png anim.gif convert -fuzz 1% -delay 1x8 *.png -coalesce -layers OptimizeTransparency animation.gif
