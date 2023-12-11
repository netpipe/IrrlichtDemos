#ifndef C_GUI_TEXTURED_SKIN_H
#define C_GUI_TEXTURED_SKIN_H


#include <irrlicht.h>


namespace irr {
namespace gui {


enum SKIN_TEXURE_COORD_ENUM
{
	ESTC_ENUM_INVALID = -1,
	ESTC_BUTTON_NORMAL_LEFT = 0,
	ESTC_BUTTON_NORMAL_MIDDLE,
	ESTC_BUTTON_NORMAL_RIGHT,
	ESTC_BUTTON_PRESSED_LEFT,
	ESTC_BUTTON_PRESSED_MIDDLE,
	ESTC_BUTTON_PRESSED_RIGHT,
	ESTC_CHECK_BOX_UNCHECKED,
	ESTC_CHECK_BOX_CHECKED,
	ESTC_MENU_PANE,
	ESTC_SUNKEN_PANE,
	ESTC_TAB_BODY,
	ESTC_TAB_BUTTON_ACTIVE,
	ESTC_TAB_BUTTON_INACTIVE,
	ESTC_TOOLBAR_LEFT,
	ESTC_TOOLBAR_MIDDLE,
	ESTC_TOOLBAR_RIGHT,
	ESTC_WINDOW_UPPER_LEFT_CORNER,
	ESTC_WINDOW_UPPER_RIGHT_CORNER,
	ESTC_WINDOW_LOWER_LEFT_CORNER,
	ESTC_WINDOW_LOWER_RIGHT_CORNER,
	ESTC_WINDOW_UPPER_EDGE,
	ESTC_WINDOW_LOWER_EDGE,
	ESTC_WINDOW_LEFT_EDGE,
	ESTC_WINDOW_RIGHT_EDGE,
	ESTC_WINDOW_INTERIOR,
	ESTC_WINDOW_TITLEBAR,
	ESTC_ENUM_COUNT
};


class CGUITexturedSkin : public irr::gui::IGUISkin
{
protected:
	// Name of this skin
	irr::core::stringc skinName;
	irr::core::stringc skinFilename;

	// Store pointer to required irrlicht classes
	irr::gui::IGUIEnvironment * pGuiEnv;
	irr::video::IVideoDriver * pVideo;
	irr::io::IFileSystem * pFileSystem;

	// The file name of the texture used
	irr::core::stringc skinTextureFilename;
	// Pointer to the texture that is used by the current skin
	irr::video::ITexture * pSkinTexture;
	// The texture coordinates for the various components of the skin
	//irr::core::array<irr::core::rect<irr::s32> > skinPartTypeRectArray;
	irr::core::rect<irr::s32> skinTexCoords[ESTC_ENUM_COUNT];

	// Whether to tile the texture for interior of the window, or to stretch it
	bool tileWindowInterior;

	// Font used by this skin
	// WARNING: This skin does not come with a default built-in font
	// Create and assign one to it before use.
	irr::gui::IGUIFont* fonts[EGDF_COUNT];
	irr::gui::IGUISpriteBank* SpriteBank;
	irr::u32 icons[EGDI_COUNT];

	// Array of skin-defined colors
	irr::video::SColor colors[EGDC_COUNT];
	// Array of skin-defined text
	irr::core::stringw texts[EGDT_COUNT];
	// Array of skin-defined sizes
	irr::s32 sizes[EGDS_COUNT];


	// This function will parse the skin definition xml file
	bool readSkinXml(const irr::c8 * guiSkinXmlFile);

	// Case insensitive compare of current xml node name to supplied name
	bool isNodeName(irr::io::IXMLReaderUTF8 * pXml, const irr::c8 * pNodeName);

	// Finds a sub-node of the current xml node that has a name specified in the
	// nodeList array. currNodeDepth is the number of sublevels from the starting
	// node, used for repeatedly calling this function to parse an entire set of
	// nodes
	// Returns the nodeList array index of the next node name that is found
	irr::s32 findInterestedSubNode(irr::io::IXMLReaderUTF8 * pXml,
								   irr::core::array<irr::c8*> nodeList,
								   irr::s32 & currNodeDepth);

