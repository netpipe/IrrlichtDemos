#!/bin/bash
#shellscript for launching custom screensavers.

#x = $(xdotool getmouselocation | cut -d ":" -f 2 |cut -d " " -f 1)
#y = $(xdotool getmouselocation | cut -d ":" -f 3 |cut -d " " -f 1)

exepath=Pong
exedir=/home/netpipe/Dev/IrrlichtDemos/screenSavers/Gibson/
#exedir=./
minutes=1
seconds=$minutes*60*10
time=$seconds*100

while :; do
    if [[ $(xprintidle) -gt $time ]]; then
        #echo "launching"
        cd $exedir
        $exedir$exepath
    fi
    echo "$(xprintidle) of  $(( $time))"
    sleep 10
done

