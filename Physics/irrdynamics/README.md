irrdynamics
===========

A light-weight Bullet Physics wrapper for the irrlicht graphics engine.
Currently irrdynamics is very basic, it features basic shape handling, allows to define constraints between them and does the physics sim stepping for you. Oh, and most notably: It handles terrain correctly.

Layout
------
irrdynamics is laid out as a singleton that can be fed with irrlicht's ISceneNodes. It does all the physics handling abstracted from the application programmer who doesn't need to know a thing about bullet's data types. Hence irrdynamics targets newbies and those coders, who need fast and simple physics support for their game.

Installation
------------
There is a CodeBlocks project in the "prj" folder plus a premake4 script and a Makefile in the base dir. They all support Linux for now, should be fairly straightforward to create a MSVC project for it. If you do so, please leave a note at https://github.com/ottona/irrdynamics

License
-------
irrdynamics has recently been released under the zlib license as according to irrlicht's and bullet's one. So you can basically do anything (well, most things) with it. Oh, and if you can: CONTRIBUTE! :)

Have fun,
Otto "polylux" Naderer
