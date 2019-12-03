#include "CFontTool.h"
#include "IXMLWriter.h"


#include "TTFText.h"

#pragma comment(lib,"gdiplus.lib")
#include <gdiplus.h>

using namespace Gdiplus;
#include <iostream> 

using namespace irr;

const int fontsizes[] = {4,6,8,9,10,11,12,14,16,18,20,22,24,26,28,32,36,42,48,56,68,72,0};

inline u32 getTextureSizeFromSurfaceSize_2(u32 size)
{
	u32 ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}

// windows specific
#ifdef _IRR_WINDOWS_

	const DWORD charsets[] = { ANSI_CHARSET, DEFAULT_CHARSET, OEM_CHARSET, BALTIC_CHARSET, GB2312_CHARSET, CHINESEBIG5_CHARSET,
								EASTEUROPE_CHARSET, GREEK_CHARSET, HANGUL_CHARSET, MAC_CHARSET, RUSSIAN_CHARSET,
								SHIFTJIS_CHARSET, SYMBOL_CHARSET, TURKISH_CHARSET, VIETNAMESE_CHARSET, JOHAB_CHARSET,
								ARABIC_CHARSET, HEBREW_CHARSET, THAI_CHARSET, 0};

	const wchar_t *setnames[] = {L"ANSI", L"All Available", L"OEM", L"Baltic", L"Chinese Simplified", L"Chinese Traditional",
								L"Eastern European", L"Greek", L"Hangul", L"Macintosh", L"Russian",
								L"Japanese", L"Symbol", L"Turkish", L"Vietnamese", L"Johab",
								L"Arabic", L"Hebrew", L"Thai", 0};

	// callback for adding font names
	int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme,
					DWORD FontType, LPARAM lParam)
	{
		CFontTool* t = (CFontTool*) lParam;
		if ((TRUETYPE_FONTTYPE == FontType) && (lpelfe->elfFullName[0] != L'@'))
		{
			if ( t->FontNames.binary_search(core::stringw(lpelfe->elfFullName)) < 0)
				t->FontNames.push_back( core::stringw(lpelfe->elfFullName));
		}
		return 1;
	}

	void CFontTool::freeFontMemory()
	{
		// get rid of the current textures/images
		for (u32 i=0; i<currentTextures.size(); ++i)
			if (currentTextures[i])
			{
				Device->getVideoDriver()->removeTexture(currentTextures[i]);
				currentTextures[i]->drop();
			}
		currentTextures.clear();

		for (u32 i=0; i<currentImages.size(); ++i)
			if (currentImages[i])
			currentImages[i]->drop();
		currentImages.clear();

//		Device->getVideoDriver()->removeAllTextures();

		// clear current image mappings
		CharMap.clear();
		// clear array
		Areas.clear();
	}

	bool CFontTool::showProgress(core::stringw * Logmsg)
	{
		video::IVideoDriver* driver = Device->getVideoDriver();
		scene::ISceneManager* smgr = Device->getSceneManager();
		gui::IGUIEnvironment *env = Device->getGUIEnvironment();
		gui::IGUIFont* font = env->getBuiltInFont();
		if (Device->run())
		{
			if (Device->isWindowActive())
				{
					video::IImage*		textImage = 0;
					video::ITexture*	textTexture = 0;
//*
					if (DefaultWinTextFormat.text_multiline)
						textImage = WinText2Image_old(Device,Logmsg->c_str());

					if (textImage)
					{
						textTexture = driver->addTexture("TTFTextProgress",textImage);
						driver->makeColorKeyTexture(textTexture, video::SColor(255,0,0,0));
						textTexture->grab();
//						driver->makeColorKeyTexture(textTexture, core::dimension2d<s32>(1,1));

						gui::IGUIImage* img = (gui::IGUIImage*)env->getRootGUIElement()->getElementFromId(101010,true);
						img->setVisible(true);
						img->setScaleImage(false);
						img->setImage(textTexture);
/*
//						float zoomTO = 1;
						core::rect<s32> imgrect;
						imgrect = img->getRelativePosition();
						imgrect.LowerRightCorner.X = imgrect.UpperLeftCorner.X + textTexture->getSize().Width;
						imgrect.LowerRightCorner.Y = imgrect.UpperLeftCorner.Y + textTexture->getSize().Height;
//						imgrect.LowerRightCorner.X = imgrect.UpperLeftCorner.X + s32((float)textTexture->getSize().Width*zoomTO);
//						imgrect.LowerRightCorner.Y = imgrect.UpperLeftCorner.Y + s32((float)textTexture->getSize().Height*zoomTO);
						img->setRelativePosition(imgrect);
						img->setScaleImage(true);
/**/
//						driver->writeImageToFile(textImage,"_TextImage.BMP");

					}
/**/
					driver->beginScene(true, true, video::SColor(0,200,200,200));
					smgr->drawAll();
					env->drawAll();
/*
					if (textTexture)
					{
//						driver->enableMaterial2D();
						driver->draw2DImage(textTexture, core::dimension2d<s32>(210,20));
	//					driver->draw2DImage(textTexture, core::dimension2d<s32>(0,0),;
	//					driver->draw2DImage(textTexture, core::rect<s32>(0,0,800,600),core::rect<s32>(0,0,400,300));
//						driver->enableMaterial2D(false);
					}
/**/
					if (font)
//						font->draw(L"This demo shows Color 2D TrueType TEXT 2 Textute",
//						font->draw(L"Work in progress...",
						{
							if (textTexture)
//							font->draw(L"Work in progress...", core::rect<s32>(10,5,400,400),video::SColor(255,0,255,0))
							;
							else
							font->draw(Logmsg->c_str(),
								core::rect<s32>(10,25,400,400),
								video::SColor(255,0,255,0));
						}

					driver->endScene();

					if (textImage)	 
					{
						textImage->drop();
					}
					if (textTexture) 
					{
						driver->removeTexture(textTexture);
						textTexture->drop();
					}
				}
			if (!AbortIT)
				{
				Device->sleep( 1 );
				return false;
				}
			*Logmsg = L"\nProcess ABORTED!!!";
			Device->getLogger()->log(Logmsg->c_str());
			if (DefaultWinTextFormat.text_multiline)
				{
				video::IImage*		textImage = 0;
				video::ITexture*	textTexture = 0;
				
				DefaultWinTextFormat.text_center = true;
				textImage = WinText2Image_old(Device,Logmsg->c_str());

				if (textImage)
					{
					textTexture = driver->addTexture("TTFTextProgress",textImage);
					driver->makeColorKeyTexture(textTexture, video::SColor(255,0,0,0));
					textTexture->grab();

					gui::IGUIImage* img = (gui::IGUIImage*)env->getRootGUIElement()->getElementFromId(101010,true);
					img->setScaleImage(false);
					img->setImage(textTexture);
					if (textImage)	 
						{
						textImage->drop();
						}
					if (textTexture) 
						{
						driver->removeTexture(textTexture);
						textTexture->drop();
						}
					}
				}

		}

			freeFontMemory();
			return true;

	};
	//
	// Constructor
	//
	CFontTool::CFontTool(IrrlichtDevice* device) : FontSizes(fontsizes),
			Device(device), UseAlphaChannel(false),
			BitmapFontStyleFormat(-1),
			previewImage(0), previewTexture(0),
