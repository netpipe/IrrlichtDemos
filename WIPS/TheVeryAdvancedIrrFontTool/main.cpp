/*
	Tool for creating Irrlicht bitmap+vector fonts,
	started by Gaz Davidson in December 2006

	Due to my laziness and Microsoft's unituitive API, surragate pairs and
	nonspacing diacritical marks are not supported!

	Linux bitmap font support added by Neil Burlock Oct 2008
	Note: Xft/Freetype2 is used to render the fonts under X11.  Anti-aliasing
	is controlled by the system and cannot be overriden by an application,
	so fonts that are rendered will be aliased or anti-aliased depending
	on the system that they are created on.

	THE VERY Advanced irrFontTool (ver 3.2b) by Nikolay Atanassov
	32-bit MS Win XP+
	TrueType (TTF) fonts to 32 bits (Display Device Indipendant) ARGB - BMP, PNG, TGA font (TTF2BMP,TTF2PNG,TTF2TGA)
	ARGB Colors
	Gradinet Colors
	Create Semi-transperant fonts
	Up to 2 outlines
	Outline Glow effect
	... and more...
*/


#include <irrlicht.h>
#include <iostream>

#include "CFontTool.h"
#include "CVectorFontTool.h"
#include "ITexture.h"

using namespace irr;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")


const s32 texturesizes[] = {128, 256, 512, 1024, 2048, 4096, 0};

const wchar_t *fileformats[]		 =  { L"bmp", L"ppm", 0 };  // bitmap font formats
const wchar_t *alphafileformats[]  =  { L"png", L"tga", 0 };  // bitmap font formats which support alpha channels
const wchar_t *vectorfileformats[] =  { L"xml", L"bin", 0 };  // file formats for vector fonts

const wchar_t *warntext = L"Legal Notice\n"
					L"------------\n\n"
					L"When making bitmap and vector fonts, you should consider the potential legal "
					L"issues with redistributing the fonts with your software; this tool basically "
					L"copies font data and some authors might not like this!\n"
					L"If you purchased fonts or they came with an application or your OS, you'll have"
					L"to check the license to see what restrictions are placed on making derivative works.\n\n"
					L"PD and the OFL\n"
					L"--------------\n\n"
					L"You'll find lots of fonts on the web listed as Public Domain, you can do what you like "
					L"with these.\n"
					L"Many fonts are released under the Open Font License, which is a 'viral' open source "
					L"license like the GPL. It's worth reading the license here: http://scripts.sil.org/OFL\n"
					L"The most important restrictions are- you must include the original font's license, you "
					L"can't stop your users from using or distributing the font, the font must have a "
					L"different name the original.\n\n"
					L"Some free fonts can be found here- www.openfontlibrary.org\n"
					L"http://savannah.nongnu.org/projects/freefont/";

wchar_t *helptext = L"This tool creates bitmap fonts for the Irrlicht Engine\n\n"

					L"First select a character encoding from the list, then choose the font, "
					L"size, and whether you'd like bold, italic, antialiasing and an alpha channel. "
					L"In Windows, antialiasing will be ignored for small fonts\n\n"

					L"Then select a texture width and height. If the output exceeds this then more than "
					L"one image will be created. You can use the scrollbar at the top of the screen to "
					L"preview them. Most modern graphics cards will support up to 2048x2048, "
					L"keep in mind that more images means worse performance when drawing text!\n\n"

					L"If you want a vector font rather than a bitmap font, check the vector box. "
					L"Vector fonts are stored in system memory while bitmap fonts are in video ram\n\n"

					L"Click create to preview your font, this may take lots of time and memory "
					L"when making a font with a lot of characters, please be patient!\n\n"

					L"Now you're ready to give your font a name, select a format and click save.\n\n"
					L"To load your font in Irrlicht, simply use env->getFont(\"Myfont.xml\");\n\n"

					L"That's all, have fun :-)";

#ifdef _IRR_WINDOWS
					wchar_t *completeText = L"Font created"
#else
					wchar_t *completeText = L"Font created\n\n"
							L"Please note that anti-aliasing under X11 is controlled by the system "
							L"configuration, so if your system is set to use anti-aliasing, then so "
							L"will any fonts you create with FontTool";
#endif

enum MYGUI
{
	MYGUI_WIN = 500,
	MYGUI_WIN_WORK = 555,
	MYGUI_WIN_PREVIEW,
	MYGUI_IMAGE_PREVIEW,
	MYGUI_IMAGE_STATUS = 101010,

	MYGUI_CHARSET  = 100,
	MYGUI_FONTNAME,
	MYGUI_SIZE,
	MYGUI_TEXWIDTH,
	MYGUI_TEXHEIGHT,
	MYGUI_BOLD,
	MYGUI_ITALIC,
	MYGUI_ANTIALIAS,
	MYGUI_ALPHA,
	MYGUI_FILL,
	MYGUI_VECTOR,
	MYGUI_FILENAME,
	MYGUI_FORMAT,
	MYGUI_CREATE,
	MYGUI_SAVE,
	MYGUI_IMAGE,
	MYGUI_CURRENTIMAGE,
	MYGUI_HELPBUTTON,

