-- irrBullet 0.1.1 --
NOTE: Extract bin.zip to see the compiled examples!

1. Introduction:

Thank you for using irrBullet!
This is currently a beta version, and only includes sphere, box,
bvh triangle mesh, and GImpact triangle mesh collision shapes.

I will add more collision shapes and features 
in future updates.

It does include most of the Bullet features, plus some features
that make it much easier to implement into your game or simulation.

It includes interfaces to physics objects, and pointers inside those
interfaces so that you can use the Bullet library to its full extent
if you need access to a function that is not yet included in the interfaces.

If you spot any bugs or would like to leave feadback,
let me know at josiah@skyreignsoftware.net16.net.

This version only comes with one simple example, with source code included.
You can find the compiled version in the bin/ directory.

There are both debug and release builds of irrBullet in libs/debug and
libs/release, as well as the source code to the wrapper.


2. Starting a project:

To begin using basic irrBullet features in your project, 
you must follow these steps:

1. add "#include <irrBullet.h>" to the top of your project
with the other includes. Make sure you have added a search directory
to the irrBullet include folder!

2. add these files to your linker (available in the libs/ folder):
	libirrBullet.a, libbulletdynamics.a, 
	libGIMPACTUtils.a (if you use GImpact),
	libbulletmath.a, libbulletcollision.a

	Make sure the linker files are in that order for irrBullet,
	or your project will not compile.

3. Extra:
The source code to the Bullet Physics library is included, under
source/Bullet. A project file to compile irrBullet is also under
the source/ folder.

The media in the bin/ folder is not all mine, some of it is from
the Irrlicht examples folder. The Irrlicht 1.6 dll is included
for the examples to run.


Copyright (C) 2009-2010 Josiah H. (Skyreign Software)
