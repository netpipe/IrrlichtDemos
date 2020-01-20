// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IVideoDriverHeader_
#define _IVideoDriverHeader_

#include "gfx/Reference.h"
#include "gfx/Vector.h"
#include "gfx/Color.h"
#include "gfx/Box.h"
#include "gfx/ITexture.h"
#include "gfx/IMaterial.h"

namespace lew
{
	namespace gfx
	{
		enum MatrixType
		{
			EMT_TRANSFORMATION,
			EMT_ROTATION,
			EMT_SCALE
		};
		
		enum MatrixMode
		{
			EMM_PROJECTION,
			EMM_TEXTURE,
			EMM_MODELVIEW
		};
		
		class IShader;
		class IFont;
		class IVideoDriver : public Reference
		{
			public:
				IVideoDriver() : Reference() {} 
				virtual ~IVideoDriver() {}

				virtual ITexture* createTexture(const std::string pFileName,
					const bool pStoreTextureData = false) = 0;
				
				virtual ITexture* createEmptyTexture(const Vector psize,
					const bool pStoreTextureData = false) = 0;
				
				virtual IShader* createShader(const std::string pVertexShaderSource,
					const std::string pFragmentShaderSource) = 0;
					
				virtual IFont* createFont(const std::string pFileName, 
					const int pSize = 10, const bool pBold = false, 
					const bool pItalic = false, const bool pEditable = false) = 0;
					
				virtual void setAmbientColor(const Color pColor) = 0;
				virtual void setBackgroundColor(const Color pBackground) = 0;
				virtual void setViewPort(const Vector pMin, const Vector pMax) = 0;
				virtual void setMaxLayerCount(const int pLayers) = 0;
				virtual void setMatrixMode(const MatrixMode pMode) = 0;
				virtual void setDepthTesting(const bool pFlag) = 0;
				
				virtual void setMatrix(const MatrixType pType,
					const Vector pValue, const float pDepth = 1.0f) = 0;
				
				virtual void setMatrix(const MatrixType pType,
					const float pX = 1.0f, const float pY = 1.0f, 
					const float pZ = 1.0f) = 0;

				virtual Vector getViewPortMin() const = 0;
				virtual Vector getViewPortMax() const = 0;
				virtual Vector getMaxViewportDimension() const = 0;
				virtual Vector getMaxTextureDimension() const = 0;
				virtual Color getBackgroundColor() const = 0;
				virtual Color getAmbientColor() const = 0;
				virtual Color getPixelColor(Vector pPixel) const = 0;
				virtual MatrixMode getMatrixMode() const = 0;
				virtual std::string getDriverName() const = 0;
				virtual bool getShaderSupport() const = 0;
				virtual int getMaxTextureLayers() const = 0;
				virtual int getFrameCount() const = 0;
				virtual int getMaxLayerCount() const = 0;
				virtual int getTextureCount() const = 0;
				virtual int getShaderCount() const = 0;
				virtual int getFontCount() const = 0;
				virtual int getTriangleCount() const = 0;
				virtual int getVertexCount() const = 0;
				
				virtual void takeScreenshot(const std::string pFileName) = 0;

				virtual void renderToTexture(ITexture* pTexture, 
					const TextureMode pMode) = 0;

				virtual void renderBox(const Box pBox, 
					IMaterial* pMaterial) = 0;
				
				virtual void renderLine(const Vector pStart, 
					const Vector pEnd, const Color pColor = 
					Color(), const float pWidth = 1.0f) = 0;

				virtual void renderLine(const Vector pStart, 
					const Vector pEnd, IMaterial* pMaterial) = 0;

				virtual void beginFrame() = 0;
				virtual void endFrame() = 0;
				virtual void saveMatrix() = 0;
				virtual void restoreMatrix() = 0;
				virtual void resetActiveMatrix() = 0;
				virtual void clearScreen() = 0;
				virtual void resetViewPort() = 0;
				virtual void removeAllTextures() = 0;
				virtual void removeAllShaders() = 0;
				virtual void removeAllFonts() = 0;
				virtual void removeAll() = 0;
		};
	};
};

#endif
