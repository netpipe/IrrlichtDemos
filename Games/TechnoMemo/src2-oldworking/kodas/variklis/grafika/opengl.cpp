#include "opengl.hpp"


void OpenGL::Init(KNustatymai &nust)
{
	glViewport(0, 0, nust.dydisX, nust.dydisY);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}


void OpenGL::Ortho(KNustatymai &nust)
{
	Ortho(0, 0, nust.dydisX, nust.dydisY, 100.0, -100.0);
}


void OpenGL::Ortho(int x, int y, int w, int h, float nearClip, float farClip)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x, w, y, h, nearClip, farClip);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void OpenGL::Perspektyva(KKamera &kam)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(kam.fov, kam.ratio, kam.arti, kam.toli);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	kam.k.x, kam.k.y, kam.k.z,		//Kamera
				kam.t.x, kam.t.y, kam.t.z,		//Taikinys
				kam.up.x, kam.up.y, kam.up.z);	//Up
}
