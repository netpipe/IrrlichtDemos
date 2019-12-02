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

#include "GLGXSetup.h"

#ifdef _GLGX_IMAGE_USE_DEVIL

#ifndef _DevILImage_H
#define _DevILImage_H


#include <IL/il.h>

#include<malloc.h>

#include <stdio.h>
#include <string.h>


class DevILImage
{
private:

	ILuint texid; 

public:

	int Width;
	int Height;
	int BPP;
	int Format; 
	int	NumMipMaps;

	DevILImage()
	{
		Width = 0 ;
		Height = 0;
		BPP = 3;
		Format = 0; 
		NumMipMaps = 1;
	}

	~DevILImage()
	{
	
	}

	unsigned char*  LoadFromFile(char* filename,unsigned char* data)
	{
	  
	  ILboolean success; 

	  if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
		return NULL;
	 
	  ilInit(); 
	  ilGenImages(1, &texid); 
	  ilBindImage(texid);

	  success = ilLoadImage(filename); 
	  if (success) 
	  {
		this->BPP        = ilGetInteger(IL_IMAGE_BPP);

		if( this->BPP == 4 ) // RGBA
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 
		}
		else // Why not if=(BPP == 3) ? Because of grey scale images 
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); 
		}


		if (!success)
		{
		  return NULL;
		}

		this->Width      = ilGetInteger(IL_IMAGE_WIDTH);
		this->Height     = ilGetInteger(IL_IMAGE_HEIGHT);
		this->BPP        = ilGetInteger(IL_IMAGE_BPP);
		this->Format     = ilGetInteger(IL_IMAGE_FORMAT);
		this->NumMipMaps = ilGetInteger(IL_ACTIVE_MIPMAP);

		data = (unsigned char*)malloc( (this->Width*this->Height*this->BPP)*sizeof(unsigned char)); 
		ilCopyPixels(0,0,0,this->Width,this->Height,1,this->Format,IL_UNSIGNED_BYTE,data);

		ilDeleteImages(1, &texid);

		return data;

	  };
	  return NULL;
	}
};


#endif // _DevILImage_H

#endif //_GLGX_IMAGE_USE_DEVIL


