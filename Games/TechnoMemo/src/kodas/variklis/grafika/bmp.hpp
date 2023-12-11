/***********************************************************
	BMP lib'as

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	BMP lib'as, skirtas uzkrauti BMP failams.
	BMP failas turi buti paprastas, t. y. nenaudoti suspaudimo ar color index
	Palaikomi spalvu formatai:
		8 bpp - intensyvumas
		16 bpp - A1B5G5R5
		24 bpp - B8G8R8
		32 bpp - A8B8G8R8
	_TODO: Padaryti issaugojima

	Pradeta:
	    2007-02-17
	Paskutini karta redaguota:
	    2009-01-27
************************************************************/

/*      BMP HEADERIS (54 baitai)
 0-1    unsigned short  id;             //Characters to identify bitmap
 2-5    unsigned int    size;           //Failo dydis baitais
 6-9    unsigned int    reserved;       //Reserved for later use
 10-13  unsigned int    offset;         //Offset from beginning of file to the beginning of the bitmap data
 14-17  unsigned int    header;         //Length of the bitmap info header
 18-21  unsigned int    width;          //Plotis
 22-25  unsigned int    height;         //Aukstis
 26-27  unsigned short  planes;         //Number of planes in this bitmap
 28-29  unsigned short  bpp;            //1 - monochrome  4 - 16 color  8 - 256 color  16 - 16bit high color  24 - 24bit true color  32 - 32bit true color
 30-33  unsigned int    compress;       //Suspaudimas: 0 - nera, 1 - rle 8-bit, 2 - rle 4-bit, 3 - bitfields
 34-37  unsigned int    dataSize;       //Size of the bitmap data in bytes
 38-41  unsigned int    hres;           //Horizontal resolution expressed in pixel per meter
 42-45  unsigned int    vres;           //Vertical resolution expressed in pixels per meter
 46-49  unsigned int    colors;         //Number of colors used by this bitmap
 50-53  unsigned int    impColors;      //Number of important colors
*/


#ifndef BMP_HPP
#define BMP_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/grafika/ogl_constants.hpp>
#include <variklis/grafika/tekstura_duom.hpp>
#include <GL/gl.h>


class KBmp: public KTeksturaDuom{
	public:
	unsigned char   header[54];	//Visas paveiksliuko headeris

	KBmp();
	~KBmp();
	void Uzkrauti(char *vardas) throw(const char*);
	void Issaugoti(char *pav);
	void Issaugoti(unsigned int xx, unsigned int yy,
	  unsigned int bppp, unsigned char *duomm, char *pav);
	void Naikinti();
};


#endif
