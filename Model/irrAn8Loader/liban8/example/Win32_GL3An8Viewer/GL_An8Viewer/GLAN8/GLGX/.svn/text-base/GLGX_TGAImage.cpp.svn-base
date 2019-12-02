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

// Thanks to nehe.gamedev.net (Lesson 33)

#include "GLGX_TGAImage.h"
#include <stdlib.h>
#include <string.h>


typedef struct
{
	unsigned char Header[12];
} AbyTGA_TGAHeader;


typedef struct
{
	unsigned char		header[6];			// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;		// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;			// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;				// Temporary Variable
	unsigned int		type;	
	unsigned int		Height;				//Height of Image
	unsigned int		Width;				//Width ofImage
	unsigned int		Bpp;				// Bits Per Pixel
} AbyTGA_TGA;



GLGXTGAImage::GLGXTGAImage(void)
{
  NumMipMaps = 1;
}   

GLGXTGAImage::~GLGXTGAImage(void)
{
}   

unsigned char* GLGXTGAImage::LoadFromFile(char * filename,unsigned char *i_Data)
{
  FILE * fTGA;		
  unsigned char uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
  unsigned char cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header
  
  AbyTGA_TGAHeader tgaHeader; 
  										
  fTGA = fopen(filename, "rb");								

  if(fTGA == NULL)										
  {
    return NULL;
  }

  if(fread(&tgaHeader, sizeof(AbyTGA_TGAHeader), 1, fTGA) == 0)				
  {
    if(fTGA != NULL)												
    {
      fclose(fTGA);												
    }
    return NULL;													
  }

  if(memcmp(uTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)			
  {																	
    return LoadUncompTGA(filename,i_Data, fTGA);					
  }
  else if(memcmp(cTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)		
  {																	
	return LoadCompressedTGA(filename,i_Data, fTGA);						
  }
  else																
  {
    //MessageBox(NULL, "TGA file be type 2 or type 10 ", "Invalid Image", MB_OK);	
    fclose(fTGA);
    return NULL;															
  }
  return NULL;	  
}    


unsigned char* GLGXTGAImage::LoadUncompTGA(char * filename,unsigned char *i_Data, FILE * fTGA)
{						
    AbyTGA_TGA tgaTemp;													
	if(fread(tgaTemp.header, sizeof(tgaTemp.header), 1, fTGA) == 0)		
	{										
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return NULL;														// Return failular
	}	

	Width  = tgaTemp.header[1] * 256 + tgaTemp.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	Height = tgaTemp.header[3] * 256 + tgaTemp.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	BPP	= tgaTemp.header[4];										// Determine the bits per pixel
	tgaTemp.Width		= Width;										// Copy width into local structure						
	tgaTemp.Height		= Height;										// Copy height into local structure
	tgaTemp.Bpp			= BPP;											// Copy BPP into local structure

	if((Width <= 0) || (Height <= 0) || ((BPP != 24) && (BPP !=32)))	// Make sure all information is valid
	{
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return NULL;														// Return failed
	}

	if(BPP == 24)													//If the BPP of the image is 24...
	{
		Format	= 0x1907;//GL_RGB;											// Set Image type to GL_RGB
	}
	else																	// Else if its 32 BPP
	{
		Format	= 0x1908;//GL_RGBA;											// Set image type to GL_RGBA
	}

	BPP = tgaTemp.bytesPerPixel	= (tgaTemp.Bpp / 8);									// Compute the number of BYTES per pixel
	tgaTemp.imageSize		= (tgaTemp.bytesPerPixel * tgaTemp.Width * tgaTemp.Height);		// Compute the total amout ofmemory needed to store data
	i_Data	= (unsigned char *)malloc(tgaTemp.imageSize);					// Allocate that much memory

	if(i_Data == NULL)											// If no space was allocated
	{
		fclose(fTGA);														// Close the file
		return NULL;														// Return failed
	}

	if(fread(i_Data, 1, tgaTemp.imageSize, fTGA) != tgaTemp.imageSize)	// Attempt to read image data
	{
		if(i_Data != NULL)										// If imagedata has data in it
		{
			free(i_Data);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return NULL;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	for(unsigned int cswap = 0; cswap < (int)tgaTemp.imageSize; cswap += tgaTemp.bytesPerPixel)
	{
		i_Data[cswap] ^= i_Data[cswap+2] ^=
		i_Data[cswap] ^= i_Data[cswap+2];
	}

	fclose(fTGA);
 		
									

	return i_Data;														
}

unsigned char* GLGXTGAImage::LoadCompressedTGA(char * filename,unsigned char *i_Data, FILE * fTGA)		// Load COMPRESSED TGAs
{ 
	AbyTGA_TGA tga;	
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Attempt to read header
	{
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
		return NULL;														// Return failed
	}

	Width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	Height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	BPP	= tga.header[4];										// Determine Bits Per Pixel
	tga.Width		= Width;										// Copy width to local structure
	tga.Height		= Height;										// Copy width to local structure
	tga.Bpp			= BPP;											// Copy width to local structure

	if((Width <= 0) || (Height <= 0) || ((BPP != 24) && (BPP !=32)))	//Make sure all texture info is ok
	{
		if(fTGA != NULL)													// Check if file is open
		{
			fclose(fTGA);													// Ifit is, close it
		}
		return NULL;														// Return failed
	}

	if(BPP == 24)													//If the BPP of the image is 24...
	{
		Format	= 0x1907;//GL_RGB;											// Set Image type to GL_RGB
	}
	else																	// Else if its 32 BPP
	{
		Format	= 0x1908;//GL_RGBA;											// Set image type to GL_RGBA
	}

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
	BPP = tga.bytesPerPixel;
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	i_Data	= (unsigned char *)malloc(tga.imageSize);					// Allocate that much memory

	if(i_Data == NULL)											// If it wasnt allocated correctly..
	{
		fclose(fTGA);														// Close file
		return NULL;														// Return failed
	}

	unsigned int pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	unsigned int currentpixel	= 0;												// Current pixel being read
	unsigned int currentbyte	= 0;												// Current byte 
	unsigned char * colorbuffer = (unsigned char *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

	do
	{
		unsigned char chunkheader = 0;											// Storage for "chunk" header

		if(fread(&chunkheader, sizeof(unsigned char), 1, fTGA) == 0)				// Read in the 1 byte header
		{
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(i_Data != NULL)									// If there is stored image data
			{
				free(i_Data);									// Delete image data
			}
			return NULL;													// Return failed
		}

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
				{

					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						free(colorbuffer);												// If so, delete it
					}

					if(i_Data != NULL)										// See if there is stored Image data
					{
						free(i_Data);										// If so, delete it too
					}

					return NULL;														// Return failed
				}
																						// write to memory
				i_Data[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
				i_Data[currentbyte + 1	] = colorbuffer[1];
				i_Data[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					i_Data[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(i_Data != NULL)										// If there is Image data
					{
						free(i_Data);										// delete it
					}

					return NULL;														// Return failed
				}
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
			{	
				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					free(colorbuffer);													// delete it
				}

				if(i_Data != NULL)											// If thereis image data
				{
					free(i_Data);											// delete it
				}

				return NULL;															// return failed
			}

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				i_Data[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
				i_Data[currentbyte + 1	] = colorbuffer[1];
				i_Data[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					i_Data[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(i_Data != NULL)										// If there is Image data
					{
						free(i_Data);										// delete it
					}

					return NULL;														// Return failed
				}
			}
		}
	}

	while(currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(fTGA);																		// Close the file
	return i_Data;																		// return success
}

