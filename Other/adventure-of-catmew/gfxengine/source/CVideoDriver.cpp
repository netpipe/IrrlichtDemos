// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "gfx/Error.h"
#include "gfx/IRoot.h"
#include "CVideoDriver.h"
#include "CTexture.h"
#include "CShader.h"
#include "CSceneManager.h"
#include "CFont.h"

namespace lew
{
	namespace gfx
	{
		CVideoDriver::CVideoDriver(IRoot* m_pRoot) : IVideoDriver()
		{
			pRoot = m_pRoot;
			LastFPSUpdate	= -1;
			LastFPS			= -1;
			FPS				= 0;
			ShaderSupport	= false;
			MultiTexturingSupport = true;
			Background		= Color(0.0f, 0.0f, 0.0f, 0.0f);
			Ambient 		= Color(0.0f, 0.0f, 0.0f, 0.0f);
			MaxLayers		= 1000;
			MaxTextureLayers= 0;
			VertexCount		= 0;
			VertexCountTemp	= 0;
			DriverName		= "";

			//get the current video driver name from SDL
			char* driver_name = new char[100];
			SDL_VideoDriverName(driver_name, 99);
			DriverName += driver_name;
			delete [] driver_name;


			//used for extension loading
			glewInit();

			//check if glsl support is available
			if(glewIsSupported("GL_VERSION_2_0") &&
			glewIsSupported("GL_ARB_shading_language_100") &&
			glewIsSupported("GL_ARB_fragment_shader") &&
			glewIsSupported("GL_ARB_vertex_shader") &&
			glewIsSupported("GL_ARB_shader_objects"))
			{
				ShaderSupport = true;
			}

			//check how many texture layers are supported
			glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &MaxTextureLayers);

			//no multitexturing support: only allow 1 texture
			if(!glewIsSupported("GL_ARB_multitexture") ||
			!glewIsSupported("GL_EXT_texture_env_combine"))
			{
				MaxTextureLayers = 1;
				MultiTexturingSupport = false;
			}

			//only allow as much layers as defined in IMaterial
			if(MaxTextureLayers > MAX_TEXTURE_LAYERS)
				MaxTextureLayers = MAX_TEXTURE_LAYERS;