	MYGUI_ZOOM,
	MYGUI_DEBUGINFO,
	MYGUI_TESTMODE,
	MYGUI_PREVIEW,
	MYGUI_BITMAPSTYLEFORMAT,
	MYGUI_ABORT,

	MYGUI_OUTLINEANTIALIAS,
	MYGUI_TEXTOVEROUTLINE,
	MYGUI_SPACE,
	MYGUI_TEXTCOLORMODE,
	MYGUI_TEXTCOLOR1,
	MYGUI_TEXTCOLOR2,
	MYGUI_OUTLINE1,
	MYGUI_OUTLINE1COLORMODE,
	MYGUI_OUTLINE1COLOR1,
	MYGUI_OUTLINE1COLOR2,
	MYGUI_OUTLINE1SIZE,
	MYGUI_OUTLINE1STYLE,
	MYGUI_OUTLINE2,
	MYGUI_OUTLINE2COLORMODE,
	MYGUI_OUTLINE2COLOR1,
	MYGUI_OUTLINE2COLOR2,
	MYGUI_OUTLINE2SIZE,
	MYGUI_OUTLINE2STYLE,
	
	MYGUI_BGCOLOR
};

IGUIWindow *winPreview = 0;
gui::IGUIImage *imgPreview = 0;

#define ABGR(a,r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))

#define GetAValue(x)          (((x) >> 24) & 0xFF)
//#define GetRValue(x)          (((x) >> 16) & 0xFF)
//#define GetGValue(x)          (((x) >> 8) & 0xFF)
//#define GetBValue(x)          ((x) & 0xFF)

bool is255(int * argb)
{
	return ((*argb > 0) && (*argb <= 255)); 
}

bool init255(int * argb)
{
	if (is255(argb)) return true;
	if (*argb < 0) *argb = 0;
	if (*argb > 255) *argb = 255;
	return false;
}

bool readColor(IGUIEditBox *edt, u32 * Color, bool readOnly = false)
{
	*Color = 0;
	if (!edt)
		return false;
	char  buffer[200];
	core::stringc text = edt->getText();
	int a = 0; int r = 0; int g = 0; int b = 0; int itmp = 0;
	int readOK = 0;
	readOK = sscanf_s (text.c_str(),"%d %d %d %d %d",&a,&r,&g,&b,&itmp);
	if (readOK >= 4)
	{
		bool isOK = init255(&a);
		isOK = init255(&r) && isOK;
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d %d %d %d",a,r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (readOK == 3)
	{
		b = g;
		g = r; 
		r = a; 
		a = 255;
		bool isOK = init255(&r);
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d %d %d",r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (readOK == 2)
	{
		b = 0;
		g = r; 
		r = a; 
		a = 255;
		bool isOK = init255(&r);
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d %d %d",r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (readOK == 1)
	{
		b = 0;
		g = 0; 
		r = a; 
		a = 255;
		bool isOK = init255(&r);
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d %d %d",r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	readOK = sscanf_s (text.c_str(),"%d,%d,%d,%d,%d",&a,&r,&g,&b,&itmp);
	if (readOK >= 4)
	{
		bool isOK = init255(&a);
		isOK = init255(&r) && isOK;
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d,%d,%d,%d",a,r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (readOK == 3)
	{
		b = g;
		g = r; 
		r = a; 
		a = 255;
		bool isOK = init255(&r);
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d,%d,%d",r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (readOK == 2)
	{
		b = 0;
		g = r; 
		r = a; 
		a = 255;
		bool isOK = init255(&r);
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d,%d,%d",r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (readOK == 1)
	{
		b = 0;
		g = 0; 
		r = a; 
		a = 255;
		bool isOK = init255(&r);
		isOK = init255(&g) && isOK;
		isOK = init255(&b) && isOK;
		*Color = ABGR(a,b,g,r);
		if (!readOnly)
		if (sprintf_s(buffer,200,"%d,%d,%d",r,g,b) > 0)
		{
			text = buffer;
			edt->setText(core::stringw(buffer).c_str());
		}
		return true;
	}
	if (!readOnly)
	edt->setText(L"0 0 0");
	return false;
}

void MakeCharPreview(IrrlichtDevice* Device, CFontTool* FontTool)
{
		IGUIEnvironment* env = Device->getGUIEnvironment();

					// create the font with the params
					IGUIComboBox* cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_CHARSET, true);
					int charset = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FONTNAME,true);
					int fontname = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_SIZE,true);
					int fontsize = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXWIDTH,true);
					int texwidth = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXHEIGHT,true);
					int texheight = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_BITMAPSTYLEFORMAT,true);
					s32 StyleFormat = cbo->getSelected();

					IGUICheckBox* chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_BOLD,true);
					bool bold = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ITALIC,true);
					bool italic = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ALPHA,true);
					bool alpha = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FILL,true);
					bool textfill = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ANTIALIAS,true);
					bool aa = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINEANTIALIAS,true);
					bool aaoutline = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTOVEROUTLINE,true);
					bool textoveroutline = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_DEBUGINFO,true);
					bool showdebuginfo = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TESTMODE,true);
					bool testmode = chk->isChecked();

					// vector fonts disabled
					//chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_VECTOR,true);
					bool vec = false;//chk->isChecked();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_SPACE,true);
					u32 space_between_letters = cbo->getSelected();

					// Background
					u32 bgColor = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_BGCOLOR,true),&bgColor,true);

					//TEXT colors
					u32 textColor1 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTCOLOR1,true),&textColor1,true);

					u32 textColor2 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTCOLOR2,true),&textColor2,true);

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTCOLORMODE,true);
					u32 textColorMode = cbo->getSelected();

					//outline1 colors
					u32 outline1Color1 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1COLOR1,true),&outline1Color1,true);

					u32 outline1Color2 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1COLOR2,true),&outline1Color2,true);

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1COLORMODE,true);
					u32 outline1ColorMode = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1STYLE,true);
					u32 outline1Style = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1SIZE,true);
					u32 outline1Size = cbo->getSelected();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1,true);
					if (!chk->isChecked())
						outline1Size = 0;

					//outline2 colors
					u32 outline2Color1 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2COLOR1,true),&outline2Color1,true);

					u32 outline2Color2 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2COLOR2,true),&outline2Color2,true);

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2COLORMODE,true);
					u32 outline2ColorMode = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2STYLE,true);
					u32 outline2Style = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2SIZE,true);
					u32 outline2Size = cbo->getSelected();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2,true);
					if (!chk->isChecked())
						outline2Size = 0;

					IGUIScrollBar* scrl = (IGUIScrollBar*)env->getRootGUIElement()->getElementFromId(MYGUI_CURRENTIMAGE,true);
