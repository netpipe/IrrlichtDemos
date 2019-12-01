# IrrAgg
A collection of utilities for using Anti-Grain Geometry (AGG) with Irrlicht.

This is currently a very small group of files and is intended to be dropped in with the rest of your project code.

More information about using AGG with Irrlicht can be found in [this blog article](https://magicsnippet.wordpress.com/2017/12/03/rendering-anti-grain-vector-graphics-in-irrlicht/).

## Current Features

- Rendering arbitrary vector graphic paths onto a 32-bit ARGB IImage.
- Vector path (static and dynamic) array storage structures that can be immediately rendered.
- Convenience functions for rendering triangles, ellipses, and rounded rectangles.
- Engine for rendering various shapes and paths in a safer way than directly using AGG.
- Functions for conversion of Irrlicht matrix to/from AGG matrix.
- Functions for conversion between Irrlicht and AGG color types.

## Planned Features

- Font loading and rendering, especially TTF and UTF-8-complete fonts.
- List vertex source (for inserting of points).

## Build

Tested using AGG-2.5 and Irrlicht-1.9 (development, svn revision 5589).
Note: Due to the consistency of Irrlicht (and thanks to its API-sensitive team), these utilities should work with most versions of Irrlicht.
