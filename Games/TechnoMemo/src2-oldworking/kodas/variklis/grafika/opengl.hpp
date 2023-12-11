/***********************************************************
	OpenGL funkcijos

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	OpenGL funkcijos: pradiniu reiksmiu inicializavimas,
	projekciju nustatymas.

	Pradeta:
	    2009-01-22
	Paskutini karta redaguota:
	    2009-01-31
************************************************************/


#ifndef OPENGL_HPP
#define OPENGL_HPP
#include <defines.hpp>
#include <variklis/matke/kamera.hpp>
#include <GL/gl.h>
#include <GL/glu.h>


class OpenGL
{
	public:
	static void Init(KNustatymai&);
	static void Ortho(KNustatymai&);
	static void Ortho(int, int, int, int, float, float);
	static void Perspektyva(KKamera&);
};


#endif