//			textImage(0), textTexture(0),
			// win specific
			dc(0), gdiplusToken(0) 
	{
		// init display context
		dc = CreateDC(L"DISPLAY", L"DISPLAY", 0 ,0 );

		// Initialize GDI+.
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		// populate list of available character set names
		for (int i=0; setnames[i] != 0; ++i)
			CharSets.push_back( core::stringw(setnames[i]));

		selectCharSet(0);
	}

	void CFontTool::selectCharSet(u32 currentCharSet)
	{
		if ( currentCharSet >= CharSets.size() )
			return;

		LOGFONTW lf;
		lf.lfFaceName[0] = L'\0';
		lf.lfCharSet = (BYTE) charsets[currentCharSet];
		// HRESULT hr; // no error checking(!)

		// clear font list
		FontNames.clear();

		// create list of available fonts
		EnumFontFamiliesExW( dc, (LPLOGFONTW) &lf, (FONTENUMPROCW) EnumFontFamExProc, (LPARAM) this, 0);
		FontNames.sort();
	}

	void * memor (void *dest,void *src, size_t n)
	{
	  char *s = (char *) src;
	  char *d = (char *) dest;

	  for (; n > 0; n--)
		*d++ |= *s++;
	  return dest;
	}

	bool ScanAndGetCharABCWidths(Bitmap * BMP,const RectF * boundRect,ABC * abcX,ABC * abcY, bool debug = false)
	{
		abcX->abcA = 0;
		abcX->abcB = 0;
		abcX->abcC = 0;

		abcY->abcA = 0;
		abcY->abcB = 0;
		abcY->abcC = 0;

			BitmapData* bitmapData = new BitmapData;
			Rect BMPrect(0, 0, BMP->GetWidth(), BMP->GetHeight());

			BMP->LockBits(
				&BMPrect,
				ImageLockModeRead,
				PixelFormat32bppARGB,
				bitmapData);

			UINT* pixels = (UINT*)bitmapData->Scan0;
			u32 * pixels32 = (u32*)bitmapData->Scan0;
			u32 pixel32;


			UINT xfrom = bitmapData->Width;
			UINT xto   = 0;
			UINT yfrom = bitmapData->Height;
			UINT yto   = 0;
			bool hasData = false;

			for(UINT row = 0; row < bitmapData->Height; ++row)
				for(UINT col = 0; col < bitmapData->Width; ++col)
				{
					pixels32[row * bitmapData->Width + col] &= 0x00FFFFFF;//remove alpha
					pixel32 = pixels32[row * bitmapData->Width + col];
					pixel32 &= 0x00FFFFFF;//remove alpha
					if (pixel32 != 0)
					{
						hasData = true;
						if (xfrom > col) xfrom = col;
						if (xto   < col) xto   = col;
						if (yfrom > row) yfrom = row;
						if (yto   < row) yto   = row;
//						pixels32[row * bitmapData->Width + col] = 0x0000FF00;
//						if (debug)
//							pixels32[row * bitmapData->Width + col] = 0xFFFFFFFF;//remove alpha
					}

				}

			BMP->UnlockBits(bitmapData);

		delete bitmapData;

		if (!hasData || (xto < xfrom)) 
			return false;
		abcX->abcA = (xfrom) - (int) boundRect->X;
		abcX->abcC = (int) boundRect->X + (int) boundRect->Width - xto;
//		abcX->abcC = xfrom;
		abcX->abcB = xto - xfrom + 1; 

		abcY->abcA = yfrom - (int) boundRect->Y;
		abcY->abcC = yto - (int) boundRect->Y;
		abcY->abcB = yto - yfrom + 1; 

		return true;
	}

	bool ScanAndGetCharABCWidths_(Bitmap * BMP,const RectF * boundRect,ABC * abc)
	{
		abc->abcA = 0;
		abc->abcB = 0;
		abc->abcC = 0;
			BitmapData* bitmapData = new BitmapData;
			Rect BMPrect ((int) boundRect->X,(int) boundRect->Y,(int) boundRect->Width,(int) boundRect->Height);

			BMP->LockBits(
				&BMPrect,
				ImageLockModeRead,
				PixelFormat32bppARGB,
				bitmapData);

			UINT* pixels = (UINT*)bitmapData->Scan0;
			u32 * pixels32 = (u32*)bitmapData->Scan0;
			u32 *BMPData = new u32[bitmapData->Width];

			for(UINT col = 0; col < bitmapData->Width; ++col)
				BMPData[col] = 0;
/*
			if (bitmapData->Stride < 0)//down-top or top-down BMP orientation
				for(UINT row = 0; row < bitmapData->Height; ++row)
					memor(BMPData,&pixels32[(bitmapData->Height-row-1) * (bitmapData->Width)], bitmapData->Width*4);
			else
				for(UINT row = 0; row < bitmapData->Height; ++row)
					memor(BMPData,&pixels32[row * (bitmapData->Width)], bitmapData->Width*4);
/**/			
			UINT xfrom = bitmapData->Width;
			UINT xto   = 0;
			bool hasData = false;

			for(UINT row = 0; row < bitmapData->Height; ++row)
				for(UINT col = 0; col < bitmapData->Width; ++col)
				{
//					pixels32[row * (bitmapData->Stride / 4) + col] = 0xFFFFFF00;
					pixels32[row * (bitmapData->Stride / 4) + col] &= 0x00FFFFFF;//remove alpha
//*
					if (pixels32[row * (bitmapData->Stride / 4) + col] != 0)
					{
						hasData = true;
						if (xfrom > col) xfrom = col;
						if (xto   < col) xto   = col;
						pixels32[row * (bitmapData->Stride / 4) + col] = 0xFFFF0000;//remove alpha
					}
					else
						pixels32[row * (bitmapData->Stride / 4) + col] = 0xFF00FF00;//remove alpha
/**/
				}

/*
			for(UINT col = 0; col < bitmapData->Width; ++col)
			{
				BMPData[col] &= 0x00FFFFFF;//remove alpha
				if (BMPData[col] != 0)
				{
					hasData = true;
					if (xfrom > col) xfrom = col;
					if (xto   < col) xto   = col;
				}

			}
/**/
			delete bitmapData;

			delete [] BMPData;
			BMP->UnlockBits(bitmapData);
		if (!hasData || (xto < xfrom)) 
			return false;
		abc->abcA = xfrom - (int) boundRect->X;
		abc->abcC = (int) boundRect->X + (int) boundRect->Width - xto;
		abc->abcB = xto - xfrom + 1; 

		return true;
	}


	bool CFontTool::makePreviewFont(u32 fontIndex, wchar_t PreviewChar, 
				s32 fontSize, u32 texturewidth, u32 textureHeight,
				bool bold, bool italic, bool aa, bool alpha,
				bool textfillmode,
				bool aaoutline, bool textoveroutline,
				u32 space_between_letters, 
				u32 bgColor, s32 textColorMode, u32 textColor1, u32 textColor2, 
				u32 outline1ColorMode, u32 outline1Color1, u32 outline1Color2, u32 outline1Style, u32 outline1Size,
				u32 outline2ColorMode, u32 outline2Color1, u32 outline2Color2, u32 outline2Style, u32 outline2Size,
				bool testmode
		)
	{
		wchar_t wch = PreviewChar;
		u32 outLineSize  = outline1Size;
		u32 outLineSize2 = outline2Size;
		if (alpha)
		{
			bgColor &= 0x00FFFFFF;
		};
		if (fontIndex >= FontNames.size())
			return false;

		video::IVideoDriver* driver = Device->getVideoDriver();
		if (previewImage)	 
			{
				previewImage->drop();
			}
		if (previewTexture) 
			{
				driver->removeTexture(previewTexture);
				previewTexture->drop();
			}

		FontFamily  fontFamily(FontNames[fontIndex].c_str());
		FontStyle fontStyle = (bold && italic) ? FontStyleBoldItalic : (FontStyleRegular | bold ? FontStyleBold : FontStyleRegular | italic ? FontStyleItalic : FontStyleRegular);
		Font        font(&fontFamily,(float) fontSize , fontStyle, UnitPixel);
		StringFormat stringFormat;
		stringFormat.GenericTypographic();

		Bitmap BMPTemp(fontSize*3 + outLineSize + outLineSize2,fontSize*3 + outLineSize + outLineSize2,PixelFormat32bppARGB);
		SolidBrush  whitebrush(Color(255, 255, 255, 255));
		SolidBrush  blackbrush(Color(255, 0, 0, 0));
		Graphics gTemp(&BMPTemp);

		gTemp.SetInterpolationMode(InterpolationModeHighQualityBicubic);

				// get the dimensions
				SIZE size;
				ABC abcX;
				ABC abcY;
//				GetTextExtentPoint32W(dc, &currentchar, 1, &size);
				SFontArea fa;
				fa.underhang = 0;
				fa.overhang  = 0;

				s32 plusY = 0;

				RectF layoutRect((float)(int)((fontSize*3 + outLineSize + outLineSize2) * 2 / 5),(float)(int)((fontSize*3 + outLineSize + outLineSize2) / 3), 100.0f, 50.0f);
//				StringFormat format;
//				format.SetAlignment(StringAlignmentFar);
				RectF boundRect;
				// Measure the string.

//				Bitmap BMPTemp_(fontSize*3,fontSize*3,PixelFormat32bppARGB);
//				Graphics gTemp_(&BMPTemp_);

				if (aa)
					gTemp.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
				else
					gTemp.SetTextRenderingHint(TextRenderingHintSingleBitPerPixelGridFit);
				if (aaoutline)
					gTemp.SetSmoothingMode(SmoothingModeAntiAlias);
				else
					gTemp.SetSmoothingMode(SmoothingModeNone);

				gTemp.MeasureString(&wch, 1, &font, layoutRect, &stringFormat, &boundRect);
				// Draw a rectangle that represents the size of the string.
				size.cx = (LONG) boundRect.Width;
				size.cy = (LONG) boundRect.Height;

				gTemp.FillRectangle(&blackbrush,0,0, fontSize*3 + outLineSize + outLineSize2,fontSize*3 + outLineSize + outLineSize2);

				if ((outLineSize + outLineSize2 > 0) || textfillmode)
				{
					GraphicsPath path;
				    path.AddString(&wch, 1, &fontFamily, 
						fontStyle,(float) fontSize, boundRect, &stringFormat);
					if (outLineSize + outLineSize2 > 0)
					{
						Pen pen(Color(234,137,6),(float) outLineSize + outLineSize2 );
						pen.SetLineJoin(LineJoinRound);
						gTemp.DrawPath(&pen, &path);
					}
//					SolidBrush brush(Color(128,0,255));
					if (aa)
						gTemp.SetSmoothingMode(SmoothingModeAntiAlias);
					else
						gTemp.SetSmoothingMode(SmoothingModeNone);
					gTemp.FillPath(&whitebrush, &path);
				}
				else
					gTemp.DrawString(&wch, 1, &font, boundRect, &stringFormat, &whitebrush);

				if (ScanAndGetCharABCWidths(&BMPTemp, &boundRect, &abcX, &abcY,false))
				{
					size.cx = abcX.abcB + space_between_letters;
					fa.underhang  = abcX.abcA;
					fa.overhang   = abcX.abcC;

//					if ((int) size.cy < (abcY.abcC + (int)outLineSize))
					if ((abcY.abcC + (int) plusY) > 0)
						size.cy = abcY.abcC + (plusY) + 1;
//					if ((abcY.abcB + (int) plusY) > size.cy)
//						size.cy = abcY.abcB + (plusY) + 1;
//					size.cy = abcY.abcA+abcY.abcB+abcY.abcC;
//					size.cy += abcY.abcA + abcY.abcC;

//					if (testStatus == 0)
						{
						if (plusY < abcY.abcA*(-1))
							plusY = abcY.abcA*(-1);
						}


					RectF		rectF_ = boundRect;
					rectF_.X += (float) abcX.abcA;
					rectF_.Width = (float) size.cx;
					rectF_.Height = (float) size.cy;

				}
				else
					{
					size.cx += int((outLineSize + outLineSize2)/4) + space_between_letters;
					}


		previewTextureWidth = size.cx;
		previewTextureHeight = size.cy+plusY;
		u32 texWidth  = getTextureSizeFromSurfaceSize(previewTextureWidth);
		u32 texHeight = getTextureSizeFromSurfaceSize(previewTextureHeight);

		// make a new bitmap
		Bitmap BMP(texWidth,texHeight,PixelFormat32bppARGB);
		Graphics g(&BMP);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

//			if (!UseAlphaChannel)
		SolidBrush  bgbrush(bgColor);
		g.FillRectangle(&bgbrush,0,0, previewTextureWidth, previewTextureHeight);

		PointF      pointF(0-(float)fa.underhang, (float)plusY);
		RectF		rectF(0-(float)fa.underhang, (float)plusY,(float) previewTextureWidth + fa.underhang,(float) previewTextureHeight);
		RectF		rectF_(0, 0,(float) previewTextureWidth,(float) previewTextureHeight);
		RectF		rectF_x2 = rectF_; rectF_x2.Height /= 2;

					if (aa)
						g.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
					else
						g.SetTextRenderingHint(TextRenderingHintSingleBitPerPixelGridFit);
					if (aaoutline)
						g.SetSmoothingMode(SmoothingModeAntiAlias);
					else
						g.SetSmoothingMode(SmoothingModeNone);

					Brush * brush = 0;
					LinearGradientBrush * brush_ = 0;
					switch (textColorMode)
					{
						case 1 :
							brush_ = new LinearGradientBrush (
								rectF_,
								textColor2,   // red
								textColor1,
								LinearGradientModeVertical);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						case 2 :
							brush_ = new LinearGradientBrush (
								rectF_,
								textColor2,   // red
								textColor1,
								LinearGradientModeForwardDiagonal);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						case 3 :
							brush_ = new LinearGradientBrush (
								rectF_,
								textColor2,   // red
								textColor1,
								LinearGradientModeBackwardDiagonal);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						case 4 :
							brush_ = new LinearGradientBrush (
								rectF_x2,
								textColor2,   // red
								textColor1,
								LinearGradientModeVertical);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						default : 
							brush = new SolidBrush(textColor1); 
							break;
					}


					//draw OUTLINE 1 & 2
					if ((outLineSize + outLineSize2 > 0) || textfillmode)
					{
						GraphicsPath path;
						path.AddString(&wch, 1, &fontFamily, 
							fontStyle,(float) fontSize, rectF, &stringFormat);


//						if (testmode)
//							path.SetFillMode(FillModeAlternate);
//						else
//							path.SetFillMode(FillModeWinding);

						//draw OUTLINE 2
						if (outLineSize2)
						{
							Brush * pbrush = 0;
							LinearGradientBrush * pbrush_ = 0;
							switch (outline2ColorMode)
							{
								case 1 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 2 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeForwardDiagonal);  
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 3 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeBackwardDiagonal); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 4 :
									pbrush_ = new LinearGradientBrush (
										rectF_x2,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								default : 
									pbrush = new SolidBrush(outline2Color1); 
									break;
							}


							if (outline2Style == 1)
								for(int i=outLineSize+1; i<(int)outLineSize+(int)outLineSize2; ++i)
								{
									Pen pen(pbrush,(float) i);
									pen.SetLineJoin(LineJoinRound);
/*
if (testmode)
	pen.SetAlignment(PenAlignmentInset);
else
	pen.SetAlignment(PenAlignmentCenter);
/**/
									g.DrawPath(&pen, &path);
								}
							else
							{
								Pen pen(pbrush,(float) outLineSize + outLineSize2);
								pen.SetLineJoin(LineJoinRound);
/*
if (testmode)
	pen.SetAlignment(PenAlignmentInset);
else
	pen.SetAlignment(PenAlignmentCenter);
/**/
								g.DrawPath(&pen, &path);
							}
							delete pbrush;
						}

						//draw OUTLINE 1
						if (outLineSize)
						{
							Brush * pbrush = 0;
							LinearGradientBrush * pbrush_ = 0;
							switch (outline1ColorMode)
							{
								case 1 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 2 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeForwardDiagonal);  
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 3 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeBackwardDiagonal); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 4 :
									pbrush_ = new LinearGradientBrush (
										rectF_x2,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								default : 
									pbrush = new SolidBrush(outline1Color1); 
									break;
							}


							if (outline1Style == 1)
								for(int i=1; i<(int)outLineSize; ++i)
								{
									Pen pen(pbrush,(float) i);
									pen.SetLineJoin(LineJoinRound);
/*
if (testmode)
	pen.SetAlignment(PenAlignmentInset);
else
	pen.SetAlignment(PenAlignmentCenter);
/**/
									g.DrawPath(&pen, &path);
								}
							else
							{
								Pen pen(pbrush,(float) outLineSize);
//if (testmode)
//pen.SetLineJoin(LineJoinBevel);
//	pen.SetMiterLimit(3);
//else
								pen.SetLineJoin(LineJoinRound);
/*
if (testmode)
	pen.SetAlignment(PenAlignmentInset);
else
	pen.SetAlignment(PenAlignmentCenter);
/**/
								g.DrawPath(&pen, &path);
							}
							delete pbrush;
						}
						// FILL TEXT
						if (aa)
							g.SetSmoothingMode(SmoothingModeAntiAlias);
						else
							g.SetSmoothingMode(SmoothingModeNone);
						if (!textoveroutline)
							{
								g.SetCompositingMode(CompositingModeSourceCopy);
							};
						if ((outLineSize + outLineSize2 > 0) && !textfillmode)
							{
								delete brush;
								brush = new SolidBrush(bgColor); 
							}
						g.FillPath(brush, &path);
					}
					else
						g.DrawString(&wch, 1, &font, rectF, &stringFormat, brush);


					delete brush;

			// copy the font bitmap into a new irrlicht image
			BitmapData* bitmapData = new BitmapData;
			Rect BMPrect(0, 0, texWidth, texHeight);

			BMP.LockBits(
				&BMPrect,
				ImageLockModeRead,
				PixelFormat32bppARGB,
				bitmapData);

			UINT* pixels = (UINT*)bitmapData->Scan0;
			u32 * pixels32 = (u32*)bitmapData->Scan0;
			u32 *BMPData = new u32[bitmapData->Height*bitmapData->Width];

			if (bitmapData->Stride < 0)//down-top or top-down BMP orientation
				for(UINT row = 0; row < bitmapData->Height; ++row)
					memcpy(&BMPData[row * bitmapData->Width],&pixels32[(bitmapData->Height-row-1) * (bitmapData->Width)], bitmapData->Width*4);
			else
				for(UINT row = 0; row < bitmapData->Height; ++row)
					memcpy(&BMPData[row * bitmapData->Width],&pixels32[row * (bitmapData->Width)], bitmapData->Width*4);

			// make a texture from the image
			bool btemp = Device->getVideoDriver()->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);	
			previewImage = Device->getVideoDriver()->createImageFromData(video::ECF_A8R8G8B8, core::dimension2d<u32>(bitmapData->Width,bitmapData->Height), (void*)BMPData);
			Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,btemp);

			delete [] BMPData;
			BMP.UnlockBits(bitmapData);
	

			if (previewImage)
			{
				// add texture
				previewTexture = Device->getVideoDriver()->addTexture("GUIFontPreviewImage",previewImage);
				previewTexture->grab();
			}
			else
			{
//				Device->getLogger()->log("Something went wrong, aborting.");
				previewImage->drop();
				// drop all images
				freeFontMemory();
				return false;
			}
		return true;
	}

	bool CFontTool::makeBitmapFont(s32 StyleFormat, u32 fontIndex, u32 charsetIndex, s32 fontSize, u32 textureWidth, u32 textureHeight, bool bold, bool italic, bool aa, bool alpha,
		bool textfillmode, bool aaoutline, bool textoveroutline,
		u32 space_between_letters, u32 bgColor, s32 textColorMode, u32 textColor1, u32 textColor2,
		u32 outline1ColorMode, u32 outline1Color1, u32 outline1Color2, u32 outline1Style, u32 outline1Size,
		u32 outline2ColorMode, u32 outline2Color1, u32 outline2Color2, u32 outline2Style, u32 outline2Size,
		bool testmode, bool ShowDebugInfo)
	{
		u32 dotBeginColor = ARGB(255,128,0,128);
		u32 dotEndColor = ARGB(255,0,128,128);
//		u32 dotBeginColor = ARGB(255,255,255,0);
//		u32 dotEndColor = ARGB(255,255,0,255);
//		s32 StyleFormat = 1;//0 xml/ 1 dot
		BitmapFontStyleFormat = StyleFormat;
		bool fixedHeight = false;
		bool fixedWidth = false;
		s32 MaxImagesCount = 0;
		wchar_t MAXchar = 0;

		if (testmode) MaxImagesCount = 1;
		
		switch (BitmapFontStyleFormat)
		{
		case 1:
			fixedHeight = true;
			MaxImagesCount = 1;
			MAXchar = 381;
			break;
		}

		AbortIT = false;

//		DefaultWinTextFormat.fontname		= "System";
		DefaultWinTextFormat.fontSize		= 8;
//		DefaultWinTextFormat.textureHeight	= 256;//2;
		DefaultWinTextFormat.textureHeight	= 128+32+16;//2;
		DefaultWinTextFormat.textureWidth	= 256;//2;
		DefaultWinTextFormat.font_bold		= true;
		DefaultWinTextFormat.MAX_TEXTURE_SIZE = 512;
		DefaultWinTextFormat.ROUNDTO = 32;
		DefaultWinTextFormat.ROUNDTO_TEXTURE = 0;

		DefaultWinTextFormat.marginLeft		= 10;
//		DefaultWinTextFormat.marginRight	= 10;
		DefaultWinTextFormat.marginTop		= 10;
//		DefaultWinTextFormat.marginBottom	= 20;

//		DefaultWinTextFormat.font_antialias	= true;

		DefaultWinTextFormat.text_multiline	= true;
		DefaultWinTextFormat.text_autosize_texture_to_fit = true;
//		DefaultWinTextFormat.text_autosize_texture_to_fit_advanced = true;

		DefaultWinTextFormat.text_vcenter = true;
		DefaultWinTextFormat.text_center = false;

//		DefaultWinTextFormat.Color.SetGradient();
//		DefaultWinTextFormat.fontColor.SetGradient();
//*
		DefaultTextFrame.Active = true;
		DefaultTextFrame.size = 1;
		DefaultTextFrame.distanceX = 5;
		DefaultTextFrame.distanceY = 5;
		DefaultTextFrame.roundX = 10;
		DefaultTextFrame.roundY = 10;
//		DefaultTextFrame.fillColor.SetActive();
//		DefaultTextFrame.fillColor.SetColor(RGB(128,128,0));
//		DefaultTextFrame.fillColor.SetGradient();
/**/
//		DefaultFrames.Frame[0].Active = true;
		DefaultFrames.Frame[0].size = 1;
		DefaultFrames.Frame[0].distanceX = 5;
		DefaultFrames.Frame[0].distanceY = 20;
		DefaultFrames.Frame[0].roundX = 60;
		DefaultFrames.Frame[0].roundY = 60;
//		DefaultFrames.Frame[0].Color.SetColor(RGB(255,0,0));
		DefaultFrames.Frame[0].fillColor.SetActive();
		DefaultFrames.Frame[0].fillColor.SetColor(RGB(64,64,64));

		DefaultFrames.Frame[1].Active = true;
		DefaultFrames.Frame[1].size = 2;
		DefaultFrames.Frame[1].distanceX = 10;
		DefaultFrames.Frame[1].distanceY = 15;
		DefaultFrames.Frame[1].roundX = 40;
		DefaultFrames.Frame[1].roundY = 40;
//		DefaultFrames.Frame[1].Color.SetColor(RGB(255,255,0));
		DefaultFrames.Frame[1].fillColor.SetActive();
		DefaultFrames.Frame[1].fillColor.SetColor(RGB(128,128,0));
		DefaultFrames.Frame[1].fillColor.SetColors(RGB(255,0,0),RGB(0,0,255),RGB(0,0,0),RGB(0,255,0));

		core::stringw Logmsg;// = L"Work in progress....";

		u32 outLineSize  = outline1Size;
		u32 outLineSize2 = outline2Size;
		if (alpha)
		{
			bgColor &= 0x00FFFFFF;
		};
		if (fontIndex >= FontNames.size() || charsetIndex >= CharSets.size() )
			return false;

		FontFamily  fontFamily(FontNames[fontIndex].c_str());
		FontStyle fontStyle = (bold && italic) ? FontStyleBoldItalic : (FontStyleRegular | bold ? FontStyleBold : FontStyleRegular | italic ? FontStyleItalic : FontStyleRegular);
		Font        font(&fontFamily,(float) fontSize , fontStyle, UnitPixel);
		StringFormat stringFormat;
		stringFormat.GenericTypographic();
//		std::cout << "Em height " << fontFamily.GetEmHeight( FontStyleRegular | bold ? FontStyleBold : 0 | italic ? FontStyleItalic : 0) << std::endl;

		Bitmap BMPTemp(fontSize*3 + outLineSize + outLineSize2,fontSize*3 + outLineSize + outLineSize2,PixelFormat32bppARGB);
		SolidBrush  whitebrush(Color(255, 255, 255, 255));
		SolidBrush  blackbrush(Color(255, 0, 0, 0));
		SolidBrush  blackbrushAlpha(Color(0, 0, 0, 0));
		Graphics gTemp(&BMPTemp);

		gTemp.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		CLSID bmpClsid;
		GetEncoderClsid(L"image/bmp", &bmpClsid);

		UseAlphaChannel = GetAValue(bgColor) < 255;
//		UseAlphaChannel = alpha;
		u32 currentImage = 0;

		// create the font
		HFONT font_ = CreateFontW(
			-MulDiv(fontSize, GetDeviceCaps(dc, LOGPIXELSY), 72), 0,
			0,0,
			bold ? FW_BOLD : 0,
			italic, 0,0, charsets[charsetIndex], 0,0,
			aa ? ANTIALIASED_QUALITY : 0,
			0, FontNames[fontIndex].c_str() );

		if (!font_)
			return false;

		SelectObject(dc, font_);
//		Font        font(dc, font_);
		SetTextAlign (dc,TA_LEFT | TA_TOP | TA_NOUPDATECP);

		freeFontMemory();

		// get information about this font's unicode ranges.
		s32 size = GetFontUnicodeRanges( dc, 0);
		c8 *buf = new c8[size];
		LPGLYPHSET glyphs = (LPGLYPHSET)buf;

		GetFontUnicodeRanges( dc, glyphs);

		// s32 TotalCharCount = glyphs->cGlyphsSupported;

		s32 currentx=0, currenty=0, maxy=0, plusY = 0, MaxHeight = 0, MaxWidth = 0, charCount = 0;
		
		for (u32 testStatus=0; testStatus < 3; testStatus++)
		{
		Device->getLogger()->log("");
		core::stringc logmsg = "Phase ";
		logmsg += (testStatus + 1);
		logmsg += " of ";
		logmsg += (s32) 3;
		Device->getLogger()->log(logmsg.c_str());
		
		if (testStatus)
			Logmsg += L"\n";
		Logmsg += logmsg;
		Logmsg += L"\n";
		if (showProgress(&Logmsg)) return 0;

		if (testStatus >= 2) break;

		currentx=0;
		currenty=0;
		maxy=0;
		currentImage = 0;
		charCount = 0;

		for (u32 range=0; range < glyphs->cRanges; range++)
		{
			WCRANGE* current = &glyphs->ranges[range];

			//maxy=0;

			// loop through each glyph and write its size and position
			for (s32 ch=current->wcLow; ch< current->wcLow + current->cGlyphs; ch++)
			{
				charCount ++;

				if (!((charCount-1) % 300))
					{
						std::cout << "*"; 
						Logmsg += L"*";
						if (showProgress(&Logmsg)) return 0;
					}
				if (BitmapFontStyleFormat == 1)
					{
						ch = 31 + charCount;
					}

				wchar_t currentchar = ch;

				if (MAXchar && (ch > MAXchar))
					break;
//					continue;

				if ( IsDBCSLeadByte((BYTE) ch))
					continue; // surragate pairs unsupported

				// get the dimensions
				SIZE size;
				ABC abcX;
				ABC abcY;
//				GetTextExtentPoint32W(dc, &currentchar, 1, &size);
				SFontArea fa;
				fa.underhang = 0;
				fa.overhang  = 0;

//				RectF layoutRect((float)((fontSize*3 + outLineSize + outLineSize2) * 2 / 5),(float)((fontSize*3 + outLineSize + outLineSize2) / 3), 100.0f, 50.0f);
				RectF layoutRect((float)(int)((fontSize*3 + outLineSize + outLineSize2) * 2 / 5),(float)(int)((fontSize*3 + outLineSize + outLineSize2) / 3), 100.0f, 50.0f);
//				StringFormat format;
//				format.SetAlignment(StringAlignmentFar);
				RectF boundRect;
				// Measure the string.

//				Bitmap BMPTemp_(fontSize*3,fontSize*3,PixelFormat32bppARGB);
//				Graphics gTemp_(&BMPTemp_);

				if (aa)
					gTemp.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
				else
					gTemp.SetTextRenderingHint(TextRenderingHintSingleBitPerPixelGridFit);
				if (aaoutline)
					gTemp.SetSmoothingMode(SmoothingModeAntiAlias);
				else
					gTemp.SetSmoothingMode(SmoothingModeNone);

				gTemp.MeasureString(&currentchar, 1, &font, layoutRect, &stringFormat, &boundRect);
				// Draw a rectangle that represents the size of the string.
				size.cx = (LONG) boundRect.Width;
				size.cy = (LONG) boundRect.Height;

				if (BitmapFontStyleFormat != 1)
				{
					if (!boundRect.Width && !! !boundRect.Height)
						continue;
				}

				gTemp.SetCompositingMode(CompositingModeSourceCopy);
				gTemp.FillRectangle(&blackbrushAlpha,0,0, fontSize*3 + outLineSize + outLineSize2,fontSize*3 + outLineSize + outLineSize2);
				gTemp.SetCompositingMode(CompositingModeSourceOver);

				if ((outLineSize + outLineSize2 > 0) || textfillmode)
				{
					GraphicsPath path;
				    path.AddString(&currentchar, 1, &fontFamily, 
						fontStyle,(float) fontSize, boundRect, &stringFormat);
					if (outLineSize + outLineSize2 > 0)
					{
						Pen pen(Color(255,0,255,0),(float) outLineSize + outLineSize2 );
						pen.SetLineJoin(LineJoinRound);
						gTemp.DrawPath(&pen, &path);
					}
//					SolidBrush brush(Color(128,0,255));
					if (aa)
						gTemp.SetSmoothingMode(SmoothingModeAntiAlias);
					else
						gTemp.SetSmoothingMode(SmoothingModeNone);
					gTemp.FillPath(&whitebrush, &path);
				}
				else
					gTemp.DrawString(&currentchar, 1, &font, boundRect, &stringFormat, &whitebrush);

				if (ScanAndGetCharABCWidths(&BMPTemp, &boundRect, &abcX, &abcY,false))//ShowDebugInfo))
				{
					size.cx = abcX.abcB + space_between_letters;
					fa.underhang  = abcX.abcA;
					fa.overhang   = abcX.abcC;

//					if ((int) size.cy < (abcY.abcC + (int)outLineSize))
					if ((abcY.abcC + (int) plusY) > 0)
						size.cy = abcY.abcC + (plusY) + 1;
//					size.cy = abcY.abcA+abcY.abcB+abcY.abcC;
//					size.cy += abcY.abcA + abcY.abcC;
					if (testStatus == 0)
						{
						if (plusY < abcY.abcA*(-1))
							plusY = abcY.abcA*(-1);
						if (MaxHeight < size.cy)
							MaxHeight = size.cy;
						if (MaxWidth < size.cx)
							MaxWidth = size.cx;
						}


//					if (testStatus == 1)
//						rectF_.Height =+ (float)plusY; 
//*
					if ((testStatus == 1) && ShowDebugInfo)
						{
/*
				gTemp.SetCompositingMode(CompositingModeSourceCopy);
				gTemp.FillRectangle(&blackbrushAlpha,0,0, fontSize*3 + outLineSize + outLineSize2,fontSize*3 + outLineSize + outLineSize2);
				gTemp.SetCompositingMode(CompositingModeSourceOver);
/**/
							gTemp.SetSmoothingMode(SmoothingModeNone);

							RectF		rectF_ = boundRect;

							Pen pen(Color(255, 0, 0, 100));
							pen.SetDashStyle(DashStyleDot);

							gTemp.DrawRectangle(&pen, rectF_);
//							gTemp.DrawRectangle(&Pen(Color(255, 0, 0, 100)), rectF_);
							rectF_.X += (float) fa.underhang;//abcX.abcA;
//							rectF_.X = (float) fa.overhang;//abcX.abcA;
							rectF_.Width = (float) size.cx;
							rectF_.Height = (float) size.cy;
							rectF_.Y -= (float) plusY;

							pen.SetColor(Color(255, 100,100,0));
							gTemp.DrawRectangle(&pen, rectF_);
//							gTemp.DrawRectangle(&Pen(Color(255, 100,100,0)), rectF_);
/*
				if (aa)
					gTemp.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
				else
					gTemp.SetTextRenderingHint(TextRenderingHintSingleBitPerPixelGridFit);
				if (aaoutline)
					gTemp.SetSmoothingMode(SmoothingModeAntiAlias);
				else
					gTemp.SetSmoothingMode(SmoothingModeNone);

				if ((outLineSize + outLineSize2 > 0) || textfillmode)
				{
					GraphicsPath path;
				    path.AddString(&currentchar, 1, &fontFamily, 
						fontStyle,(float) fontSize, boundRect, &stringFormat);
					if (outLineSize + outLineSize2 > 0)
					{
						Pen pen(Color(255,255,0,0),(float) outLineSize + outLineSize2 );
						pen.SetLineJoin(LineJoinRound);
						gTemp.DrawPath(&pen, &path);
					}
//					SolidBrush brush(Color(128,0,255));
					if (aa)
						gTemp.SetSmoothingMode(SmoothingModeAntiAlias);
					else
						gTemp.SetSmoothingMode(SmoothingModeNone);
					gTemp.FillPath(&whitebrush, &path);
				}
				else
					gTemp.DrawString(&currentchar, 1, &font, boundRect, &stringFormat, &whitebrush);

/**/
							core::stringw FN = ".\\_letters\\TEST_SAVE_";
							FN += charCount;
							FN += "_";
							FN += ch;
//*
								FN += " X ";
								FN += abcX.abcA;
								FN += ".";
								FN += abcX.abcB;
								FN += ".";
								FN += abcX.abcC;
								FN += " Y ";
								FN += abcY.abcA;
								FN += ".";
								FN += abcY.abcB;
								FN += ".";
								FN += abcY.abcC;
/**/
//							FN += ".PNG";
							FN += ".BMP";
//							if ((range == 0) && (ch < 50))
//							if ((range == 0) && (ch < 50))
//							if ((charCount <= 400))
							if ((charCount < 0))
							{
								BMPTemp.Save(FN.c_str(),&bmpClsid);
//								BMPTemp.Save(FN.c_str(),&pngClsid);
								FN = "Char ";
//								FN += currentchar;
//								FN += " / ";
								FN += charCount;
								FN += "_";
								FN += ch;
								FN += " X ";
								FN += abcX.abcA;
								FN += ".";
								FN += abcX.abcB;
								FN += ".";
								FN += abcX.abcC;
								FN += " Y ";
								FN += abcY.abcA;
								FN += ".";
								FN += abcY.abcB;
								FN += ".";
								FN += abcY.abcC;
//								Device->getLogger()->log(FN.c_str());
							}
						}
/**/
				}
				else
					{
					size.cx += int((outLineSize + outLineSize2)/4) + space_between_letters;
					}
				if (testStatus == 1)
					{
					if (fixedHeight)
						size.cy = MaxHeight;
					if (fixedWidth)
						size.cx = MaxWidth;
					}
/*
				if (GetCharABCWidthsW(dc, currentchar, currentchar, &abc)) // for unicode fonts, get overhang, underhang, width
				{
					size.cx = abc.abcB;
//					fa.underhang  = abc.abcA;
//					fa.overhang   = abc.abcC;

					if (abc.abcB-abc.abcA+abc.abcC<1)
						continue; // nothing of width 0
				}
/**/
				if (BitmapFontStyleFormat != 1)
					{
					if (size.cy < 1)
						continue;
					if (size.cx < 1)
						continue;
					}
				else
					{
					if (size.cx < 1) 
						size.cx = 1;
					if (size.cy < 1) 
						size.cy = 1;
					}
				//GetGlyphOutline(dc, currentchar, GGO_METRICS, &gm, 0, 0, 0);

				//size.cx++; size.cy++;
//				size.cx += 10;
//				size.cy += 4;

				// wrap around?
				if (currentx + size.cx > (s32) textureWidth-1)
				{
					if (MaxImagesCount == (currentImage+1)) 
						{
							if ((u32)(currenty + 2*maxy) > textureHeight)
							{
								range = glyphs->cRanges;
								break;
							}
						};
					currenty += maxy;
					currentx = 0;
					if ((u32)(currenty + maxy) > textureHeight)
					{
						currentImage++; // increase Image count
						currenty=0;
					}
					maxy = 0;
				}
				// add this char dimension to the current map

				if (testStatus == 1)
					{
						fa.rectangle = core::rect<s32>(currentx, currenty, currentx + size.cx, currenty + size.cy);
						fa.sourceimage = currentImage;

						CharMap.insert(currentchar, Areas.size());
						Areas.push_back( fa );
					}
				currentx += size.cx +1;
				if (size.cy+1 > maxy)
					maxy = size.cy+1;
			}
		}
		}
		currenty += maxy;

		u32 lastTextureHeight = getTextureSizeFromSurfaceSize(currenty);

		// delete the glyph set
		delete [] buf;

		currentImages.set_used(currentImage+1);
		currentTextures.set_used(currentImage+1);
		for (u32 i=0; i<currentTextures.size(); ++i)
			currentTextures[i] = 0;
		for (u32 i=0; i<currentImages.size(); ++i)
			currentImages[i] = 0;

		Logmsg += L"Creating ";
		Logmsg += (s32) currentImages.size();
		Logmsg += L" image(s)... \n";

		for (currentImage=0; currentImage < currentImages.size(); ++currentImage)
		{
			core::stringc logmsg = "Creating image ";
			logmsg += (s32) (currentImage+1);
			logmsg += " of ";
			logmsg += (s32) currentImages.size();
			Device->getLogger()->log(logmsg.c_str());

			if (currentImage)
			{
				Logmsg += L" ";
				if (!(currentImage % 20) && !DefaultWinTextFormat.text_multiline)
					Logmsg += L"\n";
			}
			Logmsg += "";
			Logmsg += (s32) (currentImage+1);
			Logmsg += L"..";
			if (showProgress(&Logmsg)) return 0;
			// no need for a huge final texture
			u32 texHeight = textureHeight;
			if (currentImage == currentImages.size()-1 )
				texHeight = lastTextureHeight;

			// make a new bitmap
			Bitmap BMP(textureWidth,texHeight,PixelFormat32bppARGB);
			Graphics g(&BMP);
			g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

//			if (!UseAlphaChannel)
			{
				SolidBrush  sbrush(bgColor);
				g.FillRectangle(&sbrush,0,0, textureWidth, texHeight);
			}

			if (BitmapFontStyleFormat == 1)
				{
				BMP.SetPixel(0,0,dotBeginColor);
				BMP.SetPixel(1,0,dotEndColor);
				BMP.SetPixel(2,0,bgColor);
				}
			// draw the letters...

			// iterate through the tree
			core::map<wchar_t, u32>::Iterator it = CharMap.getIterator();
			while (!it.atEnd())
			{
				s32 currentArea = (*it).getValue();
				wchar_t wch = (*it).getKey();
				// sloppy but I couldnt be bothered rewriting it
//				if ((wch >= 159) && (wch <= 161))
				if (Areas[currentArea].sourceimage == currentImage)
				{
					// draw letter
					s32 sx = Areas[currentArea].rectangle.UpperLeftCorner.X - Areas[currentArea].underhang;
					PointF      pointF((float)sx, (float)Areas[currentArea].rectangle.UpperLeftCorner.Y);
//					u32 w = Areas[currentArea].rectangle.getWidth();
//					u32 h = Areas[currentArea].rectangle.getHeight();
					RectF		rectF((float)sx, (float)Areas[currentArea].rectangle.UpperLeftCorner.Y + (plusY), 
						(float) Areas[currentArea].rectangle.getWidth() + Areas[currentArea].underhang, (float) Areas[currentArea].rectangle.getHeight());
					RectF		rectF_((float)Areas[currentArea].rectangle.UpperLeftCorner.X, (float)Areas[currentArea].rectangle.UpperLeftCorner.Y, 
						(float) Areas[currentArea].rectangle.getWidth(), (float) Areas[currentArea].rectangle.getHeight());
					RectF		rectF_x2 = rectF_; rectF_x2.Height /= 2;
					 
					if (aa)
						g.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
					else
						g.SetTextRenderingHint(TextRenderingHintSingleBitPerPixelGridFit);
					if (aaoutline)
						g.SetSmoothingMode(SmoothingModeAntiAlias);
					else
						g.SetSmoothingMode(SmoothingModeNone);

					Brush * brush = 0;
					LinearGradientBrush * brush_ = 0;
					switch (textColorMode)
					{
						case 1 :
							brush_ = new LinearGradientBrush (
								rectF_,
								textColor2,   // red
								textColor1,
								LinearGradientModeVertical);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						case 2 :
							brush_ = new LinearGradientBrush (
								rectF_,
								textColor2,   // red
								textColor1,
								LinearGradientModeForwardDiagonal);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						case 3 :
							brush_ = new LinearGradientBrush (
								rectF_,
								textColor2,   // red
								textColor1,
								LinearGradientModeBackwardDiagonal);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						case 4 :
							brush_ = new LinearGradientBrush (
								rectF_x2,
								textColor2,   // red
								textColor1,
								LinearGradientModeVertical);  // blue
							brush_->SetWrapMode(WrapModeTileFlipXY);
							brush = brush_;
							break;
						default : 
							brush = new SolidBrush(textColor1); 
							break;
					}
/*
//					g.DrawString(&wch, 1, &font, pointF, &brush_);
//					format.SetAlignment(StringAlignmentCenter);
							LinearGradientBrush lbrush  (
								 rectF_,
								  Color(32, 0, 128, 192),   // red
								  Color(32, 0, 128, 192),
								  LinearGradientModeBackwardDiagonal);  // blue
		//						  LinearGradientModeForwardDiagonal);  // blue
		//						  LinearGradientModeHorizontal);  // blue
		//						  LinearGradientModeVertical);  // blue
							lbrush.SetWrapMode(WrapModeTileFlipXY);
/**/
					//draw OUTLINE 1 & 2
					if ((outLineSize + outLineSize2 > 0) || textfillmode)
					{
						GraphicsPath path;
						path.AddString(&wch, 1, &fontFamily, 
							fontStyle,(float) fontSize, rectF, &stringFormat);

						//draw OUTLINE 2
						if (outLineSize2)
						{
							Brush * pbrush = 0;
							LinearGradientBrush * pbrush_ = 0;
							switch (outline2ColorMode)
							{
								case 1 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 2 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeForwardDiagonal);  
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 3 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeBackwardDiagonal); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 4 :
									pbrush_ = new LinearGradientBrush (
										rectF_x2,
										outline2Color1,   
										outline2Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								default : 
									pbrush = new SolidBrush(outline2Color1); 
									break;
							}


							if (outline2Style == 1)
								for(int i=outLineSize+1; i<(int)outLineSize+(int)outLineSize2; ++i)
								{
									Pen pen(pbrush,(float) i);
									pen.SetLineJoin(LineJoinRound);
									g.DrawPath(&pen, &path);
								}
							else
							{
								Pen pen(pbrush,(float) outLineSize + outLineSize2);
								pen.SetLineJoin(LineJoinRound);
								g.DrawPath(&pen, &path);
							}
							delete pbrush;
						}

						//draw OUTLINE 1
						if (outLineSize)
						{
							Brush * pbrush = 0;
							LinearGradientBrush * pbrush_ = 0;
							switch (outline1ColorMode)
							{
								case 1 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 2 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeForwardDiagonal);  
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 3 :
									pbrush_ = new LinearGradientBrush (
										rectF_,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeBackwardDiagonal); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								case 4 :
									pbrush_ = new LinearGradientBrush (
										rectF_x2,
										outline1Color1,   
										outline1Color2,   
										LinearGradientModeVertical); 
									pbrush_->SetWrapMode(WrapModeTileFlipXY);
									pbrush = pbrush_;
									break;
								default : 
									pbrush = new SolidBrush(outline1Color1); 
									break;
							}


							if (outline1Style == 1)
								for(int i=1; i<(int)outLineSize; ++i)
								{
									Pen pen(pbrush,(float) i);
									pen.SetLineJoin(LineJoinRound);
									g.DrawPath(&pen, &path);
								}
							else
							{
								Pen pen(pbrush,(float) outLineSize);
								pen.SetLineJoin(LineJoinRound);
								g.DrawPath(&pen, &path);
							}
							delete pbrush;
						}
						// FILL TEXT
						if (aa)
							g.SetSmoothingMode(SmoothingModeAntiAlias);
						else
							g.SetSmoothingMode(SmoothingModeNone);
						if (!textoveroutline)
							{
								g.SetCompositingMode(CompositingModeSourceCopy);
							};
						if ((outLineSize + outLineSize2 > 0) && !textfillmode)
							{
								delete brush;
								brush = new SolidBrush(bgColor); 
							}
						g.FillPath(brush, &path);
						if (!textoveroutline)
							{
								g.SetCompositingMode(CompositingModeSourceOver);
							};
					}
					else
						g.DrawString(&wch, 1, &font, rectF, &stringFormat, brush);


					delete brush;


//					ABC abc;
//					ScanAndGetCharABCWidths_(&BMP,&rectF_, &abc);
					Areas[currentArea].underhang = 0;
					Areas[currentArea].overhang  = 0;

					if (ShowDebugInfo)
						if (currentArea % 2 == 0)
							g.DrawRectangle(&Pen(Color(64, 0, 0, 255)), rectF_);
						else
							g.DrawRectangle(&Pen(Color(64, 255, 255, 0)), rectF_);
					if (BitmapFontStyleFormat == 1)
						{
						BMP.SetPixel((int)rectF_.X,(int)rectF_.Y,dotBeginColor);
						BMP.SetPixel((int)rectF_.X+(int)rectF_.Width,(int)rectF_.Y+(int)rectF_.Height,dotEndColor);
						}
				}
				it++;
			}

			// copy the font bitmap into a new irrlicht image
			BitmapData* bitmapData = new BitmapData;
			Rect BMPrect(0, 0, textureWidth, texHeight);

			BMP.LockBits(
				&BMPrect,
				ImageLockModeRead,
				PixelFormat32bppARGB,
				bitmapData);

			UINT* pixels = (UINT*)bitmapData->Scan0;
			u32 * pixels32 = (u32*)bitmapData->Scan0;
			u32 *BMPData = new u32[bitmapData->Height*bitmapData->Width];

			if (bitmapData->Stride < 0)//down-top or top-down BMP orientation
				for(UINT row = 0; row < bitmapData->Height; ++row)
					memcpy(&BMPData[row * bitmapData->Width],&pixels32[(bitmapData->Height-row-1) * (bitmapData->Width)], bitmapData->Width*4);
			else
				for(UINT row = 0; row < bitmapData->Height; ++row)
					memcpy(&BMPData[row * bitmapData->Width],&pixels32[row * (bitmapData->Width)], bitmapData->Width*4);

			// make a texture from the image
			bool btemp = Device->getVideoDriver()->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);	
			currentImages[currentImage] = Device->getVideoDriver()->createImageFromData(video::ECF_A8R8G8B8, core::dimension2d<u32>(bitmapData->Width,bitmapData->Height), (void*)BMPData);