//					scrl->setEnabled(false);
					IGUIWindow *win = (IGUIWindow*)env->getRootGUIElement()->getElementFromId(MYGUI_WIN,true);
					IGUIWindow *winwork = (IGUIWindow*)env->getRootGUIElement()->getElementFromId(MYGUI_WIN_WORK,true);
					win->setEnabled(false);
					winwork->setVisible(true);
					winwork->setEnabled(true);
					env->getRootGUIElement()->bringToFront(winwork);

					//JUST MAKE IT
					FontTool->makePreviewFont(fontname, L'A', FontTool->FontSizes[fontsize], 130, 130, bold, italic, aa, alpha,
						textfill, aaoutline, textoveroutline,
						space_between_letters,
						bgColor,textColorMode,textColor1,textColor2,
						outline1ColorMode, outline1Color1, outline1Color2, outline1Style, outline1Size,
						outline2ColorMode, outline2Color1, outline2Color2, outline2Style, outline2Size,
						testmode);

					win->setEnabled(true);
					winwork->setVisible(false);

					scrl->setMax(FontTool->currentTextures.size() == 0 ? 0 : FontTool->currentTextures.size()-1);

					if (FontTool->previewTexture > 0)
					{
//						IGUIImage* img = (IGUIImage*)env->getRootGUIElement()->getElementFromId(MYGUI_IMAGE_PREVIEW,true);
//						img->setVisible(false);
						imgPreview->setImage(FontTool->previewTexture);
					}
}

void ZOOMit(IrrlichtDevice* Device, CFontTool* FontTool)
	{
		IGUIEnvironment* env = Device->getGUIEnvironment();

		IGUIComboBox* cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ZOOM,true);
		float zoomTO = (float) cbo->getSelected();

		IGUIScrollBar* scrl = (IGUIScrollBar*)env->getRootGUIElement()->getElementFromId(MYGUI_CURRENTIMAGE,true);
		s32 pos = scrl->getPos();

		zoomTO = ((zoomTO+1)*50)/100;

		if (FontTool->currentTextures.size())
		{
			IGUIImage* img = (IGUIImage*)env->getRootGUIElement()->getElementFromId(MYGUI_IMAGE,true);
			core::rect<s32> imgrect;
			imgrect = img->getRelativePosition();
			imgrect.LowerRightCorner.X = imgrect.UpperLeftCorner.X + s32((float)FontTool->currentTextures[pos]->getSize().Width*zoomTO);
			imgrect.LowerRightCorner.Y = imgrect.UpperLeftCorner.Y + s32((float)FontTool->currentTextures[pos]->getSize().Height*zoomTO);
			img->setRelativePosition(imgrect);
			img->setScaleImage(true);
		}
	if (FontTool->previewTexture)
		if (imgPreview->isVisible())
		{
			IGUIImage* img = imgPreview;
			core::rect<s32> imgrect;
			imgrect = img->getRelativePosition();
//			imgrect.LowerRightCorner.X = imgrect.UpperLeftCorner.X + s32((float)FontTool->previewTexture->getSize().Width*zoomTO);
//			imgrect.LowerRightCorner.Y = imgrect.UpperLeftCorner.Y + s32((float)FontTool->previewTexture->getSize().Height*zoomTO);
			imgrect.LowerRightCorner.X = imgrect.UpperLeftCorner.X + s32((float)FontTool->previewTexture->getSize().Width*zoomTO);
			imgrect.LowerRightCorner.Y = imgrect.UpperLeftCorner.Y + s32((float)FontTool->previewTexture->getSize().Height*zoomTO);
			img->setRelativePosition(imgrect);
			img->setScaleImage(true);
			imgrect = winPreview->getRelativePosition();
			s32 add = s32((float)FontTool->previewTextureWidth*zoomTO);
//			s32 add = s32((float)FontTool->previewTexture->getSize().Width*zoomTO);
			if (add < 70) add = 70;
			imgrect.LowerRightCorner.X = imgrect.UpperLeftCorner.X + add + 2*5;
//			add = s32((float)FontTool->previewTexture->getSize().Height*zoomTO);
			add = s32((float)FontTool->previewTextureHeight*zoomTO);
			if (add < 60) add = 60;
			imgrect.LowerRightCorner.Y = imgrect.UpperLeftCorner.Y + add + 2*5 + 20;
			winPreview->setRelativePosition(imgrect);
		}
	}

