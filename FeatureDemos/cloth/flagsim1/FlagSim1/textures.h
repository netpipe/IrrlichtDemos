#ifndef INCLUDE_TEXTURES_H
#define INCLUDE_TEXTURES_H

#include <GL\glaux.h>

typedef struct
{
	GLubyte	*Data;
	GLuint	BitsPerPixel;	
	GLuint	Width;
	GLuint	Height;	
} STGAData;

class COGLTexture
{
public:
	
	_AUX_RGBImageRec *Image;
	
	unsigned int GetID();
	void LoadFromFile(char *filename);
	bool LoadFromTGA(const char *filename, const char* AlphaMap, bool bMipMaps);
	void SetActive();
	GLuint GetWidth();
	GLuint GetHeight();
	GLuint type;
	char * m_TexFilename;
	char * m_AlphaFilename;
private:
	GLuint Width, Height;
	unsigned int ID;
	GLuint BitsPerPixel;

};

#endif