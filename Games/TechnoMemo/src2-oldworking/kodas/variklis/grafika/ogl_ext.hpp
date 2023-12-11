/***********************************************************
	OpenGL extensions

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	OpenGL, WGL prapletimai.
	Gaunami funkciju adresai.

	Pradeta:
	    2008-11-26
	Paskutini karta keista:
	    2009-01-28
************************************************************/


#ifndef OGL_EXT_HPP
#define OGL_EXT_HPP
#include <defines.hpp>
#include <windows.h>
#include <GL/gl.h>


class OglExt
{
	private:
	//Privatus kintamieji OpenGL konteksto sukurimui
	static HDC hDC;
	static HGLRC hRC;
	static HWND hWnd;
	static HINSTANCE hInstance;
	static int PixelFormat;
	
	public:
	static const char *exts;	//Prapletimu sarasas
	//Funkcijos
	static GLubyte * (APIENTRY * wglGetExtensionsStringARB)(HDC);
	static BOOL (APIENTRY * wglChoosePixelFormatARB)(HDC, int const *, FLOAT const *, UINT, int *, UINT *);
	static GLvoid (APIENTRY * glSampleCoverage)(GLclampf, GLboolean);
	static GLvoid (APIENTRY * glActiveTexture)(GLenum); 
	static GLvoid (APIENTRY * glMultiTexCoord2f)(GLenum, GLfloat, GLfloat);
	//Palaikomi prapletimai
	static bool WGLMultisample;
	static bool Multitexture;
	static bool Cubemap;
	static bool Anisotropic;
	
	static void Sukurti(HDC = NULL) throw(const char*);	//Inicializuojamos funkcijos (gaunami adresai)
	//static bool InitGL() throw(const char*);	//Inicializuojamas OpenGL kontekstas
	//static void CloseGL();						//Naikinamas OpenGL kontekstas
	//static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};


#endif
