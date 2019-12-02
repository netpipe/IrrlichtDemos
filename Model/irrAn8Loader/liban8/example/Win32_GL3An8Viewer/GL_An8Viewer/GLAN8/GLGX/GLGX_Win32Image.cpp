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
#include "GLGX_Win32Image.h"

#ifdef _GLGX_IMAGE_USE_WIN32IPICTURE


#include<stdio.h>

#define	HIMETRIC_INCH	2540

Win32Image::Win32Image(void)
{
	picture = NULL;
	pictureStreamPtr = NULL;
}

Win32Image::~Win32Image(void)
{
	Release();
}

unsigned char* Win32Image::LoadFromFile(char* filename,unsigned char* data)
{
	HRESULT 		hr;
	unsigned long 	ulFileSize = 0  , ulBytesRead = 0 ;
	HANDLE			hFile;
	char			*dataPtr =NULL;
	HGLOBAL 		hGlobal = NULL ;
	HDC			memDC;				

	FILE *file = NULL;
    file = fopen(filename,"r");
    if( file == NULL)
	{
		data = NULL;
        return NULL;
	}
    else
        fclose(file);
	
	try 
	{
	
		 hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		 if ( hFile == INVALID_HANDLE_VALUE )				
			 throw( "File not found." ) ;
		
	
		ulFileSize = GetFileSize(hFile, NULL);
		if ( ulFileSize == 0xFFFFFFFF || ulFileSize == 0) 		
			throw("File is corrupted.") ;

		dataPtr  = NULL;
		
	
		hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulFileSize);
		if ( hGlobal == NULL )		
			throw("Out of memory") ;

	
		dataPtr = (char*) GlobalLock(hGlobal);	
		if ( dataPtr == NULL )		
			throw("Can't lock memory block.") ;

		
		ReadFile(hFile, dataPtr, ulFileSize, &ulBytesRead, NULL);
		if ( ulBytesRead != ulFileSize)	
			throw("Impossible to read file") ;

	
		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pictureStreamPtr);
		if ( hr != S_OK )		
			throw("Impossible to get picture stream") ;
	
	
		hr = OleLoadPicture(pictureStreamPtr, ulFileSize, FALSE, IID_IPicture , (LPVOID *)&picture);
		if (hr != S_OK )			
			throw("Impossible to get IPICTURE interface") ;

		if( hGlobal )					
		{	
			GlobalUnlock( hGlobal );
			GlobalFree( hGlobal ) ;
		}

		CloseHandle(hFile);
	
	}
	catch( char	*message )
	{

		
		if (pictureStreamPtr)		
			pictureStreamPtr->Release() ;
		if(hGlobal)					
		{	
			GlobalUnlock( hGlobal );
			GlobalFree( hGlobal ) ;
		}

		CloseHandle(hFile);
		data = NULL;
		return NULL;
	}
	  
    picture->get_Width(&pictureWidth);
	picture->get_Height(&pictureHeight);

	memDC	=	CreateCompatibleDC( NULL );		
	
	Width	= MulDiv(pictureWidth,GetDeviceCaps(memDC,LOGPIXELSX),HIMETRIC_INCH);
	Height	= MulDiv(pictureHeight,GetDeviceCaps(memDC,LOGPIXELSY),HIMETRIC_INCH);  
	
	DeleteObject( memDC );		
	  
	return GetData(data);
}

unsigned char* Win32Image::GetData(unsigned char* data)
{
	RECT		r = {0,0,320,240 };
	HBITMAP		memBitmap , oldBitmap;	
	HDC			memDC;				
	BITMAPINFO	bmi;

    memDC	=	CreateCompatibleDC( NULL );	

	memset( &bmi,0,sizeof(BITMAPINFO) ) ;
	bmi.bmiHeader.biSize			=	sizeof (BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth			=	Width;  
	bmi.bmiHeader.biHeight			=	Height;	
	bmi.bmiHeader.biPlanes			=	1;
	bmi.bmiHeader.biBitCount		=	32;  //16 = 16 bits  24 = 24 bits et 32 = 32 bits
	bmi.bmiHeader.biCompression		=	BI_RGB;
	bmi.bmiHeader.biSizeImage		=	0;
	bmi.bmiHeader.biXPelsPerMeter	=	0;
	bmi.bmiHeader.biYPelsPerMeter	=	0;
	bmi.bmiHeader.biClrUsed			=	0;
	bmi.bmiHeader.biClrImportant	=	0;
		
	memBitmap=CreateDIBSection(memDC, 
							   &bmi,
							   DIB_RGB_COLORS, 
							   (void**)&pixelPtr, 
							   NULL,
							   0
	);
																 															
	oldBitmap=( HBITMAP)SelectObject(memDC,memBitmap);	

	picture->Render(memDC,
					0,0,Width,Height,				
					0,pictureHeight,pictureWidth,-pictureHeight,	
					NULL  
	);
	
	// BGR -> RGB 
	long i;
 	for(i = 0; i < Width * Height; i++)			
	{
		BYTE* pPixel	= (BYTE*)(&pixelPtr[i]);							
		BYTE  temp		= pPixel[0];									
	 	pPixel[0]		= pPixel[2];								
		pPixel[2]		= temp;											
									
	}

	this->BPP    = 3;
	//this->BPP    = 4; // RGBA
	this->Format = GL_RGB; // RGB
	//this->Format = 6408; // RGBA

	data = new unsigned char[Width*Height*this->BPP];

	for(i=0;i<Width*Height;++i)
	{
	  BYTE* pPixel	    = (BYTE*)(&pixelPtr[i]);	

	  data[i*this->BPP]   = pPixel[0];
	  data[i*this->BPP+1] = pPixel[1];
	  data[i*this->BPP+2] = pPixel[2];
	  //data[i*this->BPP+3] = 255; // Alpha
	}

	SelectObject( memDC,oldBitmap );	
	DeleteObject( memBitmap );			
	DeleteObject( memDC );		

	Release();

	return data;
}

void Win32Image::Release( void )
{
	if(picture != NULL)
	{
		picture->Release() ;
		picture = NULL;
	}
	
	if ( pictureStreamPtr != NULL )		
	{
		pictureStreamPtr->Release() ;
		pictureStreamPtr = NULL;
	}

}

#endif //_GLGX_IMAGE_USE_WIN32IPICTURE


