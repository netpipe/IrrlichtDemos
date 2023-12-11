/***********************************************************
	Geometriniai objektai

	© 2008 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Piesiami geometriniai objektai, tokie kaip plokstuma,
	apskritimas, ziedas. Yra wire/solid versijos.
	Generuojami display list'ai.

	Pradeta:
	    2007-12-02
	Paskutini karta redaguota:
	    2009-01-26
************************************************************/


#ifndef GEOM_OBJ_HPP
#define GEOM_OBJ_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <GL/gl.h>
#include <math.h>


class Geom
{
	public:
	
	enum {
		PLOKST,
		PLOKST_WIRE,
		APSKRITIMAS,
		SKRITULYS,
		ZIEDAS_2D,
		KOORD,
		KUBAS,
		KUBAS_SP,
		KUBAS_WIRE,
		ZIEDAS,
		ZIEDAS_WIRE,
		DYDIS
	};
	static GLuint dl[DYDIS];
	
	Geom();
	~Geom();
	static void Sukurti();		//Sukuriami display listai
	static void Naikinti();		//Naikinami duomenys
	
	//2D
	static void Plokst(GLfloat plotis);
	static void Plokst(GLfloat plotis, GLfloat aukstis);
	static GLuint PlokstDL();
	static void PlokstWire(GLfloat plotis);
	static void PlokstWire(GLfloat plotis, GLfloat aukstis);
	static GLuint PlokstWireDL();
	static void Apskritimas(GLfloat dydis = 1.0);
	static GLuint ApskritimasDL(GLfloat dydis = 1.0, int kokybe = 32);
	static void Skritulys(GLfloat dydis = 1.0);
	static GLuint SkritulysDL(GLfloat dydis = 1.0, int kokybe = 32);
	static void Ziedas2D(GLfloat dydis = 1.0);
	static GLuint Ziedas2DDL(GLfloat dydis1 = 0.5, GLfloat dydis2 = 1.0, int kokybe = 32);
	
	//3D
	static void Koord();
	static GLuint KoordDL(GLfloat dydis = 10.0, int tarpu = 10);
	static void Kubas(GLfloat x);
	static void Kubas(GLfloat x, GLfloat y, GLfloat z);
	static GLuint KubasDL();
	static void KubasSp(GLfloat x);
	static void KubasSp(GLfloat x, GLfloat y, GLfloat z);
	static GLuint KubasSpDL();
	static void KubasWire(GLfloat x);
	static void KubasWire(GLfloat x, GLfloat y, GLfloat z);
	static GLuint KubasWireDL();
	static void Ziedas();
	static GLuint ZiedasDL(int kok = 24, int kok2 = 8, float dyd = 2.0, float dyd2 = 0.5);
	static void ZiedasWire();
	static GLuint ZiedasWireDL(int kok = 24, int kok2 = 8, float dyd = 2.0, float dyd2 = 0.5);
};


#endif
