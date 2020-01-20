// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ColorGfxHeader_
#define _ColorGfxHeader_

namespace lew
{
	namespace gfx
	{
		class Color
		{
			public:
				Color(const float p1 = 0.0f, const float p2 = 0.0f,
						const float p3 = 0.0f, const float p4 = 0.0f)
				{
					r = p1;
					g = p2;
					b = p3;
					a = p4;
				}

				Color operator=(const Color New)
				{r = New.r; g = New.g; b = New.b; a = New.a; return *this;}

				Color operator=(const float Value)
				{r = Value; g = Value; b = Value; a = Value; return *this;}

				Color operator+(const Color New) const
				{return Color(r+New.r, g+New.g, b+New.b, a+New.a);}

				Color operator+(const float Value) const
				{return Color(r+Value, g+Value, b+Value, a+Value);}

				Color operator+=(const Color New)
				{return Color(r+=New.r, g+=New.g, b+=New.b, a+=New.a);}

				Color operator+=(const float Value)
				{return Color(r+=Value, g+=Value, b+=Value, a+=Value);}

				Color operator-(const Color New) const
				{return Color(r-New.r, g-New.g, b-New.b, a-New.a);}

				Color operator-(const float Value) const
				{return Color(r-Value, g-Value, b-Value, a-Value);}

				Color operator-=(const Color New)
				{return Color(r-=New.r, g-=New.g, b-=New.b, a-=New.a);}

				Color operator-=(const float Value)
				{return Color(r-=Value, g-=Value, b-=Value, a-=Value);}

				Color operator*(const Color New) const
				{return Color(r*New.r, g*New.g, b*New.b, a*New.a);}

				Color operator*(const float Value) const
				{return Color(r*Value, g*Value, b*Value, a*Value);}

				Color operator*=(const Color New)
				{return Color(r*=New.r, g*=New.g, b*=New.b, a*=New.a);}

				Color operator*=(const float Value)
				{return Color(r*=Value, g*=Value, b*=Value, a*=Value);}

				Color operator/(const Color New) const
				{return Color(r/New.r, g/New.g, b/New.b, a/New.a);}

				Color operator/(const float Value) const
				{return Color(r/Value, g/Value, b/Value, a/Value);}

				Color operator/=(const Color New)
				{return Color(r/=New.r, g/=New.g, b/=New.b, a/=New.a);}

				Color operator/=(const float Value)
				{return Color(r/=Value, g/=Value, b/=Value, a/=Value);}

				bool operator<(const Color New) const
				{return r<New.r && g<New.g && b<New.b && a<New.a;}

				bool operator<(const float Value) const
				{return r<Value && g<Value && b<Value && a<Value;}
				
				bool operator<=(const Color New) const
				{return r<=New.r && g<=New.g && b<=New.b && a<=New.a;}

				bool operator<=(const float Value) const
				{return r<=Value && g<=Value && b<=Value && a<=Value;}

				bool operator>(const Color New) const
				{return r>New.r && g>New.g && b>New.b && a>New.a;}

				bool operator>(const float Value) const
				{return r>Value && g>Value && b>Value && a>Value;}

				bool operator>=(const Color New) const
				{return r>=New.r && g>=New.g && b>=New.b && a>=New.a;}

				bool operator>=(const float Value) const
				{return r>=Value && g>=Value && b>=Value && a>=Value;}

				bool operator==(const Color New) const
				{return r==New.r && g==New.g && b==New.b && a==New.a;}

				bool operator==(const float Value) const
				{return r==Value && g==Value && b ==Value && a==Value;}

				bool operator!=(const Color New) const
				{return r!=New.r || g!=New.g || b!=New.b || a!=New.a;}

				bool operator!=(const float Value) const
				{return r!=Value || g!=Value || b!=Value || a!=Value;}

				void clear()
				{
					r = g = b = a = 0.0f;
				}
				
				void clamp(const float pMin, const float pMax)
				{
					if(r < pMin)
						r = pMin;
					
					if(r > pMax)
						r = pMax;
						
					if(g < pMin)
						g = pMin;
						
					if(g > pMax)
						g = pMax;
						
					if(b < pMin)
						b = pMin;
						
					if(b > pMax)
						b = pMax;
						
					if(a < pMin)
						a = pMin;
						
					if(a > pMax)
						a = pMax;
				}
				
				void clamp(const Color pMin, const Color pMax)
				{
					if(r < pMin.r)
						r = pMin.r;
					
					if(r > pMax.r)
						r = pMax.r;
						
					if(g < pMin.g)
						g = pMin.g;
					
					if(g > pMax.g)
						g = pMax.g;
						
					if(b < pMin.b)
						b = pMin.b;
					
					if(b > pMax.b)
						b = pMax.b;
						
					if(a < pMin.a)
						a = pMin.a;
					
					if(a > pMax.a)
						a = pMax.a;
				}
				
				void contrast(const float pContr)
				{
					if(pContr > 0.0f)
					{
						float v[4] = {r*255, g*255, b*255, a*255};
						for(int i = 0; i != 4; ++i)
						{
							if(v[i] < (int)(128.0f + 128.0f * tan(pContr)) && v[i] > 
							(int)(128.0f - 128.0f * tan(pContr)))
							{
								v[i] = (v[i] - 128)/tan(pContr) + 128;
								v[i] = (1.0f/255.0f) * v[i];
							}
							else if(v[i] >= (int)(128.0f + 128.0f * tan(pContr)))
							{
								v[i] = 1.0f;
							}
							else
							{
								v[i] = 0.0f;
							}
						}
						
						r = v[0];
						g = v[1];
						b = v[2];
						a = v[3];	
					}
					else
					{		
						Color col = Color(r, g, b, a);
								
						if(col >= 0.5f)
						{
							col += pContr;
							col.clamp(0.5f, 1.0f);
						}
						else
						{
							col -= pContr;	
							col.clamp(0.0f, 0.5f);
						}	
						
						r = col.r;
						b = col.b;
						g = col.g;
						a = col.a;		
					}
					
					clamp(0.0f, 1.0f);				
				}
				
				void invert()
				{
					r = 1.0f - r;
					g = 1.0f - g;
					b = 1.0f - b;
					a = 1.0f - a;
					
					clamp(0.0f, 1.0f);
				}
				
				Color blend(const Color pColor, float pFactor)
				{
					if(pFactor > 1.0f)
						pFactor = 1.0f;
						
					if(pFactor < 0.0f)
						pFactor = 0.0f;
						
					const float nr = (r*(1.0f-pFactor)) + (pColor.r*pFactor);
					const float ng = (g*(1.0f-pFactor)) + (pColor.g*pFactor);
					const float nb = (b*(1.0f-pFactor)) + (pColor.b*pFactor);
					const float na = (a*(1.0f-pFactor)) + (pColor.a*pFactor);
					
					return Color(nr, ng, nb, na);
				}

				float r, g, b, a;
		};
	};
};

#endif