//			currentImages[currentImage] = 0;//Device->getVideoDriver()->createImageFromData(video::ECF_A8R8G8B8, core::dimension2d<u32>(bitmapData->Width,bitmapData->Height), (void*)BMPData);
			Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,btemp);

			delete [] BMPData;
			BMP.UnlockBits(bitmapData);
	

			if (currentImages[currentImage])
			{
				// add texture
				currentTextures[currentImage] = Device->getVideoDriver()->addTexture("GUIFontImage",currentImages[currentImage]);
				currentTextures[currentImage]->grab();
			}
			else
			{
				Device->getLogger()->log("Something went wrong, aborting.");
				// drop all images
				DeleteObject(font_);
				freeFontMemory();
				return false;
			}
		} // looping through each texture
		DeleteObject(font_);
//		freeFontMemory();
		return true;
	}

#else

	CFontTool::CFontTool(IrrlichtDevice *device) : FontSizes(fontsizes), Device(device), UseAlphaChannel(false)
	{
		if (!XftInitFtLibrary())
		{
			core::stringc logmsg = "XFT not found\n";
			Device->getLogger()->log(logmsg.c_str());
			exit(EXIT_FAILURE);
		}

		/* Get a list of the font foundries, storing them in a set to sort */
		std::set<core::stringw> foundries;
		Display* display = (Display*)Device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display;
		XftFontSet* fonts = XftListFonts(display, DefaultScreen(display), 0, XFT_FOUNDRY, 0);
		for (int i = 0; i < fonts->nfont; i++)
		{
			char *foundry;
			XftPatternGetString(fonts->fonts[i], XFT_FOUNDRY, 0, &foundry);
			core::stringw tmp(foundry);
			foundries.insert(tmp);
		}
		XftFontSetDestroy(fonts);

		/* Copy the sorted list into the array */
		CharSets.clear();
		for (std::set<core::stringw>::iterator i = foundries.begin(); i != foundries.end(); i++)
			CharSets.push_back((*i).c_str());
		selectCharSet(0);
	}

	/* Note: There must be some trick for using strings as pattern parameters to XftListFonts because
	no matter how I specify a string, I end up with an intermittent segfault.  Since XftFontList is
	just calling FcFontList, that's what I'll do too since that works OK */
	void CFontTool::selectCharSet(u32 currentCharSet)
	{
		/* Get a list of the font families, storing them in a set to sort */
		char foundry[256];
		sprintf(&foundry[0],"%ls",CharSets[currentCharSet].c_str());
		std::set<core::stringw> families;
		XftPattern *pattern = FcPatternCreate();
		XftPatternAddString(pattern, FC_FOUNDRY, &foundry[0]);
		XftObjectSet *objectset = FcObjectSetCreate();
		XftObjectSetAdd(objectset, XFT_FOUNDRY);
		XftObjectSetAdd(objectset, XFT_FAMILY);
		FcFontSet *fonts = FcFontList(NULL, pattern, objectset);

		for (int i = 0; i < fonts->nfont; i++)
		{
			char* ptr;
			XftPatternGetString(fonts->fonts[i], XFT_FAMILY, 0, &ptr);
			core::stringw family(ptr);
			families.insert(family);
		}
		XftPatternDestroy(pattern);
		FcObjectSetDestroy(objectset);

		/* Copy the sorted list into the array */
		FontNames.clear();
		for (std::set<core::stringw>::iterator i = families.begin(); i != families.end(); i++)
			FontNames.push_back((*i).c_str());
	}

	bool CFontTool::makeBitmapFont(u32 fontIndex, u32 charsetIndex, s32 fontSize, u32 textureWidth, u32 textureHeight, bool bold, bool italic, bool aa, bool alpha)
	{
		if (fontIndex >= FontNames.size() || charsetIndex >= CharSets.size() )
			return false;

		Display *display = (Display*) Device->getVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display;
		u32 screen = DefaultScreen(display);
		Window win = RootWindow(display, screen);
		Visual *visual = DefaultVisual(display, screen);
		UseAlphaChannel = alpha;
		u32 currentImage = 0;

		XftResult result;
		XftPattern *request = XftPatternCreate();
		char foundry[256], family[256];
		sprintf(&foundry[0],"%ls",CharSets[charsetIndex].c_str());
		sprintf(&family[0],"%ls",FontNames[fontIndex].c_str());
		XftPatternAddString(request, XFT_FOUNDRY, &foundry[0]);
		XftPatternAddString(request, XFT_FAMILY, &family[0]);
		XftPatternAddInteger(request, XFT_PIXEL_SIZE, fontSize);
		XftPatternAddInteger(request, XFT_WEIGHT, bold ? XFT_WEIGHT_BLACK : XFT_WEIGHT_LIGHT);
		XftPatternAddInteger(request, XFT_SLANT, italic ? XFT_SLANT_ITALIC : XFT_SLANT_ROMAN);
		XftPatternAddBool(request, XFT_ANTIALIAS, aa);

		/* Find the closest font that matches the user choices and open it and check if the returned
		font has anti aliasing enabled by default, even if it wasn't requested */
		FcBool aaEnabled;
		XftPattern *found = XftFontMatch(display, DefaultScreen(display), request, &result);
		XftPatternGetBool(found, XFT_ANTIALIAS, 0, &aaEnabled);
		aa = aaEnabled;
		XftFont *font = XftFontOpenPattern(display, found);

		// get rid of the current textures/images
		for (u32 i=0; i<currentTextures.size(); ++i)
			currentTextures[i]->drop();
		currentTextures.clear();
		for (u32 i=0; i<currentImages.size(); ++i)
			currentImages[i]->drop();
		currentImages.clear();
		CharMap.clear();
		Areas.clear();

		/* Calculate the max height of the font.  Annoyingly, it seems that the height property of the font
		is the maximum height of any single character, but a string of characters, aligned along their
		baselines, can exceed this figure.  Because I don't know any better way of doing it, I'm going to
		have to use the brute force method.

		Note: There will be a certain number of charters in a font, however they may not be grouped
		consecutively, and could in fact be spread out with many gaps */
		u32 maxY = 0;
		u32 charsFound = 0;
		for (FT_UInt charCode = 0; charsFound < FcCharSetCount(font->charset); charCode++)
		{
			if (!XftCharExists(display, font, charCode))
				continue;

			charsFound++;

			XGlyphInfo extents;
			XftTextExtents32(display, font, &charCode, 1, &extents);
			if ((extents.xOff <= 0) && (extents.height <= 0))
				continue;

			/* Calculate the width and height, adding 1 extra pixel if anti aliasing is enabled */
			u32 chWidth = extents.xOff + (aa ? 1 : 0);
			u32 chHeight = (font->ascent - extents.y + extents.height) +  (aa ? 1 : 0);
			if (chHeight > maxY)
				maxY = chHeight;

			/* Store the character details here */
			SFontArea fontArea;
			fontArea.rectangle = core::rect<s32>(0, 0, chWidth, chHeight);
			CharMap.insert(charCode, Areas.size());
			Areas.push_back(fontArea);
		}
		core::stringc logmsg = "Found ";
		logmsg += (s32) (CharMap.size() + 1);
		logmsg += " characters";
		Device->getLogger()->log(logmsg.c_str());

		/* Get the size of the chars and allocate them a position on a texture.  If the next character that
		is added would be outside the width or height of the texture, then a new texture is added */
		u32 currentX = 0, currentY = 0, rowY = 0;
		for (core::map<wchar_t, u32>::Iterator it = CharMap.getIterator(); !it.atEnd(); it++)
		{
			s32 currentArea = (*it).getValue();
			SFontArea *fontArea = &Areas[currentArea];
			u32 chWidth = fontArea->rectangle.LowerRightCorner.X;
			u32 chHeight = fontArea->rectangle.LowerRightCorner.Y;

			/* If the width of this char will exceed the textureWidth then start a new row */
			if ((currentX + chWidth) > textureWidth)
			{
				currentY += rowY;
				currentX = 0;

				/* If the new row added to the texture exceeds the textureHeight then start a new texture */
				if ((currentY + rowY) > textureHeight)
				{
					currentImage++;
					currentY = 0;
				}
				rowY = 0;
			}

			/* Update the area with the current x and y and texture */
			fontArea->rectangle = core::rect<s32>(currentX, currentY, currentX + chWidth, currentY + chHeight);
			fontArea->sourceimage = currentImage;
			currentX += chWidth + 1;
			if (chHeight + 1 > rowY)
				rowY = chHeight + 1;
		}

		/* The last row of chars and the last texture weren't accounted for in the loop, so add them here */
		currentY += rowY;
		u32 lastTextureHeight = getTextureSizeFromSurfaceSize(currentY);
		currentImages.set_used(currentImage + 1);
		currentTextures.set_used(currentImage + 1);

		/* Initialise colours */
		XftColor colFore, colBack;
		XRenderColor xFore = {0xffff, 0xffff, 0xffff, 0xffff};
		XRenderColor xBack = {0x0000, 0x0000, 0x0000, 0xffff};
		XftColorAllocValue(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &xFore, &colFore);
		XftColorAllocValue(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &xBack, &colBack);

		/* Create a pixmap that is large enough to hold any character in the font */
		Pixmap pixmap = XCreatePixmap(display, win, textureWidth, maxY, DefaultDepth(display, screen));
		XftDraw *draw = XftDrawCreate(display, pixmap, visual, DefaultColormap(display, screen));

		/* Render the chars */
		for (currentImage = 0; currentImage < currentImages.size(); ++currentImage)
		{
			core::stringc logmsg = "Creating image ";
			logmsg += (s32) (currentImage+1);
			logmsg += " of ";
			logmsg += (s32) currentImages.size();
			Device->getLogger()->log(logmsg.c_str());

			/* The last texture that is saved is vertically shrunk to fit the characters drawn on it */
			u32 texHeight = textureHeight;
			if (currentImage == currentImages.size() - 1)
				texHeight = lastTextureHeight;

			/* The texture that holds this "page" of characters */
			currentImages[currentImage] = Device->getVideoDriver()->createImage(video::ECF_A8R8G8B8, core::dimension2du(textureWidth, texHeight));
			currentImages[currentImage]->fill(video::SColor(alpha ? 0 : 255,0,0,0));

			for (core::map<wchar_t, u32>::Iterator it = CharMap.getIterator(); !it.atEnd(); it++)
			{
				FcChar32 wch = (*it).getKey();
				s32 currentArea = (*it).getValue();
				if (Areas[currentArea].sourceimage == currentImage)
				{
					SFontArea *fontArea = &Areas[currentArea];
					u32 chWidth = fontArea->rectangle.LowerRightCorner.X - fontArea->rectangle.UpperLeftCorner.X;
					u32 chHeight = fontArea->rectangle.LowerRightCorner.Y - fontArea->rectangle.UpperLeftCorner.Y;

					/* Draw the glyph onto the pixmap */
					XGlyphInfo extents;
					XftDrawRect(draw, &colBack, 0, 0, chWidth, chHeight);
					XftTextExtents32(display, font, &wch, 1, &extents);
					XftDrawString32(draw, &colFore, font, extents.x, extents.y, &wch, 1);

					/* Convert the pixmap into an image, then copy it onto the Irrlicht texture, pixel by pixel.
					There's bound to be a faster way, but this is adequate */
					u32 xDest = fontArea->rectangle.UpperLeftCorner.X;
					u32 yDest = fontArea->rectangle.UpperLeftCorner.Y + font->ascent - extents.y;
					XImage *image = XGetImage(display, pixmap, 0, 0, chWidth, chHeight, 0xffffff, XYPixmap);
					if (image)
					{
						for (u32 ySrc = 0; ySrc < chHeight; ySrc++)
							for (u32 xSrc = 0; xSrc < chWidth; xSrc++)
							{
								/* Get the pixel colour and break it down into rgb components */
								u32 col = XGetPixel(image, xSrc, ySrc);
								u32 a = 255;
								u32 r = col & visual->red_mask;
								u32 g = col & visual->green_mask;
								u32 b = col & visual->blue_mask;
								while (r > 0xff) r >>= 8;
								while (g > 0xff) g >>= 8;
								while (b > 0xff) b >>= 8;

								/* To make the background transparent, set the colour to 100% white and the alpha to
								the average of the three rgb colour components to maintain the anti-aliasing */
								if (alpha)
								{
									a = (r + g + b) / 3;
									r = 255;
									g = 255;
									b = 255;
								}
								currentImages[currentImage]->setPixel(xDest + xSrc,yDest + ySrc,video::SColor(a,r,g,b));
							}
						image->f.destroy_image(image);
					}
				}
			}

			/* Add the texture to the list */
			currentTextures[currentImage] = Device->getVideoDriver()->addTexture("GUIFontImage",currentImages[currentImage]);
			currentTextures[currentImage]->grab();
		}

		XftColorFree (display, visual, DefaultColormap(display, screen), &colFore);
		XftColorFree (display, visual, DefaultColormap(display, screen), &colBack);
		XftFontClose(display,font);
		XftPatternDestroy(request);
		XftDrawDestroy(draw);
		XFreePixmap(display, pixmap);
		return true;
	}
