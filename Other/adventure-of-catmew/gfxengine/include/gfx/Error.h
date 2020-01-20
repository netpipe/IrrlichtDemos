// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ErrorGfxHeader_
#define _ErrorGfxHeader_

#include <string>
#include "gfx/Vector.h"

namespace lew
{
	namespace gfx
	{
		namespace err
		{
			//! general exception from which every other exception in this namespace derrives.
			//! if you are not interested in the error which is thrown by the library,
			//! you can catch this generic one.
			class Error {};
			
			class VideoMode : public Error //can't initialize video mode
			{
				public:
					VideoMode(const Vector pDimension, const bool pFullscreen) : 
						Dimension(pDimension), Fullscreen(pFullscreen) {}
						
					Vector Dimension;
					bool Fullscreen;
			};
			
			class Platform : public Error //unsupported platform
			{
				public:
					Platform(const std::string pName) : Name(pName) {}
					
					std::string Name;
			};
			
			//! this exception is thrown whenever accessing/opening a file failed.
			class File : public Error
			{
				public: 
					//! constructor
					//! \param pFile: the name of the file that could not be accessed.
					File(const std::string pFile) : Name(pFile) {}
					
					//! stores the file name.
					std::string Name;
			}; 
			
			//! this exception is thrown whenever a directory could not be opened.
			class Directory : public Error
			{
				public:
					//! constructor
					//! \param pDir: the name of the directory that could not be opened.
					Directory(const std::string pDir) : Name(pDir) {}
					
					//! stores the directory name.
					std::string Name;
			};
			
			class Font : public Error //can't generate font character
			{
				public:
					Font(const std::string pFile, const std::string pChar, 
						const int pSize, const bool pBold = false, 
						const bool pItalic = false)
					{
						FileName	= pFile;
						Character	= pChar;
						Size		= pSize;
						Bold		= pBold;
						Italic		= pItalic;
					}
					
					std::string FileName;
					std::string Character;
					int Size;
					
					bool Bold;
					bool Italic;
			};
			
			class Texture : public Error //can't generate texture
			{
				public:
					Texture(const std::string pFile, const Vector pSize) : 
						FileName(pFile), Size(pSize) {}
					
					std::string FileName;
					Vector Size;
			};
			
			class Shader : public Error //can't generate shader
			{
				public:
					Shader(const std::string pOutput) : Output(pOutput) {}
						
					std::string Output;				
			};
			
			class VertexShader : public Shader //can't compile vertex-shader
			{
				public:
					VertexShader(const std::string pSource, 
					const std::string pOutput) : Shader(pOutput)
					{
						Source = pSource;
					}
						
					std::string Source;
			};
			
			class FragmentShader : public Shader //can't compile fragment-shader
			{
				public:
					FragmentShader(const std::string pSource, 
					const std::string pOutput) : Shader(pOutput)
					{
						Source = pSource;
					}
						
					std::string Source;
			};
		};
	};
};

#endif

