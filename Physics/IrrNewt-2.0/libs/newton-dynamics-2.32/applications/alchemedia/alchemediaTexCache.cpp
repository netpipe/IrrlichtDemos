/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaTextureCache.cpp
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////

// alchemediaDoc.cpp : implementation of the alchemediaDoc class
//

#include "alchemedia.h"
#include "alchemediaTexCache.h"


#pragma pack(1)
struct dAlchemediaTargaHeader
{
	GLbyte	identsize;              // Size of ID field that follows header (0)
	GLbyte	colorMapType;           // 0 = None, 1 = palette
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;  // First color map entry
	unsigned short	colorMapLength; // Number of colors
	unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;         // image x origin
	unsigned short	ystart;         // image y origin
	unsigned short	width;          // width in pixels
	unsigned short	height;         // height in pixels
	GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;             // image descriptor
};
#pragma pack(8)


void alchemediaTextureCache::LoadTexture (const char* const fileName, dTextureNodeInfo* const texture)
{
	_ASSERTE (texture->GetInternalId() == -1);

	unsigned key = dCRC(texture->GetPathName());
	dTreeNode* node = Find(key);
	if (!node) {

		FILE* const pFile = fopen (fileName, "rb");
		_ASSERTE (pFile);
		if (!pFile) {		    
			return;
		}

		//_ASSERTE (sizeof (dAlchemediaTargaHeader) == 18);
		// Read in header (binary) sizeof(dAlchemediaTargaHeader) = 18
		dAlchemediaTargaHeader dAlchemediaTargaHeader;		// TGA file header
		fread(&dAlchemediaTargaHeader, 18, 1, pFile);

		// Do byte swap for big vs little Indian
		//dAlchemediaTargaHeader.colorMapStart = SWAP_INT16(dAlchemediaTargaHeader.colorMapStart);
		//dAlchemediaTargaHeader.colorMapLength = SWAP_INT16(dAlchemediaTargaHeader.colorMapLength);
		//dAlchemediaTargaHeader.xstart = SWAP_INT16(dAlchemediaTargaHeader.xstart);
		//dAlchemediaTargaHeader.ystart = SWAP_INT16(dAlchemediaTargaHeader.ystart);
		//dAlchemediaTargaHeader.width = SWAP_INT16(dAlchemediaTargaHeader.width);
		//dAlchemediaTargaHeader.height = SWAP_INT16(dAlchemediaTargaHeader.height);
		        
		// Get width, height, and depth of texture
		short sDepth = dAlchemediaTargaHeader.bits / 8;
		_ASSERTE ((sDepth == 3) || (sDepth == 4));

	    
		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if(dAlchemediaTargaHeader.bits != 8 && dAlchemediaTargaHeader.bits != 24 && dAlchemediaTargaHeader.bits != 32) {
			fclose(pFile);
			return;
		}


		GLint width = dAlchemediaTargaHeader.width;
		GLint height = dAlchemediaTargaHeader.height;

		GLuint texture = 0;
		// Calculate size of image buffer
		unsigned lImageSize = dAlchemediaTargaHeader.width * dAlchemediaTargaHeader.height * sDepth;
	    
		// Allocate memory and check for success
		GLbyte* const pBits = new GLbyte [dAlchemediaTargaHeader.width * dAlchemediaTargaHeader.height * 4];
		if(pBits == NULL) {
			fclose(pFile);
			return;
		}
	    
		// Read in the bits
		// Check for read error. This should catch RLE or other 
		// weird formats that I don't want to recognize
		if(fread(pBits, lImageSize, 1, pFile) != 1)  {
			fclose(pFile);
			delete[] pBits;
			return; 
	   }

		GLenum eFormat = GL_RGBA;
		GLint iComponents = 4;
		switch(sDepth)
		{
			// intel arch
			case 3:     // Most likely case
				eFormat = GL_BGR;
				iComponents = 3;
				break;

			case 4:
				eFormat = GL_BGRA_EXT;
				iComponents = 4;
			break;

			case 1:
				eFormat = GL_LUMINANCE;
				iComponents = 1;
				break;
		};

		node = Insert (key);
		alchemediaTextureResource& textIntry = node->GetInfo();

		glGenTextures(1, &textIntry.m_textID);
		glBindTexture(GL_TEXTURE_2D, textIntry.m_textID);


	   // select modulate to mix texture with color for shading
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		// when texture area is small, bilinear filter the closest mipmap
	//  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );

		// when texture area is small, trilinear filter mipmaped
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// when texture area is large, bilinear filter the first mipmap
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// build our texture mipmaps
		gluBuild2DMipmaps (GL_TEXTURE_2D, iComponents, width, height, eFormat, GL_UNSIGNED_BYTE, pBits);

		// Done with File
		fclose(pFile);
		delete[] pBits;
	} 

	alchemediaTextureResource& textIntry = node->GetInfo();
	texture->SetInternalId (textIntry.m_textID);
	textIntry.AddRef();
}


//void alchemediaTextureCache::UnloadTexture (alchemediaTextureResource* texture)
void alchemediaTextureCache::UnloadTexture (dTextureNodeInfo* const texture)
{
	unsigned key = dCRC(texture->GetPathName());
	dTreeNode* const node = Find(key);
	_ASSERTE (node);
	alchemediaTextureResource& textIntry = node->GetInfo();
	textIntry.Release();
	if (textIntry.GetRef() == 1) {
		Remove(node);
	}
	texture->SetInternalId (-1);
}




