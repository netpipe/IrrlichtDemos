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

#include "GLGXTexture.h"
#include<stdio.h>

#ifdef _GLGX_IMAGE_USE_WIN32IPICTURE
	#include"GLGX_Win32Image.h"
#endif //_GLGX_IMAGE_USE_WIN32IPICTURE

#ifdef _GLGX_IMAGE_USE_DEVIL
	#include"GLGX_DevILImage.h"
#endif //_GLGX_IMAGE_USE_DEVIL

#include "GLGX_lodepng.h"
#include "GLGX_DDSImage.h"
#include "GLGX_TGAImage.h"

#include<string>
using namespace std;

GLGXIMAGE::GLGXIMAGE()
{
		Data = NULL;
		Width  = 0;
		Height = 0;
		NumMipMaps = 1;
};

GLGXIMAGE::~GLGXIMAGE()
{
	Release();
};

void GLGXIMAGE::Release(void)
{
	if( Data != NULL )
	{
		delete [] Data;
		Data = NULL;
	}
}

bool GLGXIMAGE::LoadFromFile(char* filename)
{
	string fileName = filename;

	// Always use GLGX own code to load dds
	if( fileName.find(".dds") != string::npos || fileName.find(".DDS") != string::npos)
	{
		DDSImage image;
		Data = image.LoadFromFile(filename,(unsigned char*)Data);
		if( Data == NULL )
			return false;
		this->Width  = image.Width;
		this->Height = image.Height;
		this->BPP    = image.BPP;
		this->Format = image.Format;
		this->NumMipMaps = image.NumMipMaps;
		return true;
	}



#ifdef _GLGX_IMAGE_USE_WIN32IPICTURE

	if(    fileName.find(".bmp") != string::npos || fileName.find(".BMP") != string::npos
		|| fileName.find(".jpg") != string::npos || fileName.find(".JPG") != string::npos
		|| fileName.find(".gif") != string::npos || fileName.find(".GIF") != string::npos)
	{
		Win32Image image;
		Data = image.LoadFromFile(filename,(unsigned char*)Data);
		if( Data == NULL )
			return false;
		this->Width  = image.Width;
		this->Height = image.Height;
		this->BPP    = image.BPP;
		this->Format = image.Format;
		return true;
	}

#endif // _GLGX_IMAGE_USE_WIN32IPICTURE

#ifdef _GLGX_IMAGE_USE_DEVIL
		DevILImage image;
		Data = image.LoadFromFile(filename,(unsigned char*)Data);

		if( Data == NULL )
			return false;

		this->Width  = image.Width;
		this->Height = image.Height;
		this->BPP    = image.BPP;
		this->Format = image.Format;
		this->NumMipMaps = image.NumMipMaps;
		return true;
#endif // _GLGX_IMAGE_USE_DEVIL

	// If we are here DevIL has not been used

	if( fileName.find(".png") != string::npos || fileName.find(".PNG") != string::npos
		|| fileName.find(".tga") != string::npos || fileName.find(".TGA") != string::npos)
	{
		if( fileName.find(".png") != string::npos || fileName.find(".PNG") != string::npos)
		{
			unsigned char* buffer;
			size_t buffersize, imagesize;
			LodePNG_Decoder decoder;
			LodePNG_loadFile(&buffer, &buffersize, filename);
			LodePNG_Decoder_init(&decoder);
			LodePNG_decode(&decoder, &Data, &imagesize, buffer, buffersize);
			if( Data == NULL )
				return false;
			this->Width  = decoder.infoPng.width;
			this->Height = decoder.infoPng.height;
			this->BPP    = 4;
			this->Format = 6408; // RGBA

			free(buffer);
			return true;
		}
		else if( fileName.find(".tga") != string::npos || fileName.find(".TGA") != string::npos )
		{
			GLGXTGAImage image;
			Data = image.LoadFromFile(filename,(unsigned char*)Data);
			if( Data == NULL )
				return false;
			this->Width  = image.Width;
			this->Height = image.Height;
			this->BPP    = image.BPP;
			this->Format = image.Format;
			this->NumMipMaps = image.NumMipMaps;
			return true;
		}
	}

	return false;
}

