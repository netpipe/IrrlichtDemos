Hello Irrlicht-community. I searched for a good terrain-engine on http://vterrain.org, 
i found Stefan Roettgers terrainengine libmini. vterrain.org says: 
"libMini by Stefan Röttger - implements his paper Real-Time Generation of Continuous Levels of Detail for Height Fields 
LGPL source, highly optimized, 5 bytes/heixel, very fast" 
I translate it with: " Quick and Cool !". 

The terrain rendering algorithm (abbreviated GOLD) has also been included into the game AquaNox. 

Normally it draws with OPENGL, but it also has a stubengine inside, where the user 
has to write some interface-functions and the user can draw with other things
I wrote the interface for Irrlicht. The first little example with the interface is below. 
If you want to use it, first you have to download Stefan Roettgers engine: 
http://www9.informatik.uni-erlangen.de/Persons/Roettger/download/ 
Then you have to compile the stubengine - in UNIX/LINUX use the build.sh like written in the 
README, with Microsoft c++/Relo you have to put in the parameter -DNOOGL 
Then you can try to compile this first example. 


// use cursor-keys and mouse to fly
//
// The mini library is the core of the terrain renderer described in the
// paper "Real-Time Generation of Continuous Levels of Detail for Height Fields".
// Copyright (c) 1995-2004 by Stefan Roettger (Version 5.02 as of 18.August.2004).
//
// The terrain renderer is licensed under the terms of the LGPL (see
// http://www.gnu.org/copyleft/ for more information on the license).
// Any commercial use of the code or parts of it requires the explicit
// permission of the author!
//
// The libmini-author's contact address is:
//
// mailto:roettger@cs.fau.de
// http://www9.cs.fau.de/Persons/Roettger
//
// The original terrain rendering paper and the talk are available here:
//
// http://www9.cs.fau.de/Persons/Roettger/papers/TERRAIN.PDF
// http://www9.cs.fau.de/Persons/Roettger/papers/WSCG98.PPT
//
//
// example and Irrlicht-interface-functions by zenprogramming
// (with help from Stefan Roettger ;-) )
// mailto: zenprogramming at yahoo dot de
// http://zenprogramming.tripod.com