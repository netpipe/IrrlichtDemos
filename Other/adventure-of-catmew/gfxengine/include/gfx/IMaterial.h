// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IMaterialGfxHeader_
#define _IMaterialGfxHeader_

#include "gfx/Reference.h"
#include "gfx/Vector.h"
#include "gfx/Color.h"
#include "gfx/ITexture.h"
#include "gfx/IShader.h"

namespace lew
{
	namespace gfx
	{
		#define MAX_TEXTURE_LAYERS 2

		enum BlendMode
		{
			EBM_NONE,
			EBM_ALPHA,
			EBM_ADD,
			EBM_SUBTRACT,
			EBM_MODULATE
		};

		class IMaterial : public Reference
		{
			public:
				IMaterial() : Reference()
				{
					for(int i = 0; i != MAX_TEXTURE_LAYERS; ++i)
					{
						AnimationSpeed[i]		= 0;
						AnimationFrame[i]		= 0;
						AnimationLoopCount[i]	= 0;
						AnimationLoopIndex[i]	= 0;
						AnimationLoopInterval[i]= 0;
						LastAnimationLoopTime[i]= 0;
						LastAnimationFrame[i]	= 0;

						TextureScale[i] = Vector(1.0f, 1.0f);
						TextureOffset[i] = Vector(0.0f, 0.0f);

						TextureAnimation[i] = false;
						TextureFilter[i] = ETF_NEAR;
						TextureBlendMode[i] = EBM_NONE;
						pTexture[i] = 0;
					}

					pShader = 0;
					WireWidth = 1.0f;
					BlendSpeed = 0;
					BlendTime = 1;
					BlendFactor = 0.0f;

					BlendOut = false;
					BlendIn = false;
					BackfaceCulling = true;
					Wireframe = false;
					UseAmbientColor = true;

					pBlendMode = EBM_ALPHA;
					pColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
					OriginalAlpha = 1.0f;
				}

				~IMaterial()
				{
					for(int i = 0; i != MAX_TEXTURE_LAYERS; ++i)
						setTexture(i, 0);

					setShader(0);
				}

				void setTexture(const int num, ITexture* m_pTexture)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						if(pTexture[num])
							pTexture[num]->remove();

						pTexture[num] = m_pTexture;

						if(pTexture[num])
							pTexture[num]->grab();
					}
				}

				void setShader(IShader* m_pShader)
				{
					if(pShader)
						pShader->remove();

					pShader = m_pShader;

					if(pShader)
						pShader->grab();
				}

