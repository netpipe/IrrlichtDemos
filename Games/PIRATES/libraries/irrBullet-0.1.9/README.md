[![Build Status](https://travis-ci.org/danyalzia/irrBullet.png?branch=master)](https://travis-ci.org/danyalzia/irrBullet)
[![Stories in Ready](https://badge.waffle.io/danyalzia/irrBullet.png?label=ready&title=Ready)](https://waffle.io/danyalzia/irrBullet)

irrBullet - Bullet Physics Wrapper for Irrlicht Engine
======================================================

Details
-------

This is a fork of irrBullet, which can be found <a href="https://irrbullet.svn.sourceforge.net/">here</a>. 

It is an updated version of irrBullet which works with Irrlicht 1.8.1 and latest SVN branch (unstable) and Bullet 2.83. It also has several improvements over outdated irrBullet. I tried contacting the original author of irrBullet, but got no response.

Note: The following version hasn't released yet and maybe unstable. If you find any bug, then please open an issue in issues tracker. Thanks.

What's new in 0.1.9
-------------------
* Performance improvement (refactored loops, replaced Irrlicht's containers with STL, add move semantics etc)
* Several bug fixes
* Support for latest versions of Bullet and Irrlicht
* ICapsuleShape and ICylinderShape added
* IRigidBody, irrBulletWorld etc are feature complete (all methods have been wrapped)
* Strong C++11 support - dropped support for non-C++11 compilers
* irrBullet is forked - now maintained by Danyal Zia

Installation
------------

(Note: Only Linux supported at the moment)

Open `Makefile` and change the value of `IrrlichtHome` and `BulletHome` to the root directory of Irrlicht and Bullet respectively, and simply run "make" to compile the library.

License
-------

It is released under permissive zlib license same as Bullet and Irrlicht.
