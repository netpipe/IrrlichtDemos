<a href="http://irrlicht.sourceforge.net/">
    <img src="http://irrlicht.sourceforge.net/images/irrlicht_logo.png" alt="Irrlicht logo" title="Irrlicht" align="right" height="60" />
</a>
<a href="http://bulletphysics.org/wordpress/">
    <img src="https://upload.wikimedia.org/wikipedia/commons/5/50/Bullet_Physics_Library_Logo.png" alt="Bullet logo" title="Bullet" align="right" height="60" />
</a>

# Castle Battle

![Docs](https://img.shields.io/badge/docs-99%25-blue.svg) ![Open](https://camo.githubusercontent.com/2091d99fb3b1ea0dcacb2ce564d5a3fc099c9ee7/68747470733a2f2f6261646765732e66726170736f66742e636f6d2f6f732f76322f6f70656e2d736f757263652e7376673f763d313032)
![License](https://img.shields.io/github/license/mashape/apistatus.svg)
![dependencies Status](https://david-dm.org/boennemann/badges/status.svg)
![version](https://img.shields.io/badge/version-1.0-orange.svg)
## Some facts
This game is made in C++ with Irrlicht engine for 3D graphic rendering and Bullet Engine for physics simulation. It is made for learning purpose and for an exam at university. Also i didn't developed this type of game before: since i'm a web developer i only developed few 2D HTML5 games at college. That's why it was a little tricky to develop this game, both for time and difficulty reasons. Fortunately Irrlicht documentation is well written.
## Prerequisites
* [Irrlicht library](http://irrlicht.sourceforge.net)
* [Bullet library](http://bulletphysics.org/wordpress)
* C++11
* This source
## Compiling
Castle battle is cross-platform but for now it only compiles on Linux
### Compile release mode
From the root directory of the project run
```
make
```
### Compile debug mode
Don't forget to look Makefile for other commands and for more specifications
#### Debug level 1
```
make debug
```
#### Debug level 2
```
make debug2
```
#### Debug level n
To setup any level use the VERBOSITY variable.
For example for level 5 you can run
```
make VERBOSITY=5 debug
```
## Documentation
If you want, you can download documentation from this repository.
Otherwise you can re-generate on your own but remember that you will need [doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) installed on your system:
```
make doc
```
### Show documentation
To show documentation run
```
make doc-show
```
### Documentation online
Visit the online version of the documentation at 
https://iwasingh.github.io/CastleBattle
### License
```
The MIT License

Copyright (c) 2018 Singh Amarjot

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
```
