// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "gfx/Noise.h"

namespace lew
{
	namespace gfx
	{
		namespace noise
		{
			float interpolation(const float x, const float y, const float d)
			{
				//return x + d * (y - d);
				return x * (1 - d) + y * d;
			}

			float noise1D(int x)	 
			{
			    x = (x<<13) ^ x;
			    return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
			}

			float snoise1D(const int x)
			{
			    return noise1D(x)/2 + noise1D(x - 1)/4 + noise1D(x + 1)/4;
			}

			float inoise1D(const float x)
			{
				int rounded = (int)x;
				float fraction = x - rounded;

				float v1 = snoise1D(rounded);
				float v2 = snoise1D(rounded + 1);

				return interpolation(v1, v2, fraction);
			}

			float pnoise1D(const float x, const float p, const int o)
			{
				float result = 0.0f;
				int n = o - 1;
				
				for(int i = 0; i != n; ++i)
				{
					float frequency = pow(2, i);
					float amplitude = pow(p, i);

					result = result + inoise1D(x * frequency) * amplitude;
				}
				
				//convert colors to 0 to 2 range
				result += 1.0f;
						
				//get the colors in 0 to 1 range
				result *= 0.5f;
				
				if(result > 0.98f)
					result = 0.98f;
					
				if(result < 0.0f)
					result = 0.0f;

				return result;
			}

			float noise2D(const float x, const float y, const float m, const float t)
			{
				int n = (int)x + (int)y * 57;
				n = (n<<13) ^ n;
			
				return (1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589*int(m)) & 0x7fffffff) / 1073741824.0*t);    
			}

			float snoise2D(const float x, const float y, const float m, const float t)
			{
				float corners = (noise2D(x-1, y-1, m, t) + noise2D(x+1, y-1, m, t) + 
					noise2D(x-1, y+1, m, t) + noise2D(x+1, y+1, m, t))/16;
				
				float sides   = (noise2D(x-1, y, m, t) + noise2D(x+1, y, m, t) + 
					noise2D(x, y-1, m, t) + noise2D(x, y+1, m, t) )/8;
					
				float center  =  noise2D(x, y, m, t)/4;
				
				return corners + sides + center;
			}

			float inoise2D(const float x, const float y, const float m, const float t)
			{
				int roundedX = (int)x;
				int roundedY = (int)y;
				
				float fractionX = x - roundedX;
				float fractionY = y - roundedY;

				float v1 = noise2D(roundedX, roundedY, m, t);
				float v2 = noise2D(roundedX + 1, roundedY, m, t);
				float v3 = noise2D(roundedX, roundedY + 1, m, t);
				float v4 = noise2D(roundedX + 1, roundedY + 1, m, t);

				float i1 = interpolation(v1 , v2 , fractionX);
				float i2 = interpolation(v3 , v4 , fractionX);

				return interpolation(i1 , i2 , fractionY);
			}

			float pnoise2D(const float x, const float y, const float p, 
			const int o, const float m, const float t)
			{
				float result = 0.0f;
				int n = o - 1;
				
				for(int i = 0; i != n; ++i)
				{
					float frequency = pow(2, i);
					float amplitude = pow(p, i);

					result = result + inoise2D(x * frequency, y * frequency, m, t) * amplitude;
				}

				//convert colors to 0 to 2 range
				result += 1.0f;
						
				//get the colors in 0 to 1 range
				result *= 0.5f;
				
				if(result > 0.98f)
					result = 0.98f;
					
				if(result < 0.0f)
					result = 0.0f;

				return result;
			}
			
			void createNoiseTexture(ITexture* pTex, const int pOct, 
			const float pPersistence, const float pZoom, const Vector pCoords,   
			const float pMod, const float pTurbulence, const bool pInvert)
			{
				if(pTex->getEditable())
				{
					float xoff = pCoords.x;

					for(int i = 0; i != pTex->getDimension().x; ++i)
					{
						xoff += pZoom;
						float yoff = pCoords.y;

						for(int j = 0; j != pTex->getDimension().y; ++j)
						{
							yoff += pZoom;
							Color col;
							
							float val = pnoise2D(xoff, yoff, pPersistence, pOct, 
							pMod, pTurbulence);
							
							col.r = val;
							col.g = val;
							col.b = val;
							col.a = 1;
							
							if(pInvert)
							{
								//0.0 - 1.0 gives artifacts, so we use 0.98
								col.invert();
								col.clamp(0.0f, 0.98f);
								col.a = 1.0f;
							}
							
							pTex->setPixel(Vector(i, j), col);
						}
					}								
				}
			}
			
