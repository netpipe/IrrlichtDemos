// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "Leeuwen" Library.
// For conditions of distribution and use, see copyright notice in Leeuwen.h

#include "gfx/Error.h"
#include "CTexture.h"
#include "CRoot.h"
#include "CVideoDriver.h"

namespace lew
{
	namespace gfx
	{
		CTexture::CTexture(IRoot* m_pRoot, const bool pStoreTextureData) : 
		ITexture()
		{
			pRoot = m_pRoot;
			
			FileName		= "none";
			Image			= 0;
			Debug			= false;
			LockedTexture	= false;
			Manipulated		= false;
			Font			= false;
			RenderTarget	= false;
			ValidTextures	= false;
			StoreTextureData= pStoreTextureData;
			
			int mts = 0;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mts);
			MaxDimension = Vector(mts, mts);
		}

		CTexture::~CTexture()
		{
			((CVideoDriver*)pRoot->getVideoDriver())->removeTexture(this);
			
			if(ValidTextures)
			{
				glDeleteTextures(2, &FilteredImage[0]);
			}
			
			if(StoreTextureData)
			{
				if(Image)
				{
					SDL_FreeSurface(Image);
				}
			}
		}
		
		void CTexture::create(const std::string pFont, const std::string pChar, 
		const int pSize, const bool pBold, const bool pItalic)
		{
			Font = true;
			FileName = pFont;
			TTF_Font* font = TTF_OpenFont(pFont.c_str(), pSize);
			
			if(font)
			{
				if(pBold && !pItalic)
					TTF_SetFontStyle(font, TTF_STYLE_BOLD);
				
				if(pItalic && !pBold)
					TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
				
				if(pItalic && pBold)
					TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
				
				SDL_Color color;
				color.r = 255;
				color.g = 255;
				color.b = 255;

				SDL_Surface* Char = TTF_RenderText_Blended(font, 
					pChar.c_str(), color);
				
				if(Char)
				{
					int maxX, maxY;
					TTF_GlyphMetrics(font, pChar[0], 0, &maxX, 0, &maxY, 0);					
					OriginalDimension = Vector(maxX, maxY);
					
					if(pChar == " ")
					{
						OriginalDimension = pSize;
					}
					
					try
					{
						create(Vector(Char->w, Char->h), true);
					}
					catch(err::Texture& err)
					{
						TTF_CloseFont(font);
						SDL_FreeSurface(Char);
					
						throw(err::Texture(err.FileName, err.Size));
					}
					
					if(Image)
					{
						SDL_SetAlpha(Char, 0, 0);
						SDL_BlitSurface(Char, 0, Image, 0);
									
						Dimension = Vector(Image->w, Image->h);
						update();
						
						if(!StoreTextureData)
						{
							SDL_FreeSurface(Image);
							Image = 0;
						}
					}
					
					SDL_FreeSurface(Char);
				}
				else
				{
					TTF_CloseFont(font);
					throw(err::Font(pFont, pChar, pSize, pBold, pItalic));
				}	

				TTF_CloseFont(font);
			}
			else
			{
				throw(err::File(pFont));
			}
		}

		void CTexture::create(const std::string pFile)
		{
			FileName = pFile;
			SDL_Surface* File = IMG_Load(pFile.c_str());

			if(File)
			{
				OriginalDimension = Vector(File->w, File->h);
				
				try
				{
					create(OriginalDimension, true);
				}
				catch(err::Texture& err)
				{
					SDL_FreeSurface(File);
					throw(err::Texture(err.FileName, err.Size));
				}
				
				if(Image)
				{
					SDL_SetAlpha(File, 0, 0);
					SDL_BlitSurface(File, 0, Image, 0);
				
					Dimension = Vector(Image->w, Image->h);
					update();

					if(!StoreTextureData)
					{
						SDL_FreeSurface(Image);
						Image = 0;
					}
				}

				SDL_FreeSurface(File);
			}
			else
			{
				throw(err::File(FileName));
			}
		}

		void CTexture::create(Vector pSize, const bool pStoreTextureData)
		{
			Vector PowerOfTwo = Vector(1, 1);

			if(pSize.x > MaxDimension.x)
				pSize.x = MaxDimension.x;

			if(pSize.y > MaxDimension.y)
				pSize.y = MaxDimension.y;

			if(pSize.x < 0)
				pSize.x = 0;

			if(pSize.y < 0)
				pSize.y = 0;

			if(!glewIsSupported("GL_ARB_texture_non_power_of_two"))
			{
				while(PowerOfTwo.x < pSize.x)
					PowerOfTwo.x *= 2;

				while(PowerOfTwo.y < pSize.y)
					PowerOfTwo.y *= 2;

				Dimension = PowerOfTwo;
			}
			else
			{
				Dimension = pSize;
			}

			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			Uint32 r = 0xff000000;
			Uint32 g = 0x00ff0000;
			Uint32 b = 0x0000ff00;
			Uint32 a = 0x000000ff;
			#else
			Uint32 r = 0x000000ff;
			Uint32 g = 0x0000ff00;
			Uint32 b = 0x00ff0000;
			Uint32 a = 0xff000000;
			#endif

			Image = SDL_CreateRGBSurface(SDL_SWSURFACE, 
				(int)Dimension.x, (int)Dimension.y, 32, r, g, b, a);
				
			if(Image)
			{
				glGenTextures(2, &FilteredImage[0]);
				update();
				
				if(!pStoreTextureData)
				{
					if(!StoreTextureData)
					{
						SDL_FreeSurface(Image);
						Image = 0;
					}
				}
			}
			else
			{
				throw(err::Texture(FileName, pSize));
			}
		}

