// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CTextureGfxHeader_
#define _CTextureGfxHeader_

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_ttf.h>

#include "gfx/ITexture.h"

namespace lew
{
	namespace gfx
	{
		class IRoot;
		class CTexture : public ITexture
		{
			public:
				CTexture(IRoot* m_pRoot, const bool pStoreTextureData);
				~CTexture();

				void create(const std::string pFile);
				void create(Vector pSize, const bool pStoreTextureData = false);

				void create(const std::string pFont, const std::string pChar, 
					const int pSize, const bool pBold, const bool pItalic);

				void setPixel(Vector pPixel, const Color pColor);
				Color getPixel(Vector pPixel);

				void mask(Vector pPixel);
				void mask(Color pPixel);

				void lockTexture();
				void unlockTexture();

				Vector getDimension() const;
				Vector getOriginalDimension() const;
				std::string getFileName() const;
				bool getManipulated() const;
				bool getEditable() const;
				
				void update();
				void save(const std::string pFileName) const;

				unsigned long int getInternalHandle(TextureMode pFilter);
				
				void setRenderTarget(const bool pFlag);
				bool isRenderTarget() const;


			private:			
				Vector validatePixel(Vector pPixel);
				
				IRoot* pRoot;

				GLuint FilteredImage[2];

				SDL_Surface* Image;
				std::string	FileName;

				Vector	Dimension;
				Vector OriginalDimension;

				bool LockedTexture;
				bool Debug;
				bool Manipulated;
				bool Font;
				bool RenderTarget;
				bool StoreTextureData;
				bool ValidTextures;
				
				Vector MaxDimension;
		};
	};
};

#endif