			void createColorTexture(ITexture* pHeightmap, ITexture* pColormap,
			ITexture* pNoisemap, const float pDesertExpansion, 
			const float pArcticExpansion, const int pClimateIntensity,
			const float pNoiseIntensity, Color pDeepSea, Color pSea, 
			Color pOpenWater, Color pShore, Color pSand, Color pGrass, 
			Color pHills, Color pRocks,	Color pMountains, Color pSnow, 
			Color pArctic, Color pDesert)
			{
				Color Gradient[101];
				int x = 0;

				Gradient[0] = pDeepSea;
				Gradient[15] = pSea;
				Gradient[40] = pOpenWater;
				Gradient[45] = pShore;
				Gradient[51] = pSand;
				Gradient[60] = pGrass;
				Gradient[79] = pHills;
				Gradient[84] = pRocks;
				Gradient[98] = pMountains;
				Gradient[100] = pSnow;
				

				x = 1; 
				for(int i = 1; i != 15; ++i)
				{
					Gradient[i] = Gradient[0].blend(Gradient[15], (1.0/15)*x);
					++x;
				}

				x = 1; 
				for(int i = 16; i != 40; ++i)
				{
					Gradient[i] = Gradient[15].blend(Gradient[40], (1.0/25)*x);
					++x;
				}

				x = 1; 
				for(int i = 41; i != 45; ++i)
				{
					Gradient[i] = Gradient[40].blend(Gradient[45], (1.0/5)*x);
					++x;
				}
				
				x = 1; 
				for(int i = 46; i != 51; ++i)
				{
					Gradient[i] = Gradient[45].blend(Gradient[51], (1.0/6)*x);
					++x;
				}

				x = 1; 
				for(int i = 52; i != 60; ++i)
				{
					Gradient[i] = Gradient[51].blend(Gradient[60], (1.0/9)*x);
					++x;
				}

				x = 1; 
				for(int i = 61; i != 79; ++i)
				{
					Gradient[i] = Gradient[60].blend(Gradient[79], (1.0/19)*x);
					++x;
				}

				x = 1; 
				for(int i = 80; i != 84; ++i)
				{
					Gradient[i] = Gradient[79].blend(Gradient[84], (1.0/5)*x);
					++x;
				}

				x = 1; 
				for(int i = 85; i != 98; ++i)
				{
					Gradient[i] = Gradient[84].blend(Gradient[98], (1.0/13)*x);
					++x;
				}

				x = 1; 
				for(int i = 99; i != 100; ++i)
				{
					Gradient[i] = Gradient[98].blend(Gradient[100], (1.0/3)*x);
					++x;
				}
				
				for(int i = 0; i != 101; ++i)
				{
					Gradient[i].a = 1.0f;	
					Gradient[i].clamp(0.0f, 1.0f);
				}
				
				if(pHeightmap->getDimension() == pColormap->getDimension() &&
					pHeightmap->getDimension() == pNoisemap->getDimension() && 
					pHeightmap->getEditable() && pColormap->getEditable())
				{
					for(int i = 0; i != pHeightmap->getDimension().x; ++i)
					{
						for(int j = 0; j != pHeightmap->getDimension().y; ++j)
						{
							Vector pos = Vector(i, j);
							Color col = pHeightmap->getPixel(pos);
							Color ncol = pNoisemap->getPixel(pos);

							col = Gradient[int(col.r*100.0f)]; 
							col.a = 1.0f;
							
							
							if(col.b < col.r && col.b < col.g)
							{
								const float nDim = pNoisemap->getDimension().y;
								float desert_factor = col.r - (fabs((nDim/2.0f) - j)/pDesertExpansion);
								float arctic_factor = 0.0f;
								
								
								//below equator
								if(j <= (nDim/2.0f))
								{					
									arctic_factor = (col.r + (pArcticExpansion/1000.0f)) - (j/pArcticExpansion);	
								}
								else
								{					
									float v = abs(j - int(nDim));
									arctic_factor = (col.r + (pArcticExpansion/1000.0f)) - (v/pArcticExpansion);
								}
								
								
								if(desert_factor < 0.0f) 
									desert_factor = 0.0f;
									
								if(arctic_factor < 0.0f)
									arctic_factor = 0.0f;
									

								for(int x = 0; x != pClimateIntensity; ++x)
								{
									if(desert_factor > 0.0f)
									{
										col = col.blend(pDesert, desert_factor);
										col.a = 1.0f;

									}
									if(arctic_factor > 0.0f)
									{
										col = col.blend(pArctic, arctic_factor);
										col.a = 1.0f;		
									}
								}

								
								//scather noise for non-uniform look								
								if(pNoiseIntensity > 0)
								{
									col -= ncol.r/(pNoiseIntensity);
									col.clamp(0.0f, 0.98f);
									col.a = 1.0f;	
								}						
							}
							
							pColormap->setPixel(pos, col);
						}
					}
				}
			}			
		};
	};
};