				void setTextureScale(const int num, const Vector pScale)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						TextureScale[num] = pScale;
					}
				}

				void setTextureOffset(const int num, const Vector pOffset)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						TextureOffset[num] = pOffset;
					}
				}

				void setAnimation(const int num, const bool pFlag)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						TextureAnimation[num] = pFlag;
					}
				}

				void setBackfaceCulling(const bool pFlag)
				{
					BackfaceCulling = pFlag;
				}

				void setUseAmbientColor(const bool pFlag)
				{
					UseAmbientColor = pFlag;
				}

				void setAnimationCount(const int num, const Vector pAnimationCount)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						AnimationCount[num] = pAnimationCount;

						ScaleAnimation[num].x = 1.0f/AnimationCount[num].x;

						//we subtract -0.00001f because otherwise
						//the edges of the next animation frame will be visible
						//sometimes because of rounding errors/some weird texel stuff
						//GL_CLAMP_TO_EDGE is supposed to work, but it doesn't, so
						//screw it, this works most of the time!
						ScaleAnimation[num].y = 1.0f/AnimationCount[num].y-0.00001f;

						OffsetAnimation[num].x = 0.0f;
						OffsetAnimation[num].y = 0.0f;
					}
				}

				void setAnimationLoop(const int num, const Vector pAnimationLoop,
				const int pLoopCount = 0)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						AnimationLoop[num] = pAnimationLoop;
						AnimationLoopCount[num] = pLoopCount;

						AnimationLoop[num].clamp(0, AnimationCount[num].product());

						AnimationLoopInterval[num] = (int)(AnimationSpeed[num]/
							(AnimationLoop[num].y - AnimationLoop[num].x));

						if(AnimationLoopInterval[num] < 0)
							AnimationLoopInterval[num] *= -1;

						LastAnimationLoopTime[num] = AnimationLoopInterval[num];

						//set the frame to startframe+1 to avoid a delay
						//at animation start
						//explanation: we set the animation frame below,
						//in the update() function we set the animation frame
						//again after a time-delay because LastAnimationFrame
						//gets modified _after_ we apply the animation-frame
						//in the update() loop
						LastAnimationFrame[num] = int(AnimationLoop[num].x+1);
						AnimationFrame[num] = int(AnimationLoop[num].x);
						AnimationLoopIndex[num] = 0;

						setAnimationFrame(num, AnimationFrame[num]);
					}
				}

				void setAnimationSpeed(const int num, const int pAnimationSpeed)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						AnimationSpeed[num] = pAnimationSpeed;

						AnimationLoopInterval[num] = (int)(AnimationSpeed[num]/
							(AnimationLoop[num].y - AnimationLoop[num].x));

						if(AnimationLoopInterval[num] < 0)
							AnimationLoopInterval[num] *= -1;
					}
				}

				void setAnimationFrame(const int num, const int pAnimationFrame)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						AnimationFrame[num] = pAnimationFrame;

						if(AnimationFrame[num] > AnimationCount[num].product())
							AnimationFrame[num] = int(AnimationCount[num].product());

						if(AnimationFrame[num] < 1)
							AnimationFrame[num] = 1;

						for(unsigned int i = 1; i <= AnimationCount[num].y; ++i)
						{
							if(AnimationFrame[num] > AnimationCount[num].x*i)
								OffsetAnimation[num].y = ScaleAnimation[num].y*i;
						}

						OffsetAnimation[num].x = ScaleAnimation[num].x*(AnimationFrame[num]-1);
					}
				}

				void setTextureFilter(const int num, const TextureMode pTextureFilter)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						TextureFilter[num] = pTextureFilter;
					}
				}

				void setBlendMode(const BlendMode m_pBlendMode)
				{
					pBlendMode = m_pBlendMode;
				}

				void setTextureBlendMode(const int num, const BlendMode m_pBlendMode)
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						TextureBlendMode[num] = m_pBlendMode;
					}
				}

				void setColor(const Color m_pColor)
				{
					pColor = m_pColor;
					OriginalAlpha = pColor.a;
				}

				void setWireframe(const bool pFlag)
				{
					Wireframe = pFlag;
				}

				void setWireWidth(const float pWireWidth)
				{
					WireWidth = pWireWidth;
				}

				void setBlend(const bool pBlendIn, const int pMilliseconds = 100)
				{
					BlendSpeed = pMilliseconds;
					BlendTime = 1;

					if(pBlendIn)
					{
						BlendOut = false;
						BlendIn = true;

						BlendFactor = OriginalAlpha/BlendSpeed;
					}
					else
					{
						BlendOut = true;
						BlendIn = false;

						BlendFactor = pColor.a/BlendSpeed;
					}
				}

				ITexture* getTexture(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return pTexture[num];
					}

					return 0;
				}

				IShader* getShader() const
				{
					return pShader;
				}

				Vector getTextureScale(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						if(TextureAnimation[num])
							return ScaleAnimation[num];

						return TextureScale[num];
					}

					return Vector();
				}

				Vector getTextureOffset(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						if(TextureAnimation[num])
							return OffsetAnimation[num];

						return TextureOffset[num];
					}

					return Vector();
				}

				bool getAnimation(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return TextureAnimation[num];
					}

					return false;
				}

				bool getBackfaceCulling() const
				{
					return BackfaceCulling;
				}

				bool getUseAmbientColor() const
				{
					return UseAmbientColor;
				}

				bool getBlendIn() const
				{
					return BlendIn;
				}

				bool getBlendOut() const
				{
					return BlendOut;
				}

				Vector getAnimationCount(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return AnimationCount[num];
					}

					return Vector();
				}

				Vector getAnimationLoop(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return AnimationLoop[num];
					}

					return Vector();
				}

				int getAnimationSpeed(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return AnimationSpeed[num];
					}

					return 0;
				}

				int getAnimationFrame(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return AnimationFrame[num];
					}

					return 0;
				}

				TextureMode getTextureFilter(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return TextureFilter[num];
					}

					return ETF_NEAR;
				}

				BlendMode getBlendMode() const
				{
					return pBlendMode;
				}

				BlendMode getTextureBlendMode(const int num) const
				{
					if(num >= 0 && num < MAX_TEXTURE_LAYERS)
					{
						return TextureBlendMode[num];
					}

					return EBM_NONE;
				}

				Color getColor() const
				{
					return pColor;
				}

				bool getWireframe() const
				{
					return Wireframe;
				}

				float getWireWidth() const
				{
					return WireWidth;
				}

				void update(const int pTime)
				{
					if(BlendIn || BlendOut)
					{
						if((pTime - BlendTime) >= 1)
						{
							BlendTime = pTime;

							if(BlendIn)
							{
								if(pColor.a >= OriginalAlpha)
								{
									BlendIn = false;
									pColor.a = OriginalAlpha;
								}
								else
								{
									pColor.a += BlendFactor;
								}
							}

							if(BlendOut)
							{
								if(pColor.a <= 0.0f)
								{
									pColor.a = 0.0f;
									BlendOut = false;
								}
								else
								{
									pColor.a -= BlendFactor;
								}
							}
						}
					}

					for(int i = 0; i != MAX_TEXTURE_LAYERS; ++i)
					{
						if(TextureAnimation[i])
						{
							if(AnimationLoopIndex[i] <= AnimationLoopCount[i])
							{
								if(pTime - LastAnimationLoopTime[i] >= AnimationLoopInterval[i])
								{
									//loop forever if loop-counter is set to 0
									if(AnimationLoopCount[i] > 0)
									{
										//increase the loop counter by one
										if(LastAnimationFrame[i] == AnimationLoop[i].y)
											AnimationLoopIndex[i] += 1;
									}
									else
									{
										//animation has finished
										if(AnimationFrame[i] == AnimationLoop[i].y)
										{
											//reset the animation texture coordinates
											setAnimationCount(i, AnimationCount[i]);

											//set the animation frame to the start frame again
											LastAnimationFrame[i] = int(AnimationLoop[i].x);
										}
									}

									setAnimationFrame(i, LastAnimationFrame[i]);

									LastAnimationLoopTime[i] = pTime;
									LastAnimationFrame[i] += 1;
								}
							}
						}
					}
				}


			private:
				ITexture* pTexture[MAX_TEXTURE_LAYERS];
				IShader* pShader;

				Vector TextureScale[MAX_TEXTURE_LAYERS];
				Vector TextureOffset[MAX_TEXTURE_LAYERS];
				Vector AnimationCount[MAX_TEXTURE_LAYERS];
				Vector AnimationLoop[MAX_TEXTURE_LAYERS];
				Vector ScaleAnimation[MAX_TEXTURE_LAYERS];
				Vector OffsetAnimation[MAX_TEXTURE_LAYERS];

				int AnimationSpeed[MAX_TEXTURE_LAYERS];
				int AnimationFrame[MAX_TEXTURE_LAYERS];
				int AnimationLoopCount[MAX_TEXTURE_LAYERS];
				int AnimationLoopIndex[MAX_TEXTURE_LAYERS];
				int AnimationLoopInterval[MAX_TEXTURE_LAYERS];
				int LastAnimationLoopTime[MAX_TEXTURE_LAYERS];
				int LastAnimationFrame[MAX_TEXTURE_LAYERS];

				float WireWidth;
				float OriginalAlpha;
				float BlendFactor;
				int BlendSpeed;
				int BlendTime;

				bool TextureAnimation[MAX_TEXTURE_LAYERS];
				bool Wireframe;
				bool BackfaceCulling;
				bool UseAmbientColor;
				bool BlendIn;
				bool BlendOut;

				TextureMode TextureFilter[MAX_TEXTURE_LAYERS];
				BlendMode TextureBlendMode[MAX_TEXTURE_LAYERS];
				BlendMode pBlendMode;

				Color pColor;
		};
	};
};

#endif