void CharPreview(IrrlichtDevice* Device, CFontTool* FontTool)
{
	if (imgPreview->isVisible())
	{
		MakeCharPreview(Device,FontTool);
		ZOOMit(Device,FontTool);
	}
}

// event reciever
class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(IrrlichtDevice* device, CFontTool*& fonttool, CVectorFontTool* &vectool) :
		Device(device), FontTool(fonttool), VecTool(vectool)
	{
		device->setEventReceiver(this);
	}

	virtual bool OnEvent(const SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			IGUIEnvironment* env = Device->getGUIEnvironment();
			IGUIWindow *thewin = (IGUIWindow*)env->getRootGUIElement()->getElementFromId(MYGUI_WIN,true);
			s32 id = event.GUIEvent.Caller->getID();
			if (!thewin->isEnabled())
			{
//				if (!((id == MYGUI_WIN_WORK) || (id == MYGUI_ABORT)))
//					return true;
				switch(event.GUIEvent.EventType)
				{
				case EGET_BUTTON_CLICKED:

					if (id == MYGUI_ABORT)
					{
						FontTool->AbortIT = true;
					}
					break;
				}
//				return false;
			}

			if (thewin->isEnabled())
			switch(event.GUIEvent.EventType)
			{
			case EGET_EDITBOX_CHANGED:
				CharPreview(Device,FontTool);
				break;
			case EGET_SCROLL_BAR_CHANGED:
				if (id == MYGUI_CURRENTIMAGE)
				{
					IGUIImage* img = (IGUIImage*)env->getRootGUIElement()->getElementFromId(MYGUI_IMAGE,true);
					s32 i = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					img->setImage(FontTool->currentTextures[i]);

					ZOOMit(Device,FontTool);
					return true;
				}
				break;
			case EGET_COMBO_BOX_CHANGED:
				if (id == MYGUI_CHARSET)
				{
					IGUIComboBox* cbo = (IGUIComboBox*)event.GUIEvent.Caller;
					FontTool->selectCharSet(cbo->getSelected());
					// rebuild font list
					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FONTNAME,true);
					cbo->clear();
					for (u32 i=0; i < FontTool->FontNames.size(); ++i)
						cbo->addItem(FontTool->FontNames[i].c_str());
					s32 arial = FontTool->FontNames.binary_search(L"Arial");
					if (arial >= 0)
						cbo->setSelected(arial);
					CharPreview(Device,FontTool);
					return true;
				}
				else
				if (id == MYGUI_ZOOM)
				{
					ZOOMit(Device,FontTool);
					return true;
				}
				CharPreview(Device,FontTool);
				return true;
				break;
			case EGET_CHECKBOX_CHANGED:
				if (id == MYGUI_VECTOR)
				{
					IGUICheckBox* chk = (IGUICheckBox*)event.GUIEvent.Caller;

					IGUIComboBox *cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FORMAT,true);
					cbo->clear();

					if (chk->isChecked() && VecTool)
					{
						// vector formats
						for (s32 i=0; fileformats[i] != 0; ++i)
							cbo->addItem( core::stringw(vectorfileformats[i]).c_str());

					}
					else
					{

						// bitmap formats
						if (!FontTool->UseAlphaChannel)
						{
							// add non-alpha formats
							for (s32 i=0; fileformats[i] != 0; ++i)
								cbo->addItem( core::stringw(fileformats[i]).c_str());
						}
						// add formats which support alpha
						for (s32 i=0; alphafileformats[i] != 0; ++i)
							cbo->addItem( core::stringw(alphafileformats[i]).c_str());
					}

				}
				if (id == MYGUI_PREVIEW)
				{
					IGUICheckBox* chk = (IGUICheckBox*)event.GUIEvent.Caller;
					imgPreview->setVisible(chk->isChecked());
					winPreview->setVisible(chk->isChecked());
				}
				CharPreview(Device,FontTool);
				break;

			case EGET_BUTTON_CLICKED:

				if (id == MYGUI_CREATE)
				{
					// create the font with the params
					IGUIComboBox* cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_CHARSET, true);
					int charset = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FONTNAME,true);
					int fontname = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_SIZE,true);
					int fontsize = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXWIDTH,true);
					int texwidth = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXHEIGHT,true);
					int texheight = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_BITMAPSTYLEFORMAT,true);
					s32 StyleFormat = cbo->getSelected();

					IGUICheckBox* chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_BOLD,true);
					bool bold = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ITALIC,true);
					bool italic = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ALPHA,true);
					bool alpha = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FILL,true);
					bool textfill = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_ANTIALIAS,true);
					bool aa = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINEANTIALIAS,true);
					bool aaoutline = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTOVEROUTLINE,true);
					bool textoveroutline = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_DEBUGINFO,true);
					bool showdebuginfo = chk->isChecked();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TESTMODE,true);
					bool testmode = chk->isChecked();

					// vector fonts disabled
					//chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_VECTOR,true);
					bool vec = false;//chk->isChecked();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_SPACE,true);
					u32 space_between_letters = cbo->getSelected();

					// Background
					u32 bgColor = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_BGCOLOR,true),&bgColor);

					//TEXT colors
					u32 textColor1 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTCOLOR1,true),&textColor1);

					u32 textColor2 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTCOLOR2,true),&textColor2);

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_TEXTCOLORMODE,true);
					u32 textColorMode = cbo->getSelected();

					//outline1 colors
					u32 outline1Color1 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1COLOR1,true),&outline1Color1);

					u32 outline1Color2 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1COLOR2,true),&outline1Color2);

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1COLORMODE,true);
					u32 outline1ColorMode = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1STYLE,true);
					u32 outline1Style = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1SIZE,true);
					u32 outline1Size = cbo->getSelected();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE1,true);
					if (!chk->isChecked())
						outline1Size = 0;

					//outline2 colors
					u32 outline2Color1 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2COLOR1,true),&outline2Color1);

					u32 outline2Color2 = 0;
					readColor((IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2COLOR2,true),&outline2Color2);

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2COLORMODE,true);
					u32 outline2ColorMode = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2STYLE,true);
					u32 outline2Style = cbo->getSelected();

					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2SIZE,true);
					u32 outline2Size = cbo->getSelected();

					chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_OUTLINE2,true);
					if (!chk->isChecked())
						outline2Size = 0;

					IGUIScrollBar* scrl = (IGUIScrollBar*)env->getRootGUIElement()->getElementFromId(MYGUI_CURRENTIMAGE,true);
					scrl->setEnabled(false);
					IGUIWindow *win = (IGUIWindow*)env->getRootGUIElement()->getElementFromId(MYGUI_WIN,true);
					IGUIWindow *winwork = (IGUIWindow*)env->getRootGUIElement()->getElementFromId(MYGUI_WIN_WORK,true);
					win->setEnabled(false);
					winwork->setVisible(true);
					winwork->setEnabled(true);
					env->getRootGUIElement()->bringToFront(winwork);

					//JUST MAKE IT
					FontTool->makeBitmapFont(StyleFormat, fontname, charset, FontTool->FontSizes[fontsize], texturesizes[texwidth], texturesizes[texheight], bold, italic, aa, alpha,
						textfill, aaoutline, textoveroutline,
						space_between_letters,bgColor,textColorMode,textColor1,textColor2,
						outline1ColorMode, outline1Color1, outline1Color2, outline1Style, outline1Size,
						outline2ColorMode, outline2Color1, outline2Color2, outline2Style, outline2Size,
						testmode, showdebuginfo);

					win->setEnabled(true);
					winwork->setVisible(false);

					scrl->setMax(FontTool->currentTextures.size() == 0 ? 0 : FontTool->currentTextures.size()-1);

					if (FontTool->currentTextures.size() > 0)
					{
						IGUIImage* img = (IGUIImage*)env->getRootGUIElement()->getElementFromId(MYGUI_IMAGE_STATUS,true);
						img->setVisible(false);

						scrl->setEnabled(true);
						img = (IGUIImage*)env->getRootGUIElement()->getElementFromId(MYGUI_IMAGE,true);
						img->setImage(FontTool->currentTextures[0]);
						scrl->setPos(0);
						ZOOMit(Device,FontTool);
					}

					// make sure users pick a file format that supports alpha channel
					cbo = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FORMAT,true);
					s32 oldselection = cbo->getSelected();
					cbo->clear();

					if (vec)
					{
						// add vector formats
						for (s32 i=0; fileformats[i] != 0; ++i)
							cbo->addItem( core::stringw(vectorfileformats[i]).c_str());
					}
					else
					{
						if (!FontTool->UseAlphaChannel)
						{
							// add non-alpha formats
							for (s32 i=0; fileformats[i] != 0; ++i)
								cbo->addItem( core::stringw(fileformats[i]).c_str());
						}
						// add formats which support alpha
						for (s32 i=0; alphafileformats[i] != 0; ++i)
							cbo->addItem( core::stringw(alphafileformats[i]).c_str());
						cbo->setSelected(oldselection);
					}
					if (VecTool)
					{
						delete VecTool;
						VecTool = 0;
					}
					if (vec)
					{
						VecTool = new CVectorFontTool(FontTool);
					}

					/* Message box letting the user know the process is complete */