void GLGXIMAGE::SetColorKey(int keyR, int keyG, int keyB, int newR, int newG,int newB)
{
	if( Data == NULL)
		return;

	for( int i = 0 ; i < this->Width*this->Height; ++i)
	{
		if( Data[i*this->BPP] == keyR && Data[i*this->BPP+1] == keyG && Data[i*this->BPP+2] == keyB )
		{
			Data[i*this->BPP]   = newR;
			Data[i*this->BPP+1] = newG;
			Data[i*this->BPP+2] = newB;
		}
	}
}

void GLGXIMAGE::SetColorKey(int keyR, int keyG, int keyB, int newR, int newG,int newB, int newA )
{
	if( Data == NULL)
		return;

	if( this->BPP == 3 )
	{
		unsigned char* tempData = new unsigned char[Width*Height*4];

		for( int i = 0 ; i < this->Width*this->Height; ++i)
		{
			if( Data[i*this->BPP] == keyR && Data[i*this->BPP+1] == keyG && Data[i*this->BPP+2] == keyB )
			{
				tempData[i*4]   = newR;
				tempData[i*4+1] = newG;
				tempData[i*4+2] = newB;
				tempData[i*4+3] = newA;
			}
			else
			{
				tempData[i*4]   = Data[i*this->BPP];
				tempData[i*4+1] = Data[i*this->BPP+1];
				tempData[i*4+2] = Data[i*this->BPP+2];
			}
		}

		delete [] Data;
		Data = NULL;
		Data = new unsigned char[Width*Height*4];
		for( int i = 0 ; i < this->Width*this->Height*4; ++i)
		{
				Data[i] = tempData[i];
		}
		this->BPP    = 4; 
		this->Format = 6408; 

		delete [] tempData;
		tempData = NULL;
	}
	else if( this->BPP == 4 )
	{
		for( int i = 0 ; i < this->Width*this->Height; ++i)
		{
			if( Data[i*this->BPP] == keyR && Data[i*this->BPP+1] == keyG && Data[i*this->BPP+2] == keyB )
			{
				Data[i*4]   = newR;
				Data[i*4+1] = newG;
				Data[i*4+2] = newB;
				Data[i*4+3] = newA;
			}
		}
	}
}

void GLGXIMAGE::FillAllWithRGB( int newR, int newG, int newB )
{
	if( Data == NULL)
		return;

	for( int i = 0 ; i < this->Width*this->Height; ++i)
	{
		Data[i*this->BPP]   = newR;
		Data[i*this->BPP+1] = newG;
		Data[i*this->BPP+2] = newB;
	}
}

void GLGXIMAGE::SetAverageColorAsAlpha( int threshold )
{
	if( Data == NULL)
		return;

	if( this->BPP == 3 )
	{
		unsigned char* tempData = new unsigned char[Width*Height*4];

		for( int i = 0 ; i < this->Width*this->Height; ++i)
		{
			float averageColor = (float)Data[i*this->BPP] + (float)Data[i*this->BPP+1] + (float)Data[i*this->BPP+2];
			int alpha = (int) (averageColor / 3.0f);
			 
			if( alpha > threshold )
				averageColor = 255;

			tempData[i*4]   = Data[i*this->BPP];
			tempData[i*4+1] = Data[i*this->BPP+1];
			tempData[i*4+2] = Data[i*this->BPP+2];
			tempData[i*4+3] = (unsigned char) averageColor;

		}

		delete [] Data;
		Data = NULL;
		Data = new unsigned char[Width*Height*4];
		for( int i = 0 ; i < this->Width*this->Height*4; ++i)
		{
				Data[i] = tempData[i];
		}
		this->BPP    = 4; 
		this->Format = 6408; 

		delete [] tempData;
		tempData = NULL;
	}
	else if( this->BPP == 4 )
	{
		for( int i = 0 ; i < this->Width*this->Height; ++i)
		{
			float averageColor = (float)Data[i*this->BPP] + (float)Data[i*this->BPP+1] + (float)Data[i*this->BPP+2];
			Data[i*4+3] = (unsigned char)(averageColor / 3.0f);
		}
	}
}


