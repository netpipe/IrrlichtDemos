/*
* GLGX 
* An OpenGL Utility Library
*
* Copyright (c) 2002-2010, Grégory Smialek
*
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted 
* provided that the following conditions are met:
* 
*   * Redistributions of source code must retain the above copyright notice, 
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation 
*     and/or other materials provided with the distribution.
*   * Neither the name of the Grégory Smialek nor the names of its contributors
*     may be used to endorse or promote products derived from this software 
*     without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Tanks to Andreas T Jonsson for his tutorial on http://codesampler.com


#include "GLGX_DDSImage.h"

#include<stdio.h>
#include <string.h>

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                    0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                   0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                   0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                   0x83F3

DDSImage::DDSImage()
{
	Width  = 0;
	Height = 0;
	Format = 0;

	BPP = 0;
	NumMipMaps = 0;


}

DDSImage::~DDSImage()
{
	
}

unsigned char* DDSImage::LoadFromFile(const char *pFileName,unsigned char *i_Data)
{
	int factor;
	int bufferSize;

	FILE *pFile;
	if(!(pFile = fopen(pFileName, "rb")))
		return NULL;

	char pFileCode[4];
	fread(pFileCode, 1, 4, pFile);
	if(strncmp(pFileCode, "DDS ", 4) != 0)
		return NULL;

	unsigned int fourCC;
	fseek(pFile, CR_FOURCC_OFFSET, SEEK_SET);
	fread(&fourCC, sizeof(unsigned int), 1, pFile);

	unsigned int linearSize;
	fseek(pFile, CR_LINEAR_SIZE_OFFSET, SEEK_SET);
	fread(&linearSize, sizeof(unsigned int), 1, pFile);

	unsigned int mipMapCount;
	fseek(pFile, CR_MIPMAP_COUNT_OFFSET, SEEK_SET);
	fread(&mipMapCount, sizeof(unsigned int), 1, pFile);

	unsigned int width;
	fseek(pFile, CR_DDS_WIDTH_OFFSET, SEEK_SET);
	fread(&width, sizeof(unsigned int), 1, pFile);

	unsigned int height;
	fseek(pFile, CR_DDS_HEIGHT_OFFSET, SEEK_SET);
	fread(&height, sizeof(unsigned int), 1, pFile);

	switch(fourCC)
	{
		case FOURCC_DXT1:
			this->Format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			factor = 2;
			break;

		case FOURCC_DXT3:
			this->Format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			factor = 4;
			break;

		case FOURCC_DXT5:
			this->Format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			factor = 4;
			break;

		default:
			return NULL;
	}
	
	bufferSize = (mipMapCount > 1) ? (linearSize * factor) : linearSize;
	i_Data = new unsigned char[bufferSize];
	
	this->Width      = width;
	this->Height     = height;
	this->NumMipMaps = mipMapCount;
	this->BPP = (fourCC == FOURCC_DXT1) ? 3 : 4;

	fseek(pFile, CR_DDS_PIXEL_DATA_OFFSET, SEEK_SET);
	fread(i_Data, 1, bufferSize, pFile);
	fclose(pFile);

	return i_Data;
}
