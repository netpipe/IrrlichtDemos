For class list and documentation, refer to doc/doxygen/html/index.html

For the FAQ list, refer to doc/FAQ/FAQ.pdf

----------------------------------------------
1. Introduction:
----------------------------------------------
Thank you for using irrBullet!

irrBullet is more than just a wrapper, it's an extension library for Bullet physics.
It includes many high-level features not found in other Bullet wrappers or even Bullet itself.

It's engineered in an efficient way that's easy to extend.

It includes interfaces to physics objects, and pointers inside those
interfaces so that you can use the Bullet library to its full extent
if you need access to a function that is not yet included in the interfaces.

In the "libs" folder you will find pre-built Win32 MinGW-GCC and Microsoft Visual C++ 2008-2010 irrBullet
and Bullet libraries ready for use.

If you spot any bugs or you would like to leave feadback,
let me know at fighterstw@hotmail.com


Special Thanks:
- "RandomMesh" for IDebugDraw code
- "Go" for some Bullet softbody code used in ISoftBody::createShape()
- "Nomad" for code used in ITriangleMeshShape::getTriangleMesh()
- "Lo" and "Polylux" for helping with better Linux support
- All irrBullet users for reporting bugs and giving feedback


----------------------------------------------
2. Building irrBullet from Source:
----------------------------------------------
-----------------------
Linux:
-----------------------
There are two Linux build targets in the irrBullet Code::Blocks project in the "source/" folder:
Linux Debug and Linux Release.

Also included is a Makefile located in the "build/" folder.


-----------------------
Windows MinGW:
-----------------------
There are two Windows build targets in the irrBullet Code::Blocks project in the "source/" folder:
Win32 GCC Release and Win32 GCC Debug.


-----------------------
Windows MSVC 2008/2010:
-----------------------
There are two Microsoft Visual C++ projects in the "source/msvc/" folder.

The MSVC 2008 project is in "source/msvc/2008/" and the MSVC 2010 project is
in "source/msvc/2010/"


-----------------------
General:
-----------------------
You can choose to use the pre-built Bullet libraries included with irrBullet, or use your own. 

This only requires some modification of the project linker settings.

Alternatively, you can replace the provided Bullet libraries in the irrBullet folder with your own.



----------------------------------------------
3. Starting a project:
----------------------------------------------
To begin using basic irrBullet features in your project, 
you must follow these steps:


3.1. Add these search directories to your project:
	%irrBullet%/source
	%irrBullet%/source/bheaders
	%irrBullet%/source/bheaders/Bullet

3.2. add "#include <irrBullet.h>" to the top of your project
with the other includes. 

3.3. add these files to your linker (available in the libs/ folder):
	libirrBullet.a, libbulletdynamics.a, libbulletsoftbody.a
	libGIMPACTUtils.a (if you use GImpact),
	liblinearmath.a, libbulletcollision.a

	Make sure the linker files are in that order for irrBullet
	or your project will not compile.


----------------------------------------------
4. Extra:
----------------------------------------------
The header files to the Bullet Physics library are included under
source/bheaders/Bullet. A Code::Blocks project file to compile irrBullet is under
the source/ folder, an MSVC 2010 project is included in source/msvc/2010,
and an MSVC 2008 project is included in source/msvc/2008

The media in the bin/ folder is not all mine; some of it is from
the Irrlicht examples folder and other places. 

The Irrlicht 1.7.1 dll is included for the examples to run.


Copyright (C) 2009-2012 Josiah Hartzell (Skyreign Software)
