// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CVideoDriverHeader_
#define _CVideoDriverHeader_

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_main.h>
#include <vector>
#include <map>

#include "gfx/IVideoDriver.h"

namespace lew
{
	namespace gfx
	{
		class IRoot;
		class CVideoDriver : public IVideoDriver
		{
			public:
				CVideoDriver(IRoot* m_pRoot);
				virtual ~CVideoDriver();

				ITexture* createTexture(const std::string pFileName,
					const bool pStoreTextureData = false);
				
				ITexture* createEmptyTexture(const Vector psize,
					const bool pStoreTextureData = false);
				
				IShader* createShader(const std::string pVertexShaderSource,
					const std::string pFragmentShaderSource);
					
				IFont* createFont(const std::string pFileName, 
					const int pSize = 10, const bool pBold = false, 
					const bool pItalic = false, const bool pEditable = false);
					
				void setAmbientColor(const Color pColor);
				void setBackgroundColor(const Color pBackground);
				void setViewPort(const Vector pMin, const Vector pMax);
				void setMaxLayerCount(const int pLayers);
				void setMatrixMode(const MatrixMode pMode);
				void setDepthTesting(const bool pFlag);
				
				void setMatrix(const MatrixType pType,
					const Vector pValue, const float pDepth = 1.0f);
				
				void setMatrix(const MatrixType pType,
					const float pX = 1.0f, const float pY = 1.0f, 
					const float pZ = 1.0f);

				Vector getViewPortMin() const;
				Vector getViewPortMax() const;
				Vector getMaxViewportDimension() const;
				Vector getMaxTextureDimension() const;
				Color getBackgroundColor() const;
				Color getAmbientColor() const;
				Color getPixelColor(Vector pPixel) const;
				MatrixMode getMatrixMode() const;
				std::string getDriverName() const;
				bool getShaderSupport() const;
				int getMaxTextureLayers() const;
				int getFrameCount() const;
				int getMaxLayerCount() const;
				int getTextureCount() const;
				int getShaderCount() const;
				int getFontCount() const;
				int getTriangleCount() const;
				int getVertexCount() const;
				
				void takeScreenshot(const std::string pFileName);

				void renderToTexture(ITexture* pTexture, 
					const TextureMode pMode);

				void renderBox(const Box pBox, IMaterial* pMaterial);
				
				void renderLine(const Vector pStart, const Vector pEnd, 
					const Color pColor = Color(), const float pWidth = 1.0f);

				void renderLine(const Vector pStart, const Vector pEnd, 
					IMaterial* pMaterial);

				void beginFrame();
				void endFrame();
				void saveMatrix();
				void restoreMatrix();
				void resetActiveMatrix();
				void clearScreen();
				void resetViewPort();
				void removeAllTextures();
				void removeAllShaders();
				void removeAllFonts();
				void removeAll();
				void enableTextureUnit(const int pTextureUnit);
				void disableTextureUnit(const int pTextureUnit);
				void enableTexture(GLuint pTexture);
				void enableShader(GLuint pShader);
				void enableMaterial(IMaterial* pMaterial);
				void removeTexture(ITexture* pTexture);
				void removeShader(IShader* pShader);
				void removeFont(IFont* pFont);


			private:
				IRoot* pRoot;
				
				Color Background;
				Color Ambient;
				MatrixMode pMatrixMode;
				
				bool ShaderSupport;
				bool MultiTexturingSupport;
				int VertexCount;
				int VertexCountTemp;
				int LastFPSUpdate;
				int FPS;
				int LastFPS;
				int MaxLayers;
				int MaxTextureLayers;
				
				Vector ViewPortMin;
				Vector ViewPortMax;
				
				Vector MaxViewportDimension;
				Vector MaxTextureDimension;
								
				std::vector<ITexture*> TextureList;
				std::map<std::string, ITexture*> TextureMap;
				
				std::vector<IShader*> ShaderList;
				std::vector<IFont*> FontList;
				
				std::string DriverName;
				
				IMaterial* LastMaterial;
				IMaterial* DefaultMaterial;
		};
	};
};

#endif