//					env->addMessageBox(L"Create", completeText);
					return true;
				}

				if (id == MYGUI_SAVE)
				{
					IGUIEditBox *edt  = (IGUIEditBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FILENAME,true);
					core::stringc name = edt->getText();

					IGUIComboBox *fmt  = (IGUIComboBox*)env->getRootGUIElement()->getElementFromId(MYGUI_FORMAT,true);
					core::stringc format = fmt->getItem(fmt->getSelected());

					// vector fonts disabled
					IGUICheckBox *chk = (IGUICheckBox*)env->getRootGUIElement()->getElementFromId(MYGUI_VECTOR,true);
					bool vec = false; // chk->isChecked();

					if (vec && VecTool)
						VecTool->saveVectorFont(name.c_str(), format.c_str());
					else
						FontTool->saveBitmapFont(name.c_str(), format.c_str());

					return true;
				}

				if (id == MYGUI_HELPBUTTON)
				{
					env->addMessageBox(L"Irrlicht Unicode Font Tool", helptext);
					return true;
				}

				break;
			}
		}

		return false;
	}

	IrrlichtDevice*	 Device;
	CFontTool*       FontTool;
	CVectorFontTool* VecTool;

};

void createGUI(IrrlichtDevice* device, CFontTool* fc)
{
	gui::IGUIEnvironment *env = device->getGUIEnvironment();

	// change transparency of skin
	for (s32 i=0; i<gui::EGDC_COUNT ; ++i)
	{
		video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
	}

	IGUIWindow *winwork = env->addWindow( core::rect<s32>(10,10,100,500), false, L"Work in progress...",0,MYGUI_WIN_WORK);
	winwork->getCloseButton()->setVisible(false);

	s32 xs=10,xp=xs, yp=30, h=20;

	xp = xs+20;
//	yp += (s32)(h*1.2f);

	// create button
	env->addButton( core::rect<s32>(xp,yp,xp+50,yp+h),winwork, MYGUI_ABORT, L"A B O R T");

	yp += (s32)(h*2.2f);

	winwork->setRelativePosition( core::rect<s32>(10,10,120,yp));

	IGUIWindow *winpreview = env->addWindow( core::rect<s32>(10,10,130,130), false, L"Preview",0,MYGUI_WIN_PREVIEW);
	winpreview->getCloseButton()->setVisible(false);
	winPreview = winpreview;

	// Preview image
	gui::IGUIImage *imgpreview = env->addImage(0, core::position2d<s32>(0,0), true,winpreview, MYGUI_IMAGE_PREVIEW);
	imgpreview->setRelativePosition(core::rect<s32>(5,25,130,130));
	imgPreview = imgpreview;

	IGUIWindow *win = env->addWindow( core::rect<s32>(10,10,200,200), false, L"Font Creator",0,MYGUI_WIN);
	win->getCloseButton()->setVisible(false);

	xs=10;xp=xs; yp=30; h=20;

	env->addStaticText(L"Charset", core::rect<s32>(xp,yp,50,yp+h),false,false, win);

	xp+=60;

	// charset combo
	gui::IGUIComboBox* cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_CHARSET);
	for (u32 i=0; i < fc->CharSets.size(); ++i)
		cbo->addItem(fc->CharSets[i].c_str());

	yp += (s32)(h*1.5f);
	xp = xs;

	env->addStaticText(L"Font", core::rect<s32>(xp,yp,50,yp+h),false,false, win);

	xp+=60;

	// font name combo
	cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_FONTNAME);
	for (u32 i=0; i < fc->FontNames.size(); ++i)
		cbo->addItem(fc->FontNames[i].c_str());
	s32 arial = fc->FontNames.binary_search(L"Arial");
	if (arial >= 0)
		cbo->setSelected(arial);

	yp += (s32)(h*1.5f);
	xp = xs;

	env->addStaticText(L"Size / Space", core::rect<s32>(xp,yp,60,yp+h),false,false, win);

	xp += 60;

	// font size combo
	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_SIZE);
	for (s32 i=0; fc->FontSizes[i] != 0; ++i)
		cbo->addItem( ((core::stringw(fc->FontSizes[i])) + L"px").c_str());
	cbo->setSelected(10);
