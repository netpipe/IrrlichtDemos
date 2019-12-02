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

#include"GLGXTexture.h"
#include"GLGXMath.h"
#include<string>



#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT                 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT                 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT                 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT                 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT                 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT                 0x851A
#define GL_TEXTURE_CUBE_MAP_EXT                            0x8513

bool GLGXCreateEmptyTexture(GLuint* pTextureID, GLint width, GLint height, int red, int green, int blue, int alpha )
{	
	unsigned long* TextureData = NULL;
	TextureData = (unsigned long*)new unsigned long[((width * height)* 4 * sizeof(unsigned long))];
	if( TextureData == NULL )
		return false;

	for( int i = 0 ; i < (width * height) ; ++i )
	{
		unsigned char* pPixelData	= (unsigned char*)(&TextureData[i]);
		pPixelData[0] = red;	
		pPixelData[1] = green;	
		pPixelData[2] = blue;		
		pPixelData[3] = alpha;	
	}

	glGenTextures(1, pTextureID);							
	glBindTexture(GL_TEXTURE_2D, *pTextureID);				
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, TextureData);						

	delete [] TextureData;
	return true;
}


bool GLGXCreateTextureFromFile(char* filename,GLuint* pTextureID)
{
	GLGXIMAGE image;
	if(image.LoadFromFile(filename))
	{ 
		std::string fileName = filename;
		if( fileName.find(".dds") != std::string::npos )
		{
			int offset		= 0;
			int blockSize = (image.Format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;

			glGenTextures(1, pTextureID);
			glBindTexture(GL_TEXTURE_2D, *pTextureID);

			int width  = image.Width;
			int height = image.Height;
			for(int i = 0; i < image.NumMipMaps; i++)
			{
				int iSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
				glCompressedTexImage2DARB(GL_TEXTURE_2D, i, image.Format, width, height, 0, iSize, image.Data + offset);
				offset += iSize;

				width  /= 2;
				height /= 2;
			}
		}
		else
		{
		  glGenTextures(1, pTextureID);				    
		  glBindTexture(GL_TEXTURE_2D, *pTextureID);
		  //glTexImage2D(GL_TEXTURE_2D, 0, /*image.BPP*/3, image.Width,image.Height, 0, GL_RGB/*image.Format*/,GL_UNSIGNED_BYTE,image.Data);
		  glTexImage2D(GL_TEXTURE_2D, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
		}
      return true;
    }

    return false;
}

bool GLGXCreateCubeTextureFromFile(
	char *filenameRight,
	char *filenameLeft,
	char *filenameUp,
	char *filenameDown,
	char *filenameFront,
	char *filenameBack,
	GLuint* pTextureID
)
{
	
  GLGXIMAGE image;

  glGenTextures(1, pTextureID);				    
  glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, *pTextureID);

  if(image.LoadFromFile(filenameRight))
  { 
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
	 
  }
  else
  {
	return false;
  }
  if(image.LoadFromFile(filenameLeft))
  { 
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
  }
  else
  {
	return false;
  }
  if(image.LoadFromFile(filenameUp))
  { 
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
  }
  else
  {
	return false;
  }
  if(image.LoadFromFile(filenameDown))
  { 
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
  }
  else
  {
	return false;
  }
  if(image.LoadFromFile(filenameFront))
  { 
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
  }
  else
  {
	return false;
  }
  if(image.LoadFromFile(filenameBack))
  { 
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
  }
  else
  {
	return false;
  }
	
  return true;
}


bool GLGXCreateNormalisationCubeMapTexture(GLuint* pTextureID,int size)
{
	float halfSize= ((float)size)/2.0f;
	GLGXVECTOR3 vec;
	unsigned char * textureData = NULL;
	unsigned char * pPixelData  = NULL;

	textureData = new unsigned char[size*size*3];
	if( textureData == NULL )
		return false;

	for( int face = 0 ; face < 6 ; ++face )
	{
		pPixelData=textureData;
		for( int y = 0; y < size; y++ )
		{
			for( int x = 0; x < size; x++ )
			{
				if( face == 0 ) vec = GLGXVECTOR3(halfSize,-(y + 0.5f - halfSize),-(x + 0.5f - halfSize));
				else if( face == 1 ) vec = GLGXVECTOR3(-halfSize,-(y+0.5f-halfSize),x+0.5f-halfSize);
				else if( face == 2 ) vec = GLGXVECTOR3(x+0.5f-halfSize,halfSize,y+0.5f-halfSize);
				else if( face == 3 ) vec = GLGXVECTOR3(x+0.5f-halfSize,-halfSize,-(y+0.5f-halfSize));
				else if( face == 4 ) vec = GLGXVECTOR3(x+0.5f-halfSize,-(y+0.5f-halfSize),halfSize);
				else if( face == 5 ) vec = GLGXVECTOR3(-(x+0.5f-halfSize),-(y+0.5f-halfSize),-halfSize);

				GLGXVec3Normalize(&vec,&vec);
				GLGXVec3PackTo01(&vec,&vec);

				pPixelData[0]=(unsigned char)(vec.x*255);
				pPixelData[1]=(unsigned char)(vec.y*255);
				pPixelData[2]=(unsigned char)(vec.z*255);

				pPixelData+=3;
			}
		}
		glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT+face,
						0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	}

	delete [] textureData;

	return true;
}



