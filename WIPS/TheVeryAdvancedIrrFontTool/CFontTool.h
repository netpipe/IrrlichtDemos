#ifndef __IRR_FONT_TOOL_INCLUDED__
#define __IRR_FONT_TOOL_INCLUDED__


#include "irrlicht.h"

#if defined(_IRR_WINDOWS_)
	#ifdef _MBCS
		#undef _MBCS
	#endif

	#define UNICODE
	#define _WIN32_WINNT 0x0500	
	#include "windows.h"

	#pragma comment(lib,"gdiplus.lib")
	#include <gdiplus.h>
	using namespace Gdiplus;

#else
	#ifdef _IRR_COMPILE_WITH_X11_
	#include <X11/Xlib.h>
	#endif
	#include <X11/Xft/Xft.h>
	#include <set>
#endif


namespace irr {
	class CFontTool : public irr::IReferenceCounted
	{
	public:
		CFontTool(irr::IrrlichtDevice* device);
		~CFontTool();

		virtual bool makeBitmapFont(s32 StyleFormat, u32 fontIndex, u32 charsetIndex, 
				s32 fontSize, u32 texturewidth, u32 textureHeight,
				bool bold, bool italic, bool aa, bool alpha,
				bool textfillmode,
				bool aaoutline, bool textoveroutline,
				u32 space_between_letters, 
				u32 bgColor, s32 textColorMode, u32 textColor1, u32 textColor2, 
				u32 outline1ColorMode, u32 outline1Color1, u32 outline1Color2, u32 outline1Style, u32 outline1Size,
				u32 outline2ColorMode, u32 outline2Color1, u32 outline2Color2, u32 outline2Style, u32 outline2Size,
				bool testMode, bool ShowDebugInfo
		);

		virtual bool makePreviewFont(u32 fontIndex, wchar_t PreviewChar, 
				s32 fontSize, u32 texturewidth, u32 textureHeight,
				bool bold, bool italic, bool aa, bool alpha,
				bool textfillmode,
				bool aaoutline, bool textoveroutline,
				u32 space_between_letters, 
				u32 bgColor, s32 textColorMode, u32 textColor1, u32 textColor2, 
				u32 outline1ColorMode, u32 outline1Color1, u32 outline1Color2, u32 outline1Style, u32 outline1Size,
				u32 outline2ColorMode, u32 outline2Color1, u32 outline2Color2, u32 outline2Style, u32 outline2Size,
				bool testmode
		);

		virtual bool saveBitmapFont(const c8* filename, const c8* format);

		virtual void selectCharSet(u32 currentCharSet);

		bool showProgress(core::stringw * Logmsg);

		void sortFonts();
		void freeFontMemory();
		struct SFontArea
		{
			SFontArea() : rectangle(), underhang(0), overhang(0), sourceimage(0) {}
			core::rect<s32> rectangle;
			s32		underhang;
			s32		overhang;
			u32		sourceimage;
		};

	/*	struct SFontMap
		{
			SFontMap() :  areas(), start(0), count(0) {}
			core::array< SFontArea >	areas;
			s32				start;
			s32				count;
		};*/



		core::array<core::stringw>	FontNames;
		core::array<core::stringw>	CharSets;
		//core::array<SFontMap>		Mappings;
		core::array<SFontArea>		Areas;
		core::map<wchar_t, u32>		CharMap;

		core::array<video::ITexture*>	currentTextures;
		core::array<video::IImage*>		currentImages;
		u32 previewTextureWidth;
		u32 previewTextureHeight;
		video::ITexture*	previewTexture;
		video::IImage*		previewImage;
		const int			*FontSizes;
		IrrlichtDevice			*Device;

		bool UseAlphaChannel;
		s32 BitmapFontStyleFormat;
		bool AbortIT;

//		video::IImage*		textImage;
//		video::ITexture*	textTexture;
		// windows
		#ifdef _IRR_WINDOWS_
		HDC dc;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;
		#endif

	};
}
#endif // __IRR_FONT_TOOL_INCLUDED__
