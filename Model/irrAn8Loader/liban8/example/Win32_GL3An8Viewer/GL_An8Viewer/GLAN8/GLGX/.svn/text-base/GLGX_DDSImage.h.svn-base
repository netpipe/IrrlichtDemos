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

#ifndef _DDS_H_
#define _DDS_H_

#ifndef MAKEFOURCC
	#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
		((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) | \
		((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24))
#endif

//FOURCC codes for DX compressed-texture pixel formats.
#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT2 MAKEFOURCC('D', 'X', 'T', '2')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT4 MAKEFOURCC('D', 'X', 'T', '4')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')

#define CR_DDS_PIXEL_DATA_OFFSET 128
#define CR_FOURCC_OFFSET 84
#define CR_MIPMAP_COUNT_OFFSET 28
#define CR_LINEAR_SIZE_OFFSET 20
#define CR_DDS_WIDTH_OFFSET 16
#define CR_DDS_HEIGHT_OFFSET 12

class DDSImage
{
	public:

		int				Width;
		int				Height;
		int				Format;
		int				BPP;

		int				NumMipMaps;

		DDSImage();
		~DDSImage();

		unsigned char* LoadFromFile(const char *pFileName,unsigned char *i_Data);
};

#endif //_DDS_H_