//	cbo->setSelected(13);

	xp += 60;
	// font space combo
	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_SPACE);
	for (s32 i=0; i <= 30; ++i)
	{
		core::stringw s = "";
		s += i;
		cbo->addItem(s.c_str());
	}
	cbo->setSelected(1);

	xp = xs;
	yp += (s32)(h*1.5f);

	// bold checkbox
	env->addCheckBox(false, core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_BOLD, L"Bold");

	xp += 45;

	// italic checkbox
	env->addCheckBox(false, core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_ITALIC, L"Italic");

	xp += 45;

	// AA checkbox
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_ANTIALIAS, L"AA");

	xp +=40;

	// Text fill mode checkbox
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_FILL, L"fill");

	xp = xs;
	yp += (s32)(h*1.2f);

	// outline AA checkbox
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+70,yp+h),win, MYGUI_OUTLINEANTIALIAS, L"Outline AA");

	xp += 70;

	// Text Over Outline checkbox
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+130,yp+h),win, MYGUI_TEXTOVEROUTLINE, L"TEXT Over outline");

	xp = xs;
	yp += (s32)(h*1.5f);
	/*
	// vector fonts can't be loaded yet
	env->addCheckBox(false, core::rect<s32>(xp,yp,xp+200,yp+h),win, MYGUI_VECTOR, L"Vector Font");
	*/

	// background color
	env->addStaticText(L"BG Color", core::rect<s32>(xp,yp,180,yp+h),false,false, win);

	xp+=60;

	env->addEditBox(L"0 0 0",core::rect<s32>(xp,yp,xp+60,yp+h), true, win, MYGUI_BGCOLOR);

	xp+=60;
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_ALPHA, L"Alpha");

	yp += (s32)(h*1.5f);
	xp = xs;

	// TEXT colos 
	env->addStaticText(L"Text COLOR", core::rect<s32>(xp,yp,180,yp+h),false,false, win);

	xp+=60;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_TEXTCOLORMODE);
	cbo->addItem(L"Solid");
	cbo->addItem(L"Gradient Horizontal");
	cbo->addItem(L"Gradient FW Diagonal");
	cbo->addItem(L"Gradient BW Diagonal");
	cbo->addItem(L"Gradient Horizontal 2");

	yp += (s32)(h*1.2f);
	xp = xs;

	env->addEditBox(L"255 255 255 255",core::rect<s32>(xp,yp,90,yp+h), true, win, MYGUI_TEXTCOLOR1);

	xp+=90;

	env->addEditBox(L"255 255 128 0",core::rect<s32>(xp,yp,180,yp+h), true, win, MYGUI_TEXTCOLOR2);

	yp += (s32)(h*1.5f);
	xp = xs;

	// TEXT outline 1 
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+80,yp+h),win, MYGUI_OUTLINE1, L"Outline 1");

	xp+=60;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_OUTLINE1STYLE);
	cbo->addItem(L"LINE");
	cbo->addItem(L"Glow SET size > 5 A = 32");

	yp += (s32)(h*1.2f);
	xp = xs;

	env->addEditBox(L"255 0 0 0",core::rect<s32>(xp,yp,90,yp+h), true, win, MYGUI_OUTLINE1COLOR1);

	xp+=90;

	env->addEditBox(L"255 255 0 0",core::rect<s32>(xp,yp,180,yp+h), true, win, MYGUI_OUTLINE1COLOR2);

	yp += (s32)(h*1.2f);
	xp = xs;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_OUTLINE1SIZE);
	for (s32 i=0; i <= 30; ++i)
	{
		core::stringw s = "";
		s += i;
		cbo->addItem(s.c_str());
	}
	cbo->setSelected(4);

	xp+=60;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_OUTLINE1COLORMODE);
	cbo->addItem(L"Solid");
	cbo->addItem(L"Gradient Horizontal");
	cbo->addItem(L"Gradient FW Diagonal");
	cbo->addItem(L"Gradient BW Diagonal");
	cbo->addItem(L"Gradient Horizontal 2");

	yp += (s32)(h*1.5f);
	xp = xs;

	// TEXT outline 2 
	env->addCheckBox(false, core::rect<s32>(xp,yp,xp+80,yp+h),win, MYGUI_OUTLINE2, L"Outline 2");

	xp+=60;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_OUTLINE2STYLE);
	cbo->addItem(L"LINE");
	cbo->addItem(L"Glow SET size > 5 A = 32");
	cbo->setSelected(1);

	yp += (s32)(h*1.2f);
	xp = xs;

	env->addEditBox(L"64 255 255 0",core::rect<s32>(xp,yp,90,yp+h), true, win, MYGUI_OUTLINE2COLOR1);

	xp+=90;

	env->addEditBox(L"64 255 0 0",core::rect<s32>(xp,yp,180,yp+h), true, win, MYGUI_OUTLINE2COLOR2);

	yp += (s32)(h*1.2f);
	xp = xs;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_OUTLINE2SIZE);
	for (s32 i=0; i <= 30; ++i)
	{
		core::stringw s = "";
		s += i;
		cbo->addItem(s.c_str());
	}
	cbo->setSelected(6);

	xp+=60;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,180,yp+h),win, MYGUI_OUTLINE2COLORMODE);
	cbo->addItem(L"Solid");
	cbo->addItem(L"Gradient Horizontal");
	cbo->addItem(L"Gradient FW Diagonal");
	cbo->addItem(L"Gradient BW Diagonal");
	cbo->addItem(L"Gradient Horizontal 2");
	cbo->setSelected(1);

	yp += (s32)(h*1.2f);
	xp = xs;

	//
	//
	//
	yp += (s32)(h*0.5f);

	env->addStaticText(L"Max Width:", core::rect<s32>(xp,yp,50,yp+h),false,false, win);

	xp += 60;

	// texture widths
	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+70,yp+h),win, MYGUI_TEXWIDTH);
	for (s32 i=0; texturesizes[i] != 0; ++i)
		cbo->addItem( ((core::stringw(texturesizes[i])) + L" wide").c_str());

	cbo->setSelected(2);

	xp += 70;

	// Debug checkbox
	env->addCheckBox(false, core::rect<s32>(xp,yp,xp+130,yp+h),win, MYGUI_TESTMODE, L"TEST");

	xp=xs;
	yp += (s32)(h*1.2f);

	env->addStaticText(L"Max Height:", core::rect<s32>(xp,yp,60,yp+h),false,false, win);

	xp += 60;

	// texture height
	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+70,yp+h),win, MYGUI_TEXHEIGHT);
	for (s32 i=0; texturesizes[i] != 0; ++i)
		cbo->addItem( ((core::stringw(texturesizes[i])) + L" tall").c_str());

	cbo->setSelected(2);

	xp += 70;

	// Debug checkbox
	env->addCheckBox(true, core::rect<s32>(xp,yp,xp+130,yp+h),win, MYGUI_PREVIEW, L"Preview");
	// file name
	xp = xs;
	yp += (s32)(h*1.5f);
	env->addStaticText(L"Filename", core::rect<s32>(xp,yp,60,yp+h),false,false, win);
	xp += 60;
	env->addEditBox(L"myfont",core::rect<s32>(xp,yp,xp+110,yp+h), true, win, MYGUI_FILENAME);

	// file format
	xp = xs;
	yp += (s32)(h*1.2f);
	env->addStaticText(L"File Format", core::rect<s32>(xp,yp,60,yp+h),false,false, win);
	xp += 60;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+55,yp+h),win, MYGUI_FORMAT);
	for (s32 i=0; fileformats[i] != 0; ++i)
		cbo->addItem( core::stringw(fileformats[i]).c_str());
	for (s32 i=0; alphafileformats[i] != 0; ++i)
		cbo->addItem( core::stringw(alphafileformats[i]).c_str());

	xp += 55;

	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+55,yp+h),win, MYGUI_BITMAPSTYLEFORMAT);
	cbo->addItem(L"irrXML");
	cbo->addItem(L".dot.");

	xp = xs;
	yp += (s32)(h*1.5f);

	// texture height
	env->addStaticText(L"ZOOM", core::rect<s32>(xp,yp,60,yp+h),false,false, win);
	xp += 60;
	cbo = env->addComboBox( core::rect<s32>(xp,yp,xp+70,yp+h),win, MYGUI_ZOOM);
	for (s32 i=0; i < 20; ++i)
	{
		core::stringw s = "";
		s += (i+1)*50;
		s += "%";
		cbo->addItem(s.c_str());
	}
	cbo->setSelected(1);

	xp += 70;

	// Debug checkbox
	env->addCheckBox(false, core::rect<s32>(xp,yp,xp+130,yp+h),win, MYGUI_DEBUGINFO, L"debug");

	xp = xs;
	yp += (s32)(h*1.2f);

	// create button
	env->addButton( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_CREATE, L"Create");

	xp += 60;

	// save button
	env->addButton( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_SAVE, L"Save");

	xp += 60;

	// help button
	env->addButton( core::rect<s32>(xp,yp,xp+50,yp+h),win, MYGUI_HELPBUTTON, L"Help");

	// font image
	gui::IGUIImage *img = env->addImage(0, core::position2d<s32>(0,0), true,0, MYGUI_IMAGE);
	img->setRelativePosition(core::rect<s32>(0,20,800,600));

	// Status image
	img = env->addImage(0, core::position2d<s32>(0,0), true,0, MYGUI_IMAGE_STATUS);
	img->setRelativePosition(core::rect<s32>(0,20,800,600));
	img->setVisible(false);

	// font scrollbar
	IGUIScrollBar *scrl= env->addScrollBar(true,core::rect<s32>(0,0,800,20),0, MYGUI_CURRENTIMAGE);
	scrl->setMax(0);
	scrl->setSmallStep(1);

	yp += h*2;