	// For converting a string value of the current node from x1,y1,x2,y2 format
	// into a rect, that is returned
	irr::core::rect<irr::s32> decodeImageCoord(irr::io::IXMLReaderUTF8 * pXml);

public:
	CGUITexturedSkin(IGUIEnvironment * pEnv, irr::io::IFileSystem * ifs);
	virtual ~CGUITexturedSkin();

	// Assign a new skin definition to this skin. Can be done during runtime to
	// change the skin.
	bool setSkin(const irr::c8 * guiSkinXmlFile);

	// Implementation of IGUISkin interface
	virtual void draw3DButtonPanePressed(irr::gui::IGUIElement*element,
										 const irr::core::rect<irr::s32> &rect,
										 const irr::core::rect<irr::s32> *clip=0);

	virtual void draw3DButtonPaneStandard(irr::gui::IGUIElement *element,
										  const irr::core::rect<irr::s32> &rect,
										  const irr::core::rect<irr::s32> *clip=0);

	virtual void draw3DMenuPane(irr::gui::IGUIElement *element,
								const irr::core::rect<irr::s32> &rect,
								const irr::core::rect<irr::s32> *clip=0);

	virtual void draw3DSunkenPane(irr::gui::IGUIElement *element,
								  irr::video::SColor bgcolor,
								  bool flat, bool fillBackGround,
								  const irr::core::rect<irr::s32> &rect,
								  const irr::core::rect<irr::s32> *clip=0);

	virtual void draw3DTabBody(irr::gui::IGUIElement *element, bool border, bool background,
							   const irr::core::rect<irr::s32> &rect,
							   const irr::core::rect<irr::s32> *clip=0,
							   s32 tabHeight=-1, EGUI_ALIGNMENT alignment=EGUIA_UPPERLEFT);

  virtual void draw3DTabButton(irr::gui::IGUIElement* element, bool active,
			const irr::core::rect<irr::s32>& rect,
			const irr::core::rect<irr::s32>* clip=0,
			EGUI_ALIGNMENT alignment=EGUIA_UPPERLEFT);

	virtual void draw3DToolBar(irr::gui::IGUIElement *element,
							   const irr::core::rect<irr::s32> &rect,
							   const irr::core::rect<irr::s32> *clip=0);

	virtual irr::core::rect<irr::s32> draw3DWindowBackground(irr::gui::IGUIElement *element,
															 bool drawTitleBar,
															 irr::video::SColor titleBarColor,
															 const irr::core::rect< irr::s32 > &rect,
															 const irr::core::rect< irr::s32 > *clip=0);

	virtual irr::video::SColor getColor(EGUI_DEFAULT_COLOR color) const;
	virtual void setColor(EGUI_DEFAULT_COLOR which, irr::video::SColor newColor);

	virtual irr::s32 getSize(EGUI_DEFAULT_SIZE size) const;
	virtual void setSize(EGUI_DEFAULT_SIZE which, irr::s32 size);

	virtual const wchar_t * getDefaultText(EGUI_DEFAULT_TEXT text) const;
	virtual void setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t *newText);

	virtual irr::gui::IGUIFont * getFont(EGUI_DEFAULT_FONT which=EGDF_DEFAULT) const;
	virtual void setFont(irr::gui::IGUIFont *font, EGUI_DEFAULT_FONT which);

	virtual irr::gui::IGUISpriteBank* getSpriteBank() const;
	virtual void setSpriteBank(irr::gui::IGUISpriteBank* bank);

	virtual u32 getIcon(EGUI_DEFAULT_ICON icon) const;
	virtual void setIcon(EGUI_DEFAULT_ICON icon, irr::u32 index);

	virtual void drawIcon(irr::gui::IGUIElement* element, EGUI_DEFAULT_ICON icon,
		const irr::core::position2di position, irr::u32 starttime=0, irr::u32 currenttime=0,
		bool loop=false, const irr::core::rect<s32>* clip=0);
	virtual void draw2DRectangle(irr::gui::IGUIElement* element, const irr::video::SColor &color,
		const irr::core::rect<s32>& pos, const irr::core::rect<s32>* clip = 0);

	virtual void serializeAttributes(irr::io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;
	virtual void deserializeAttributes(irr::io::IAttributes* in, io::SAttributeReadWriteOptions* options);
};


}	// end namespace gui
}	// end namespace irr


#endif