			int MaxRes[2] = {0, 0};
			int TexRes = 0;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TexRes);
			glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &MaxRes[0]);

			MaxViewportDimension = Vector(MaxRes[0], MaxRes[1]);
			MaxTextureDimension = Vector(TexRes, TexRes);

			glEnable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);

			glDepthFunc(GL_LEQUAL);
			glShadeModel(GL_FLAT);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glCullFace(GL_FRONT);

			TTF_Init();
			resetViewPort();

			DefaultMaterial = new IMaterial();
			LastMaterial = DefaultMaterial;

			//grab because it's the first material that's used, and remove()
			//will be called in enableMaterial() before it was grabbed
			DefaultMaterial->grab();
			DefaultMaterial->lock();
		}

		CVideoDriver::~CVideoDriver()
		{
			removeAll();
			TTF_Quit();

			DefaultMaterial->unlock();
			DefaultMaterial->remove();
		}

		ITexture* CVideoDriver::createTexture(const std::string pFileName,
		const bool pStoreTextureData)
		{
			if(!TextureMap[pFileName])
			{
				CTexture* tex = new CTexture(pRoot, pStoreTextureData);

				tex->create(pFileName);

				TextureList.push_back(tex);
				TextureMap[pFileName] = tex;

				return tex;
			}
			else
			{
				TextureMap[pFileName]->grab();
				return TextureMap[pFileName];
			}

			return 0;
		}

		ITexture* CVideoDriver::createEmptyTexture(const Vector pSize,
		const bool pStoreTextureData)
		{
			CTexture* tex = new CTexture(pRoot, pStoreTextureData);

			tex->create(pSize);
			TextureList.push_back(tex);

			return tex;
		}

		IFont* CVideoDriver::createFont(const std::string pFileName,
		const int pSize, const bool pBold, const bool pItalic,
		const bool pEditable)
		{
			//check if font is already loaded
			IFont* Result = 0;
			for(int i = 0; i != getFontCount(); ++i)
			{
				if(FontList[i]->getSize() == pSize &&
					FontList[i]->getBold() == pBold &&
					FontList[i]->getItalic() == pItalic)
				{
					Result = FontList[i];
					break;
				}
			}

			//it isn't, so create it
			if(!Result)
			{
				IFont* pFont = new CFont(pRoot, pFileName, pSize,
					pBold, pItalic, pEditable);

				FontList.push_back(pFont);
				return pFont;
			}
			//already available, return a pointer
			else
			{
				Result->grab();
				return Result;
			}

			return 0;
		}

		IShader* CVideoDriver::createShader(const std::string pVertexShaderSource,
		const std::string pFragmentShaderSource)
		{
			//check if glsl shaders are supported
			if(ShaderSupport)
			{
				//check if shader is already loaded
				IShader* Result = 0;
				for(int i = 0; i != getShaderCount(); ++i)
				{
					if(ShaderList[i]->getVertexShaderSource() ==
						pVertexShaderSource &&
						ShaderList[i]->getFragmentShaderSource() ==
						pFragmentShaderSource )
					{
						Result = ShaderList[i];
						break;
					}
				}

				//it isn't, so create it
				if(!Result)
				{
					IShader* pShader = new CShader(pRoot,
						pVertexShaderSource, pFragmentShaderSource);

					ShaderList.push_back(pShader);
					return pShader;
				}
				//already available, return a pointer
				else
				{
					Result->grab();
					return Result;
				}
			}
			//shaders not supported
			else
			{
				throw(err::Shader("glsl not supported"));
			}

			return 0;
		}

		void CVideoDriver::setMaxLayerCount(const int pLayers)
		{
			if(pLayers != MaxLayers)
			{
				MaxLayers = pLayers;

				//re-initialize the viewport because we change depth-information
				setViewPort(getViewPortMin(), getViewPortMax());
			}
		}

		void CVideoDriver::setBackgroundColor(const Color pBackground)
		{
			Background = pBackground;
		}

		void CVideoDriver::setAmbientColor(const Color pColor)
		{
			Ambient = pColor;
		}

		void CVideoDriver::setViewPort(const Vector pMin, const Vector pMax)
		{
			if(pMin.difference(pMax) <= MaxViewportDimension)
			{
				ViewPortMin = pMin;
				ViewPortMax = pMax;

				setMatrixMode(EMM_PROJECTION);
				resetActiveMatrix();

				glViewport(int(pMin.x), int(pMin.y), int(pMax.x), int(pMax.y));

				glOrtho(int(pMin.x), int(pMax.x), int(pMin.y), int(pMax.y), 0.1f,
				getMaxLayerCount() + 2);

				setMatrixMode(EMM_MODELVIEW);
				resetActiveMatrix();
			}
		}

		void CVideoDriver::setDepthTesting(const bool pFlag)
		{
			if(pFlag)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		void CVideoDriver::setMatrixMode(const MatrixMode pMode)
		{
			switch (pMode)
			{
				case EMM_PROJECTION:
				{
					glMatrixMode(GL_PROJECTION);
					break;
				}

				case EMM_MODELVIEW:
				{
					glMatrixMode(GL_MODELVIEW);
					break;
				}

				case EMM_TEXTURE:
				{
					glMatrixMode(GL_TEXTURE);
					break;
				}
			}

			pMatrixMode = pMode;
		}

		void CVideoDriver::setMatrix(const MatrixType pType,
		const Vector pValue, const float pDepth)
		{
			switch(pType)
			{
				case EMT_TRANSFORMATION:
				{
					glTranslatef(pValue.x, pValue.y, pDepth);
					break;
				}
				case EMT_ROTATION:
				{
					glRotatef(pDepth, 0, 0, 1);
					glRotatef(pValue.y, 0, 1, 0);
					glRotatef(pValue.x, 1, 0, 0);
					break;
				}
				case EMT_SCALE:
				{
					glScalef(pValue.x, pValue.y, pDepth);
					break;
				}
			}
		}

		void CVideoDriver::setMatrix(const MatrixType pType,
		const float pX, const float pY, const float pZ)
		{
			setMatrix(pType, Vector(pX, pY), pZ);
		}

		Vector CVideoDriver::getViewPortMin() const
		{
			return ViewPortMin;
		}

		Vector CVideoDriver::getViewPortMax() const
		{
			return ViewPortMax;
		}

		Vector CVideoDriver::getMaxViewportDimension() const
		{
			return MaxViewportDimension;
		}

		Vector CVideoDriver::getMaxTextureDimension() const
		{
			return MaxTextureDimension;
		}

		Color CVideoDriver::getBackgroundColor() const
		{
			return Background;
		}

		Color CVideoDriver::getAmbientColor() const
		{
			return Ambient;
		}

		Color CVideoDriver::getPixelColor(Vector pPixel) const
		{
			//stay within the window (starts at 0,0, so the max. coordinate
			//is Resolution-1
			pPixel.clamp(Vector(), pRoot->getDimension()-1);

			float data[4];
			glReadBuffer(GL_FRONT);

			glReadPixels(int(pPixel.x), int(pPixel.y), 1, 1, GL_RGBA,
				GL_FLOAT, &data);

			return Color(data[0], data[1], data[2], data[3]);
		}

		MatrixMode CVideoDriver::getMatrixMode() const
		{
			return pMatrixMode;
		}

		std::string CVideoDriver::getDriverName() const
		{
			return DriverName;
		}

		bool CVideoDriver::getShaderSupport() const
		{
			return ShaderSupport;
		}

		int CVideoDriver::getMaxTextureLayers() const
		{
			return MaxTextureLayers;
		}

		int CVideoDriver::getFrameCount() const
		{
			return LastFPS;
		}

		int CVideoDriver::getTextureCount() const
		{
			return TextureList.size();
		}

		int CVideoDriver::getShaderCount() const
		{
			return ShaderList.size();
		}

		int CVideoDriver::getFontCount() const
		{
			return FontList.size();
		}

		int CVideoDriver::getMaxLayerCount() const
		{
			return MaxLayers;
		}

		int CVideoDriver::getTriangleCount() const
		{
			return int(VertexCount/3);
		}

		int CVideoDriver::getVertexCount() const
		{
			return VertexCount;
		}

		void CVideoDriver::takeScreenshot(const std::string pFileName)
		{
			ITexture* sShot = createEmptyTexture(pRoot->getDimension(), true);

			for(int i = 0; i != int(pRoot->getDimension().x); ++i)
			{
				for(int j = 0; j != int(pRoot->getDimension().y); ++j)
				{
					const Vector Pixel = Vector(i, j);

					//we need to flip the Y coordinate around
					//because opengl uses a different coordinate system
					// (0,0 is bottom-left)
					Vector ScreenPixel;
					ScreenPixel.x = Pixel.x;
					ScreenPixel.y = (pRoot->getDimension().y-1) - Pixel.y;

					sShot->setPixel(Pixel, getPixelColor(ScreenPixel));
				}
			}

			sShot->save(pFileName);
			sShot->remove();
		}

		void CVideoDriver::renderToTexture(ITexture* pTexture,
		const TextureMode pMode)
		{
			glBindTexture(GL_TEXTURE_2D, ((CTexture*)pTexture)->
				getInternalHandle(pMode));

			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0,
				int(pTexture->getDimension().x),
				int(pTexture->getDimension().y), 0);

			((CTexture*)pTexture)->setRenderTarget(true);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void CVideoDriver::renderBox(const Box pBox, IMaterial*
		pMaterial)
		{
			enableMaterial(pMaterial);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, &pBox.point[0]);

			for(int i = 0; i != MaxTextureLayers; ++i)
			{
				if(pMaterial->getTexture(i))
				{
					enableTextureUnit(i);

					//render-targets are flipped upside down because of
					//glCopyTexImage2D, so we need to flip them again
					bool flipped = false;
					if(((CTexture*)pMaterial->getTexture(i))->isRenderTarget())
					{
						flipped = true;
					}

					Vector TCoords[4];
					//no render-target, don't flip texture coordinates
					if(!flipped)
					{
						TCoords[0].x = 0; TCoords[0].y = 0;
						TCoords[1].x = 1; TCoords[1].y = 0;
						TCoords[2].x = 1; TCoords[2].y = 1;
						TCoords[3].x = 0; TCoords[3].y = 1;
					}
					else
					{
						TCoords[0].x = 0; TCoords[0].y = 1;
						TCoords[1].x = 1; TCoords[1].y = 1;
						TCoords[2].x = 1; TCoords[2].y = 0;
						TCoords[3].x = 0; TCoords[3].y = 0;
					}

					glTexCoordPointer(2, GL_FLOAT, 0, &TCoords[0]);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				}
			}

			glDrawArrays(GL_QUADS, 0, 4);

			for(int i = 0; i != MaxTextureLayers; ++i)
			{
				if(pMaterial->getTexture(i))
				{
					disableTextureUnit(i);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			VertexCountTemp += 4;
		}

		void CVideoDriver::renderLine(const Vector pStart, const Vector pEnd,
		const Color pColor, const float pWidth)
		{
			((CSceneManager*)pRoot->getSceneManager())->
			applyWorldCoordinates();

			//allow line thickness
			glLineWidth(pWidth);

			//enable transparency
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(pColor.r, pColor.g, pColor.b, pColor.a);

			glBegin(GL_LINES);
				glVertex2d(pStart.x, pStart.y);
				glVertex2d(pEnd.x, pEnd.y);
			glEnd();

			VertexCountTemp += 2;
		}

		void CVideoDriver::renderLine(const Vector pStart, const Vector pEnd,
		IMaterial* pMaterial)
		{
			((CSceneManager*)pRoot->getSceneManager())->
			applyWorldCoordinates();

			enableMaterial(pMaterial);

			glBegin(GL_LINES);
				glVertex2d(pStart.x, pStart.y);
				glVertex2d(pEnd.x, pEnd.y);
			glEnd();

			for(int i = 0; i != MaxTextureLayers; ++i)
			{
				disableTextureUnit(i);
			}

			VertexCountTemp += 2;
		}

		void CVideoDriver::beginFrame()
		{
			//update the fps counter
			if((pRoot->getTime() - LastFPSUpdate) > 1000)
			{
				LastFPSUpdate = pRoot->getTime();
				LastFPS = FPS;
				FPS = 0;

				VertexCount = VertexCountTemp;
				VertexCountTemp	= 0;
			}

			clearScreen();
		}

		void CVideoDriver::endFrame()
		{
			SDL_GL_SwapBuffers();
			FPS += 1;
		}

		void CVideoDriver::saveMatrix()
		{
			glPushMatrix();
		}

		void CVideoDriver::restoreMatrix()
		{
			glPopMatrix();
		}

		void CVideoDriver::resetActiveMatrix()
		{
			glLoadIdentity();
		}

		void CVideoDriver::clearScreen()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(Background.r,	Background.g, Background.b, Background.a);
			resetActiveMatrix();
		}

		void CVideoDriver::resetViewPort()
		{
			ViewPortMin = Vector(0, 0);
			ViewPortMax = pRoot->getDimension();

			setViewPort(ViewPortMin, ViewPortMax);
		}

		void CVideoDriver::removeTexture(ITexture* pTexture)
		{
			bool _Success = false;

			std::vector<ITexture*>::iterator Iter;

			for(Iter = TextureList.begin(); Iter != TextureList.end(); ++Iter)
			{
				if((*Iter) == pTexture)
				{
					_Success = true;
					break;
				}
			}

			if(_Success)
			{
				TextureMap.erase((*Iter)->getFileName());
				TextureList.erase(Iter);
			}
		}

		void CVideoDriver::removeShader(IShader* pShader)
		{
			bool _Success = false;

			std::vector<IShader*>::iterator Iter;

			for(Iter = ShaderList.begin(); Iter != ShaderList.end(); ++Iter)
			{
				if((*Iter) == pShader)
				{
					_Success = true;
					break;
				}
			}

			if(_Success)
				ShaderList.erase(Iter);
		}

		void CVideoDriver::removeFont(IFont* pFont)
		{
			bool _Success = false;

			std::vector<IFont*>::iterator Iter;

			for(Iter = FontList.begin(); Iter != FontList.end(); ++Iter)
			{
				if((*Iter) == pFont)
				{
					_Success = true;
					break;
				}
			}

			if(_Success)
				FontList.erase(Iter);
		}

		void CVideoDriver::removeAllTextures()
		{
			std::vector<ITexture*> Queue = TextureList;

			for(unsigned int i = 0; i != Queue.size(); ++i)
				Queue[i]->remove();
		}

		void CVideoDriver::removeAllShaders()
		{
			std::vector<IShader*> Queue = ShaderList;

			for(unsigned int i = 0; i != Queue.size(); ++i)
				Queue[i]->remove();
		}

		void CVideoDriver::removeAllFonts()
		{
			std::vector<IFont*> Queue = FontList;

			for(unsigned int i = 0; i != Queue.size(); ++i)
				Queue[i]->remove();
		}

		void CVideoDriver::removeAll()
		{
			removeAllTextures();
			removeAllShaders();
			removeAllFonts();
		}

		void CVideoDriver::enableTextureUnit(const int pTextureUnit)
		{
			if(pTextureUnit < MaxTextureLayers && MultiTexturingSupport)
			{
				glActiveTextureARB(GL_TEXTURE0_ARB+pTextureUnit);
				glClientActiveTextureARB(GL_TEXTURE0_ARB+pTextureUnit);
				glEnable(GL_TEXTURE_2D);
			}
		}

		void CVideoDriver::disableTextureUnit(const int pTextureUnit)
		{
			if(pTextureUnit < MaxTextureLayers && MultiTexturingSupport)
			{
				glActiveTextureARB(GL_TEXTURE0_ARB+pTextureUnit);
				glClientActiveTextureARB(GL_TEXTURE0_ARB+pTextureUnit);
				glDisable(GL_TEXTURE_2D);
			}
		}

		void CVideoDriver::enableTexture(GLuint pTexture)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, pTexture);
		}

		void CVideoDriver::enableShader(GLuint pShader)
		{
			if(ShaderSupport)
			{
				glUseProgram(pShader);
			}
		}

		void CVideoDriver::enableMaterial(IMaterial* pMaterial)
		{
			IMaterial* mat = pMaterial;

			if(mat->getWireframe() != LastMaterial->getWireframe())
			{
				if(mat->getWireframe())
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glLineWidth(mat->getWireWidth());
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}

			if(mat->getBackfaceCulling() != LastMaterial->getBackfaceCulling())
			{
				if(mat->getBackfaceCulling())
				{
					glEnable(GL_CULL_FACE);
				}
				else
				{
					glDisable(GL_CULL_FACE);
				}
			}

			Color color = mat->getColor();
			if(mat->getUseAmbientColor())
			{
				color += Ambient;
			}

			glColor4f(color.r, color.g, color.b, color.a);


			if(ShaderSupport)
			{
				if(mat->getShader() != LastMaterial->getShader())
				{
					if(mat->getShader())
					{
						glUseProgram(((CShader*)mat->getShader())->
						getInternalHandle());
					}
					else
					{
						glUseProgram(0);
					}
				}
			}

			if(mat->getBlendMode() != LastMaterial->getBlendMode())
			{
				switch(mat->getBlendMode())
				{
					case EBM_NONE:
						glDisable(GL_BLEND);
						break;

					case EBM_ALPHA:
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						break;

					case EBM_ADD:
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE, GL_ONE);
						break;

					case EBM_SUBTRACT:
						glEnable(GL_BLEND);
						glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
						break;

					case EBM_MODULATE:
						glEnable(GL_BLEND);
						glBlendFunc(GL_DST_COLOR, GL_ZERO);
						break;
				}
			}

			for(int i = 0; i != MaxTextureLayers; ++i)
			{
				if(mat->getTexture(i))
				{
					enableTextureUnit(i);
					enableTexture(((CTexture*)mat->getTexture(i))->
					getInternalHandle(mat->getTextureFilter(i)));

					switch(mat->getTextureBlendMode(i))
					{
						case EBM_NONE:
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SRC_ALPHA);
							break;

						case EBM_ALPHA:
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);

							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_DECAL);
							break;

						case EBM_ADD:
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SRC_ALPHA);
							break;

						case EBM_SUBTRACT:
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SRC_ALPHA);
							break;

						case EBM_MODULATE:
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
							glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SRC_ALPHA);
							break;
					}

						Vector TOffset = mat->getTextureOffset(i);
						Vector TScale = mat->getTextureScale(i);

						//apply texture coordinate-modifications
						setMatrixMode(EMM_TEXTURE);
						resetActiveMatrix();

						setMatrix(EMT_TRANSFORMATION, TOffset, 1.0f);
						setMatrix(EMT_SCALE, TScale, 1.0f);

						setMatrixMode(EMM_MODELVIEW);
				}
			}

			LastMaterial->remove();
			LastMaterial = mat;
			LastMaterial->grab();
		}
	};
};