//	win->setRelativePosition( core::rect<s32>(10,10,200,yp));
//	win->setRelativePosition(core::position2di(600-40,25));

	env->getRootGUIElement()->bringToFront(win);
	win->setRelativePosition( core::rect<s32>(10,10,200,yp));
	win->setRelativePosition(core::position2di(605,20));

	winwork->setRelativePosition(core::position2di(605-200-50+120,530));
	winwork->setVisible(false);
	winpreview->setRelativePosition(core::position2di(605-200+120-5,250));
	env->getRootGUIElement()->bringToFront(winpreview);
//	winpreview->setVisible(false);
}

int main()
{
//	IrrlichtDevice* device =createDevice(video::EDT_OPENGL, core::dimension2du(800, 600),32);
	IrrlichtDevice* device =createDevice(video::EDT_DIRECT3D9, core::dimension2du(800, 600));
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment *env = device->getGUIEnvironment();

	// create font tool
	CFontTool *fc = new CFontTool(device);
	CVectorFontTool *vc = 0;

	IEventReceiver *events = new MyEventReceiver(device,fc,vc);

	createGUI(device, fc);
	CharPreview(device, fc);

	device->setWindowCaption(L"THE VERY Advanced irrFontTool (ver 3.7~2b) by Nikolay Atanassov");

	while(device->run())
	{
		if (device->isWindowActive())
		{

			driver->beginScene(true, true, video::SColor(0,200,200,200));
			smgr->drawAll();
			env->drawAll();
			driver->endScene();
		}
		device->sleep( 20 );
	}

	// drop the font tool and resources
	fc->drop();

	device->drop();

	return 0;
}