#endif

	CFontTool::~CFontTool()
	{
#ifdef _IRR_WINDOWS_
		// destroy display context
		if (dc)
			DeleteDC(dc);
		if (gdiplusToken)
			GdiplusShutdown(gdiplusToken);
#endif

		// drop textures+images
		for (u32 i=0; i<currentTextures.size(); ++i)
			currentTextures[i]->drop();
		currentTextures.clear();

		for (u32 i=0; i<currentImages.size(); ++i)
			currentImages[i]->drop();
		currentImages.clear();
	}

bool CFontTool::saveBitmapFont(const c8 *filename, const c8* format)
{
	if (currentImages.size() == 0)
	{
		Device->getLogger()->log("No image data to write, aborting.");
		return false;
	}

	core::stringc fn = filename;
	core::stringc imagename = filename;
	fn += ".xml";

	if (BitmapFontStyleFormat == 1) //1 .dot. style
		{
			imagename = filename;
			imagename += ".";
			imagename += format;
			Device->getVideoDriver()->writeImageToFile(currentImages[0],imagename.c_str());
			return true;
		}

	io::IXMLWriter *writer = Device->getFileSystem()->createXMLWriter(fn.c_str());

	// header and line breaks
	writer->writeXMLHeader();
	writer->writeLineBreak();

	// write information
	writer->writeElement(L"font", false, L"type", L"bitmap");
	writer->writeLineBreak();
	writer->writeLineBreak();

	// write images and link to them
	for (u32 i=0; i<currentImages.size(); ++i)
	{
		imagename = filename;
		imagename += (s32)i;
		imagename += ".";
		imagename += format;
		Device->getVideoDriver()->writeImageToFile(currentImages[i],imagename.c_str());

		writer->writeElement(L"Texture", true,
				L"index", core::stringw(i).c_str(),
				L"filename", core::stringw(imagename.c_str()).c_str(),
				L"hasAlpha", UseAlphaChannel ? L"true" : L"false");
		writer->writeLineBreak();
	}

	writer->writeLineBreak();

	// write each character
	core::map<wchar_t, u32>::Iterator it = CharMap.getIterator();
	while (!it.atEnd())
	{
		SFontArea &fa = Areas[(*it).getValue()];

		wchar_t c[2];
		c[0] = (*it).getKey();
		c[1] = L'\0';
		core::stringw area, under, over, image;
		area  = core::stringw(fa.rectangle.UpperLeftCorner.X);
		area += L", ";
		area += fa.rectangle.UpperLeftCorner.Y;
		area += L", ";
		area += fa.rectangle.LowerRightCorner.X;
		area += L", ";
		area += fa.rectangle.LowerRightCorner.Y;

		core::array<core::stringw> names;
		core::array<core::stringw> values;
		names.clear();
		values.clear();
		// char
		names.push_back(core::stringw(L"c"));
		values.push_back(core::stringw(c));
		// image number
		if (fa.sourceimage != 0)
		{
			image = core::stringw(fa.sourceimage);
			names.push_back(core::stringw(L"i"));
			values.push_back(image);
		}
		// rectangle
		names.push_back(core::stringw(L"r"));
		values.push_back(area);

		if (fa.underhang != 0)
		{
			under = core::stringw(fa.underhang);
			names.push_back(core::stringw(L"u"));
			values.push_back(under);
		}
		if (fa.overhang != 0)
		{
			over = core::stringw(fa.overhang);
			names.push_back(core::stringw(L"o"));
			values.push_back(over);
		}
		writer->writeElement(L"c", true, names, values);

		writer->writeLineBreak();
		it++;
	}

	writer->writeClosingTag(L"font");

	writer->drop();

	Device->getLogger()->log("Bitmap font saved.");

	return true;
}
