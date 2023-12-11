
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_TextureLoader.h"

#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.

//using namespace gxTextureLoader;

bool gxTextureLoader::bErrorOccured = false;

gxTextureLoader::BitmapFile::BitmapFile()
{
	sizeX = 0;
	sizeY = 0;
	data = 0;
}

gxTextureLoader::BitmapFile::~BitmapFile()
{
	free(data);
}

gxTextureLoader::BitmapFile * gxTextureLoader::LoadBitmapFile(const char *filename)
{
	gxTextureLoader::BitmapFile * pImage = new gxTextureLoader::BitmapFile();

    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
		printf("File Not Found : %s\n",filename);
		bErrorOccured = true;
		return 0;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&pImage->sizeX, 4, 1, file)) != 1)
	{
		printf("Error reading width from %s.\n", filename);
		bErrorOccured = true;
		return 0;
    }

    printf("Width of %s: %lu\n", filename, pImage->sizeX);
    
    // read the height 
    if ((i = fread(&pImage->sizeY, 4, 1, file)) != 1)
	{
		printf("Error reading height from %s.\n", filename);
		bErrorOccured = true;
		return 0;
    }
    printf("Height of %s: %lu\n", filename, pImage->sizeY);
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = pImage->sizeX * pImage->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1)
	{
		printf("Error reading planes from %s.\n", filename);
		bErrorOccured = true;
		return 0;
    }

    if (planes != 1)
	{
		printf("Planes from %s is not 1: %u\n", filename, planes);
		bErrorOccured = true;
		return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1)
	{
		printf("Error reading bpp from %s.\n", filename);
		bErrorOccured = true;
		return 0;
    }

    if (bpp != 24)
	{
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		bErrorOccured = true;
		return 0;
    }
	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data. 
    pImage->data = (char *) malloc(size);
    if (pImage->data == NULL)
	{
		printf("Error allocating memory for color-corrected image data");
		bErrorOccured = true;
		return 0;
    }

    if ((i = fread(pImage->data, size, 1, file)) != 1)
	{
		printf("Error reading image data from %s.\n", filename);
		bErrorOccured = true;
		return 0;
    }

    for (i=0;i<size;i+=3) // reverse all of the colors. (bgr -> rgb)
	{
		temp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = temp;
    }
    
    // we're done.
    return pImage;
}

GLuint gxTextureLoader::MakeTextureFromBitmapFile(gxTextureLoader::BitmapFile * pImage)
{
	GLuint nTextureID = 20;
    // Create Texture	
    glGenTextures(1, &nTextureID);
    glBindTexture(GL_TEXTURE_2D, nTextureID);   // 2d texture (x and y size)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);

	return nTextureID;
}

GLuint gxTextureLoader::MakeBlankTexture(int nSize, bool bPixellated)
{
	//our OpenGL texture handle
	GLuint nTextureID = 0;

	int xSize = nSize; //size of texture
	int ySize = nSize; //size of texture

	//new array
	//char* colorBits = new char[ xSize * ySize * 3 ];
	char* colorBits = new char[ xSize * ySize * 4 ];

	for(int i = 0; i < xSize * ySize * 4; i++)
	{
		colorBits[i] = 0;
	}

	//texture creation..
	glGenTextures(1,&nTextureID);
	glBindTexture(GL_TEXTURE_2D,nTextureID);

	//you can set other texture parameters if you want
	if(bPixellated)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, xSize, ySize, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBits);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, xSize, ySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBits);

	//clean up
	delete[] colorBits;

	return nTextureID;
}

// How is the texture memory freed?

GLuint gxTextureLoader::MakeTexture(const char *filename)
{
	bErrorOccured = false;

	gxTextureLoader::BitmapFile * pImage = gxTextureLoader::LoadBitmapFile(filename);

	if(pImage == 0)
	{
		printf("Error loading image from file: %s\n",filename);
		bErrorOccured = true;
		return 0;
	}

	GLuint nTextureID = gxTextureLoader::MakeTextureFromBitmapFile(pImage);

	// Can the image be discarded now? It seems to be happy...
	// (moved to here since MakeTextureFromBitmapFile shouldn't be expected to also delete, it should have only its one stated purpose)
	delete pImage;

	return nTextureID;
}
