# IrrAgg
A collection of utilities for using Anti-Grain Geometry (AGG) with Irrlicht.
The primary utility is a Painter class into which instances of objects inheriting IShape can be given for drawing.

Complete Example
[code]
irr::IImage*
paintTriangle( irr::IVideoDriver* driver, vector2df a, vector2df b, vector2df c)
{
	irr::vecg::Triangle  triangle( toPoint_t(a), toPoint_t(b), toPoint_t(c) );
	irr::vecg::Painter  painter(driver, irr::core::dimension2du(90,30);
	painter.setColor( irr::video::SColor(0xffff0000) ); // Red
	painter.drawNormal(triangle);
	irr::video::IImage*  image = painter.getCanvas();
	image->grab();
	return image;
}
[/code]

More information about using AGG with Irrlicht can be found in [this blog article](https://magicsnippet.wordpress.com/2017/12/03/rendering-anti-grain-vector-graphics-in-irrlicht/).

## Current Features

- Rendering arbitrary vector graphic paths onto a 32-bit ARGB IImage.
- Premade classes for rendering paths.
- Functions for conversion of Irrlicht matrix to/from AGG matrix.
- Functions for conversion between Irrlicht and AGG color types.

## Planned Features

- Font loading and rendering, especially TTF and UTF-8-complete fonts.
- Other shapes (rounded rectangles)

## Build

This is currently a very small group of files and is intended to be dropped in with the rest of your project code.

Tested using AGG-2.5 and Irrlicht-1.9 (development, svn revision 5589).
Note: Due to the consistency of Irrlicht (and thanks to its API-sensitive team), these utilities should work with most versions of Irrlicht.
