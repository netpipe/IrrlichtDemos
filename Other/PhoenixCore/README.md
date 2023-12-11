PhoenixCore
============

PhoenixCore is an experimental 2D rendering framework based on OpenGL and written in C++. It is a fairly abstract library for rendering 2D geometry. PhoenixCore's features include:

 * Setting up an OpenGL Context.
 * Processing user Input (Keyboard, Mouse).
 * Managing OpenGL Context (swapping buffers, etc).
 * Provides abstract math classes for Vectors and Matrices.
 * Provides interface to loading and manipulating Textures.
 * Provides methods to modify the View Port and Transformation Matrix.
 * Provides classes for representing geometric primitives.
 * Provides methods for drawing geometric primitives.
 * Provides abstract class for Color representation and manipulation.
 * Provides access to high-precision timers.
 * Provides high-level threaded resource management.
 * Implements an experimental automatic optimizing batch renderer.


Goals
=====

Phoenix's main goal is to provide and abstract interface for drawing 2d geometry with OpenGL. Phoenix is very basic, and does not provide features such as physics, sound, collision, or even a high level sprite class. However, phoenix is designed to integrate with other systems very easily


What Phoenix Is Not
===================

Phoenix is not a complete library for game development. It does not provide many of the needed libraries for game development. However, Phoenix is a good base and integrates very well with other libraries such as OpenAL, Box2D, etc.


Building
========

Requirements
------------

 * Cmake 2.8 or newer
 * Boost C++ Libraries (1.37 or newer)
 * Boost Threads
 * Boost Datetime
 * Boost Signals 2
 * GLFW (2.7 or newer)

Boost and GLFW are usually available via the package manager on many Linux distributions. 

On Windows you can download GLFW from [their website](http://glfw.sourceforge.net/) 
and I highly suggest you use the Boost Installer from Boost Pro Consulting.


Linux/Unixes
------------

 * mkdir build && cd build
 * cmake ..
 * make


Windows
-------

  * mkdir build
  * cd build
  * Invoke CMake with the generator and paths to the libraries, should be similar to:

    cmake -G"Visual Studio 10" -DGLFW_PATH="J:\Development\glfw-lite" -DPHOENIX_PATH="J:\Development\phoenixgl" -DBOX2D_PATH="J:\Development\Box2D" -DBOOST_ROOT="D:\boost_1_47" ..

  * A Visual Studio solution should be generated that will allow you to build the core library, tests, and examples.


Special Thanks
==============
There are a couple of brave souls that made PhoenixCore possible, among these are:

 * Denzel Morris
 * [Sven Bergström](https://github.com/FuzzYspo0N)
 * Brian Labbe
 * Chris Parich


License
=======

Copyright (c) 2009-2011 Jon Wayne Parrott

This is the MIT License.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.