		void CTexture::setPixel(Vector pPixel, const Color pColor)
		{
			if(Image)
			{
				lockTexture();

				Manipulated = true;

				pPixel = validatePixel(pPixel);

				Uint8 r,g,b,a;

				r = (Uint8)(255*pColor.r);
				g = (Uint8)(255*pColor.g);
				b = (Uint8)(255*pColor.b);
				a = (Uint8)(255*pColor.a);

				Uint32 Result		= SDL_MapRGBA(Image->format, r, g, b, a);
				int BytesPerPixel	= Image->format->BytesPerPixel;

				Uint8* SPixel		=   (Uint8*)Image->pixels +
										(int)pPixel.y*Image->pitch +
										(int)pPixel.x*BytesPerPixel;

				switch(BytesPerPixel)
				{
					case 1:
					{
						*SPixel = Result;
						break;
					}

					case 2:
					{
						*(Uint16 *)SPixel = Result;
						break;
					}

					case 3:
					{
						if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
						{
							SPixel[0] = (Result >> 16) & 0xff;
							SPixel[1] = (Result >> 8) & 0xff;
							SPixel[2] = Result & 0xff;
						}
						else
						{
							SPixel[0] = Result & 0xff;
							SPixel[1] = (Result >> 8) & 0xff;
							SPixel[2] = (Result >> 16) & 0xff;
						}
						break;
					}

					case 4:
					{
						*(Uint32 *)SPixel = Result;
						break;
					}
				}

				unlockTexture();
			}
		}

		Color CTexture::getPixel(Vector pPixel)
		{
			if(Image)
			{
				lockTexture();

				pPixel = validatePixel(pPixel);

				int BytesPerPixel	= Image->format->BytesPerPixel;
				Uint32 Result		= 0;

				Uint8* SPixel		=   (Uint8*)Image->pixels +
										(int)pPixel.y*Image->pitch +
										(int)pPixel.x*BytesPerPixel;

				switch(BytesPerPixel)
				{
					case 1:
					{
						Result = *SPixel;
						break;
					}

					case 2:
					{
						Result = *(Uint16 *)SPixel;
						break;
					}

					case 3:
					{
						if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
							Result = SPixel[0] << 16 | SPixel[1] << 8 | SPixel[2];
						else
							Result = SPixel[0] | SPixel[1] << 8 | SPixel[2] << 16;

						break;
					}

					case 4:
					{
						Result = *(Uint32*)SPixel;
						break;
					}
				}

				Uint8 r, g, b, a;

				SDL_GetRGBA(Result, Image->format, &r, &g, &b, &a);

				unlockTexture();

				return Color(float(r/255.0), float(g/255.0), 
					float(b/255.0), float(a/255.0));
			}
			
			return Color();
		}

		void CTexture::mask(Vector pPixel)
		{
			mask(getPixel(pPixel));
		}

		void CTexture::mask(Color pPixel)
		{
			if(Image)
			{
				for(unsigned int i = 0; i != Dimension.x; ++i)
				{
					for(unsigned int j = 0; j != Dimension.y; ++j)
					{
						if(getPixel(Vector(i, j)) == pPixel)
						{
							Color color = pPixel;
							color.a = 0;

							setPixel(Vector(i, j), color);
						}
					}
				}
			}
		}

		void CTexture::lockTexture()
		{
			if(Image)
			{
				if(SDL_MUSTLOCK(Image))
					SDL_LockSurface(Image);

				LockedTexture = true;
			}
		}

		void CTexture::unlockTexture()
		{
			if(Image)
			{
				if(LockedTexture)
				{
					if(SDL_MUSTLOCK(Image))
						SDL_UnlockSurface(Image);

					LockedTexture = false;
				}
			}
		}

		Vector CTexture::getDimension() const
		{
			return Dimension;
		}
		
		Vector CTexture::getOriginalDimension() const
		{
			return OriginalDimension;
		}

		unsigned long int CTexture::getInternalHandle(TextureMode pFilter)
		{
			switch(pFilter)
			{
				case ETF_NEAR:
				{
					return FilteredImage[0];
				}
				case ETF_LINEAR:
				{
					return FilteredImage[1];
				}
			}

			return FilteredImage[0];
		}

		Vector CTexture::validatePixel(Vector pPixel)
		{
			pPixel.clamp(Vector(0.0f, 0.0f), Dimension-1);
			return pPixel;
		}

		void CTexture::update()
		{
			if(Image)
			{
				ValidTextures = true;
				
				//NEAR
				glBindTexture(GL_TEXTURE_2D, FilteredImage[0]);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)Dimension.x, 
				(int)Dimension.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
				(Uint32*)Image->pixels);


				//LINEAR
				glBindTexture(GL_TEXTURE_2D, FilteredImage[1]);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)Dimension.x, 
				(int)Dimension.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
				(Uint32*)Image->pixels);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		void CTexture::save(const std::string pFileName) const
		{
			if(Image)
			{
				SDL_SaveBMP(Image, pFileName.c_str());
			}
		}

		bool CTexture::getManipulated() const
		{
			return Manipulated;
		}
		
		bool CTexture::getEditable() const
		{
			return StoreTextureData;
		}

		std::string CTexture::getFileName() const
		{
			return FileName;
		}

		void CTexture::setRenderTarget(const bool pFlag)
		{
			RenderTarget = pFlag;
		}
		
		bool CTexture::isRenderTarget() const
		{
			return RenderTarget;
		}
	};
};

