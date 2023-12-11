#!/bin/bash
#testing still



#itarget=emscripten
#itarget=Linux64
#itarget=Linux32
#itarget=windows
#debug=true

if ( $debug )
then
	debugf=debug
	debugf=""
fi

codeblocks --build ./prefix="$prefix" -v --target=$itarget$debugf "./Games/Pong2/example.cbp" 


