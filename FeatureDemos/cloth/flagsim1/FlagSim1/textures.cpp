#include "Textures.h"
#include <stdio.h>

#define EIGHT_BIT_FORMAT GL_ALPHA

//**************
//Only for loading bmps:
void COGLTexture::LoadFromFile(char *filename)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1,&ID); 
	glBindTexture( GL_TEXTURE_2D, ID);
	Image = auxDIBImageLoadA( (const char*) filename );
	Width = Image->sizeX;
	Height = Image->sizeY;

	gluBuild2DMipmaps(	GL_TEXTURE_2D, 
						3, 
						Image->sizeX,
						Image->sizeY,
						GL_RGB,
						GL_UNSIGNED_BYTE,
						Image->data);
	delete Image;
}
////////////////////////////////////////////////////////////////////////////
///
///			CODE FROM NEHE'S "LESSON 25": Loading a TGA
///			(modified)
///
////////////////////////////////////////////////////////////////////////////
bool LoadTGA(STGAData *TGAData, const char *filename)			// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader1[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header for RGB / RGBA
	GLubyte		TGAheader2[12]={0,1,1,0,0,0,1,24,0,0,0,0};	// Uncompressed TGA Header for Grayscale
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if(	file==NULL)
		return false;										// Does File Exist?
	
	if (fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		(	memcmp(TGAheader1,TGAcompare,sizeof(TGAheader1))!=0
		  &&memcmp(TGAheader2,TGAcompare,sizeof(TGAheader2)) ) ||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			//fclose(file);									// If Anything Failed, Close The File
			//return false;									// Return False
		}
	}

	TGAData->Width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	TGAData->Height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	//I don't need to check here for the bpp. This will be done by
	//the calling function.



	TGAData->BitsPerPixel	= header[4];							// Grab The TGA's Bits Per Pixel
	bytesPerPixel	= TGAData->BitsPerPixel/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= TGAData->Width*TGAData->Height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	if (bytesPerPixel == 1) 
	{
		GLubyte		DummyData[768];	
		fread(DummyData,1,768,file);
	}

	TGAData->Data=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(	TGAData->Data==NULL ||							// Does The Storage Memory Exist?
		fread(TGAData->Data, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(TGAData->Data!=NULL)						// Was Image Data Loaded
			free(TGAData->Data);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	//I also do not swap Red and Blue values (this may only be done
	//with 24/32-bpp-images). Again it's the calling routine's task.

	fclose (file);											// Close The File

	return true;											
}



bool COGLTexture::LoadFromTGA(const char *filename,const char *AlphaMap, bool bMipMaps)
{
	STGAData TGAData;			//Used to load the TGA-Texture
	STGAData AlphaData;			//Used to load the Alpha-Map (if specified)
	
	GLubyte * RGBAData = NULL;	//RGBA Data. This can be the TGA file (if it contains 32bpp)
								//or it is the combination of TGA and alpha map.
								//If no alpha map is requested, it contains RGB data
	GLuint RGBADataLength;		//Length of RGBA Data

	type = GL_RGB;

	char * LogStr;	
	
	if (filename != NULL)
		{
		if (!LoadTGA(&TGAData,filename))  //Load the texture file into memory
		{
			LogStr = new char[2000];
			sprintf(LogStr,"Failed to load Texture (%s, %s). Reason: Failed to load RGB file.");
			return false;	//failed. Return false.
		}


		if (TGAData.BitsPerPixel == 32)
		{
			type = GL_RGBA;  //TGA is a 32 bit file (i.e. it contains the alpha information)
			RGBAData = TGAData.Data;	//The TGA Data can be used to create the texture
			RGBADataLength = TGAData.Height*TGAData.Height*TGAData.BitsPerPixel/8;
		}
		Width = TGAData.Width;
		Height = TGAData.Height;
	}
	else
		type = EIGHT_BIT_FORMAT;

	if (type != GL_RGBA)	//ignore Alphamap if TGA has 32 bpp
	{
		if (AlphaMap != NULL)  //Alphamap specified
		{//AlphaMap requested:
			if (!LoadTGA(&AlphaData,AlphaMap))	//Load alpha map from file into memory
			{
				LogStr = new char[2000];
				sprintf(LogStr,"Failed to load Texture (%s, %s). Reason: Failed to load alpha file.");

				return false;		//failed. Return false.
			}
						
			//Check if the TGA has 24 bpp: (if it even exists)
			if ((type == GL_RGB) && (TGAData.BitsPerPixel != 24))
			{
				LogStr = new char[2000];
				sprintf(LogStr,"Failed to load Texture (%s, %s). Reason: RGB file not in 8 Bit format!");
				return false;
			}
			//Check if the Alpha Map has 8 bpp (grayscale):
			if (AlphaData.BitsPerPixel != 8)	
			{
				LogStr = new char[2000];
				sprintf(LogStr,"Failed to load Texture (%s, %s). Reason: Alpha file not in 8 bit format.");
				return false;
			}
			//Check if the size of AlphaData and TGAData is equal: (if the TGA exists)
			if ((type == GL_RGB) && ((AlphaData.Width != TGAData.Width)||(AlphaData.Height != TGAData.Height)))
			{
				LogStr = new char[2000];
				sprintf(LogStr,"Failed to load Texture (%s, %s). Reason: Alpha size does not eqal RGB size.");
				return false;
			}
	
			if (type == GL_RGB)
			{
				type = GL_RGBA;
				Width = TGAData.Width;
				Height = TGAData.Height;
				//Now comes the tricky part: Combine the TGAData and the alpha map.
				RGBADataLength = TGAData.Width*TGAData.Height*4; //4 = (24+8)/8; we need RGBA!
				RGBAData=(GLubyte *)malloc(RGBADataLength);
				//Check if malloc succeeded:
				if (RGBAData==NULL)
				{
					LogStr = new char[2000];
					sprintf(LogStr,"Failed to load Texture (%s, %s). Reason: Out of memory.");
					return false;
				}

				//Loop. Copy three bytes of the TGA, then one byte of the alpha map. 
				//We do not swap R and B here, this is done at the end.
				for (GLuint i = 0; i < RGBADataLength/4; i++)
				{
					RGBAData[i*4+0] = TGAData.Data[i*3+0];
					RGBAData[i*4+1] = TGAData.Data[i*3+1];
					RGBAData[i*4+2] = TGAData.Data[i*3+2];
					RGBAData[i*4+3] = AlphaData.Data[i];
				}
			}
			else  //if type == EIGHT_BIT_FORMAT
			{
				RGBADataLength = AlphaData.Width*AlphaData.Height;
				RGBAData=(GLubyte *)malloc(RGBADataLength);
				for (GLuint i = 0; i < RGBADataLength; i++)
				{
				
					RGBAData[i] = AlphaData.Data[i];

				}
				type = GL_ALPHA;
				Width = AlphaData.Width;
				Height = AlphaData.Height;
			}

		}
		else					//No Alphamap specified
		{
			//Store the TGAData in the local variables. This is RGB Data!
			RGBAData = TGAData.Data;
			RGBADataLength = TGAData.Height*TGAData.Height*TGAData.BitsPerPixel/8;
		}
	}
	
	//Now swap the R and B values. Be careful. Do not forget to check if we have RGBA or RGB data!
	GLuint SwapC;	//Counter for swapping
	int TempR;	//To Swap we need a temporary variable. This var here will store the temp R value.
	
	int BytesPerPixel = 3;
	if (type == GL_RGBA)
		BytesPerPixel = 4;
	if (type == EIGHT_BIT_FORMAT)
		BytesPerPixel = 1;

	if (BytesPerPixel > 1) 
	for (SwapC = 0; SwapC < RGBADataLength/BytesPerPixel;SwapC++)  
	{
		TempR = RGBAData[SwapC*BytesPerPixel+0];
		RGBAData[SwapC*BytesPerPixel+0] = RGBAData[SwapC*BytesPerPixel+2];
		RGBAData[SwapC*BytesPerPixel+2] = TempR;
	}

	

	if (bMipMaps)
	{
		glGenTextures(1,&ID); 
		glBindTexture( GL_TEXTURE_2D, ID);
		GLuint res = gluBuild2DMipmaps(	GL_TEXTURE_2D, 
							BytesPerPixel, //computed earlier,
							Width,
							Height,
							type,
							GL_UNSIGNED_BYTE,
							RGBAData);
		if (res != 0)
		{
			return false;

		}
	}
	else
	{
		glGenTextures(1, &ID);					// Generate OpenGL texture IDs
		glBindTexture(GL_TEXTURE_2D, ID);			// Bind Our Texture
		glTexImage2D(GL_TEXTURE_2D, 0, BytesPerPixel, Width,Height, 0, type, GL_UNSIGNED_BYTE, RGBAData);
	}

	//Store the filenames:
	if (filename == NULL)
		m_TexFilename = NULL;
	else
	{
		m_TexFilename = new char[strlen(filename)+2];
		strcpy(m_TexFilename,filename);
	}
	if (AlphaMap == NULL)
		m_AlphaFilename = NULL;
	else
	{
		m_AlphaFilename = new char[strlen(AlphaMap)+2];
		strcpy(m_AlphaFilename,AlphaMap);
	}


	return true;
}


void COGLTexture::SetActive()
{
	glBindTexture( GL_TEXTURE_2D, ID);
}

unsigned int COGLTexture::GetID()
{
	return ID;
}

GLuint COGLTexture::GetWidth()
{
	return Width;
}
GLuint COGLTexture::GetHeight()
{
	return Height;
}