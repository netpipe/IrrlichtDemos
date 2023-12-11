
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_TEXTURELOADER_H
#define GX_TEXTURELOADER_H

#include <GL/glut.h>

class gxTextureLoader
{
public:

	class BitmapFile
	{
	public:
		BitmapFile();
		virtual ~BitmapFile();

		unsigned long sizeX;
		unsigned long sizeY;
		char *data;
	};

	static bool bErrorOccured;

	static BitmapFile * LoadBitmapFile(const char *filename);
	static GLuint MakeTextureFromBitmapFile(BitmapFile * pImage);

	static GLuint MakeTexture(const char *filename);

	static GLuint MakeBlankTexture(int nSize, bool bPixellated = false);
};

#endif // GX_TEXTURELOADER_H

