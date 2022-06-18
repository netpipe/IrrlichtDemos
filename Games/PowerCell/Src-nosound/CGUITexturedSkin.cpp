#include "CGUITexturedSkin.h"


using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace gui;


namespace irr {
namespace gui {


CGUITexturedSkin::CGUITexturedSkin(IGUIEnvironment * pEnv, irr::io::IFileSystem * ifs)
{
	pGuiEnv = pEnv;
	pGuiEnv->grab();
	pVideo = pEnv->getVideoDriver();
	pVideo->grab();
	pFileSystem = ifs;
	pFileSystem->grab();

	SpriteBank = 0;
	pSkinTexture = 0;

	IGUIFont* builtinfont = pEnv->getBuiltInFont();
	IGUIFontBitmap* bitfont = 0;
	if (builtinfont && builtinfont->getType() == EGFT_BITMAP)
		bitfont = (IGUIFontBitmap*)builtinfont;
	if (bitfont)
		setSpriteBank( bitfont->getSpriteBank() );

	for (u32 i=0; i<EGDF_COUNT; ++i)
		fonts[i] = 0;

	colors[EGDC_3D_DARK_SHADOW]   = video::SColor(101,50,50,50);
	colors[EGDC_3D_SHADOW]        = video::SColor(101,130,130,130);
	colors[EGDC_3D_FACE]          = video::SColor(101,210,210,210);
	colors[EGDC_3D_HIGH_LIGHT]    = video::SColor(101,255,255,255);
	colors[EGDC_3D_LIGHT]         =	video::SColor(101,210,210,210);
	colors[EGDC_ACTIVE_BORDER]    = video::SColor(101,16,14,115);
	colors[EGDC_ACTIVE_CAPTION]   = video::SColor(200,255,255,255);
	colors[EGDC_APP_WORKSPACE]    = video::SColor(101,100,100,100);
	colors[EGDC_BUTTON_TEXT]      = video::SColor(240,10,10,10);
	colors[EGDC_GRAY_TEXT]        = video::SColor(240,130,130,130);
	colors[EGDC_HIGH_LIGHT]       = video::SColor(101,8,36,107);
	colors[EGDC_HIGH_LIGHT_TEXT]  = video::SColor(240,255,255,255);
	colors[EGDC_INACTIVE_BORDER]  = video::SColor(101,165,165,165);
	colors[EGDC_INACTIVE_CAPTION] = video::SColor(101,210,210,210);
	colors[EGDC_TOOLTIP]          = video::SColor(200,0,0,0);
	colors[EGDC_TOOLTIP_BACKGROUND]= video::SColor(200,255,255,225);
	colors[EGDC_SCROLLBAR]        = video::SColor(101,230,230,230);
	colors[EGDC_WINDOW]           = video::SColor(101,255,255,255);
	colors[EGDC_WINDOW_SYMBOL]    = video::SColor(200,10,10,10);
	colors[EGDC_ICON]             = video::SColor(200,255,255,255);
	colors[EGDC_ICON_HIGH_LIGHT]  = video::SColor(200,8,36,107);

	sizes[EGDS_SCROLLBAR_SIZE] = 14;
	sizes[EGDS_MENU_HEIGHT] = 30;
	sizes[EGDS_WINDOW_BUTTON_WIDTH] = 15;
	sizes[EGDS_CHECK_BOX_WIDTH] = 18;
	sizes[EGDS_MESSAGE_BOX_WIDTH] = 500;
	sizes[EGDS_MESSAGE_BOX_HEIGHT] = 200;
	sizes[EGDS_BUTTON_WIDTH] = 80;
	sizes[EGDS_BUTTON_HEIGHT] = 30;
	sizes[EGDS_TEXT_DISTANCE_X] = 2;
	sizes[EGDS_TEXT_DISTANCE_Y] = 0;

	texts[EGDT_MSG_BOX_OK] = L"OK";
	texts[EGDT_MSG_BOX_CANCEL] = L"Cancel";
	texts[EGDT_MSG_BOX_YES] = L"Yes";
	texts[EGDT_MSG_BOX_NO] = L"No";
	texts[EGDT_WINDOW_CLOSE] = L"Close";
	texts[EGDT_WINDOW_RESTORE] = L"Restore";
	texts[EGDT_WINDOW_MINIMIZE] = L"Minimize";
	texts[EGDT_WINDOW_MAXIMIZE] = L"Maximize";

	icons[EGDI_WINDOW_MAXIMIZE] = 225;
	icons[EGDI_WINDOW_RESTORE] = 226;
	icons[EGDI_WINDOW_CLOSE] = 227;
	icons[EGDI_WINDOW_MINIMIZE] = 228;
	icons[EGDI_CURSOR_UP] = 229;
	icons[EGDI_CURSOR_DOWN] = 230;
	icons[EGDI_CURSOR_LEFT] = 231;
	icons[EGDI_CURSOR_RIGHT] = 232;
	icons[EGDI_MENU_MORE] = 232;
	icons[EGDI_CHECK_BOX_CHECKED] = 0;
	icons[EGDI_DROP_DOWN] = 234;
	icons[EGDI_SMALL_CURSOR_UP] = 235;
	icons[EGDI_SMALL_CURSOR_DOWN] = 236;
	icons[EGDI_RADIO_BUTTON_CHECKED] = 237;
	icons[EGDI_MORE_LEFT] = 238;
	icons[EGDI_MORE_RIGHT] = 239;
	icons[EGDI_MORE_UP] = 240;
	icons[EGDI_MORE_DOWN] = 241;
	icons[EGDI_WINDOW_RESIZE] = 242;
	icons[EGDI_EXPAND] = 243;
	icons[EGDI_COLLAPSE] = 244;
	icons[EGDI_FILE] = 245;
	icons[EGDI_DIRECTORY] = 246;
}


CGUITexturedSkin::~CGUITexturedSkin()
{
	for (u32 i=0; i<EGDF_COUNT; ++i)
	{
		if (fonts[i])
			fonts[i]->drop();
	}

	if (SpriteBank)
		SpriteBank->drop();

	if ( pSkinTexture )
	{
		pSkinTexture->drop();
		pSkinTexture = 0;
	}
	pGuiEnv->drop();
	pVideo->drop();
	pFileSystem->drop();
}


// Protected helper functions...


bool CGUITexturedSkin::readSkinXml( const c8 * guiSkinXmlFile )
{
	IXMLReaderUTF8 * pXml = pFileSystem->createXMLReaderUTF8( guiSkinXmlFile );
	if ( !pXml )
	{
		return false;
	}

	skinFilename = guiSkinXmlFile;

	while ( pXml->read() )
	{
		if ( EXN_ELEMENT == pXml->getNodeType() )
		{
			if ( isNodeName( pXml, "guiskin" ) )
			{
				skinName = pXml->getAttributeValue( "name" );
				skinTextureFilename = pXml->getAttributeValue( "texture" );
			}
			else if ( isNodeName( pXml, "buttonNormal" ) )	// Normal button
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "left" );
				nodeList.push_back( "middle" );
				nodeList.push_back( "right" );
				nodeList.push_back( "imageCoord" );
				s32 texCoordIndex = ESTC_ENUM_INVALID;
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
					      findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					switch ( findResult )
					{
					case 0:
						texCoordIndex = ESTC_BUTTON_NORMAL_LEFT;
						break;

					case 1:
						texCoordIndex = ESTC_BUTTON_NORMAL_MIDDLE;
						break;

					case 2:
						texCoordIndex = ESTC_BUTTON_NORMAL_RIGHT;
						break;

					case 3:
						if ( ESTC_ENUM_INVALID != texCoordIndex )
						{
							skinTexCoords[texCoordIndex] = decodeImageCoord( pXml );
						}
						break;
					}
				}
			}
			else if ( isNodeName( pXml, "buttonPressed" ) )		// Pressed button
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "left" );
				nodeList.push_back( "middle" );
				nodeList.push_back( "right" );
				nodeList.push_back( "imageCoord" );
				s32 texCoordIndex = ESTC_ENUM_INVALID;
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					switch ( findResult )
					{
					case 0:
						texCoordIndex = ESTC_BUTTON_PRESSED_LEFT;
						break;

					case 1:
						texCoordIndex = ESTC_BUTTON_PRESSED_MIDDLE;
						break;

					case 2:
						texCoordIndex = ESTC_BUTTON_PRESSED_RIGHT;
						break;

					case 3:
						if ( ESTC_ENUM_INVALID != texCoordIndex )
						{
							skinTexCoords[texCoordIndex] = decodeImageCoord( pXml );
						}
						break;
					}
				}
			}
			else if ( isNodeName( pXml, "checkBox" ) )		// Tab button
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "unchecked" );
				nodeList.push_back( "checked" );
				nodeList.push_back( "imageCoord" );
				s32 checkBoxType = ESTC_CHECK_BOX_UNCHECKED;

				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
					findResult != -1;
					findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					switch ( findResult )
					{
					case 0:
						checkBoxType = ESTC_CHECK_BOX_UNCHECKED;
						break;

					case 1:
						checkBoxType = ESTC_CHECK_BOX_CHECKED;
						break;

					case 2:
						skinTexCoords[checkBoxType] = decodeImageCoord( pXml );
						break;
					}
				}
			}
			else if ( isNodeName( pXml, "menuPane" ) )		// Menu pane
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "imageCoord" );
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					if ( 0 == findResult )
					{
						skinTexCoords[ESTC_MENU_PANE] = decodeImageCoord( pXml );
					}
				}
			}
			else if ( isNodeName( pXml, "sunkenPane" ) )	// Sunken pane
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "imageCoord" );
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					if ( 0 == findResult )
					{
						skinTexCoords[ESTC_SUNKEN_PANE] = decodeImageCoord( pXml );
					}
				}
			}
			else if ( isNodeName( pXml, "tabBody" ) )		// Tab body
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "imageCoord" );
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					if ( 0 == findResult )
					{
						skinTexCoords[ESTC_TAB_BODY] = decodeImageCoord( pXml );
					}
				}
			}
			else if ( isNodeName( pXml, "tabButton" ) )		// Tab button
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "active" );
				nodeList.push_back( "inactive" );
				nodeList.push_back( "imageCoord" );
				s32 buttonType = ESTC_TAB_BUTTON_ACTIVE;

				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					switch ( findResult )
					{
					case 0:
						buttonType = ESTC_TAB_BUTTON_ACTIVE;
						break;

					case 1:
						buttonType = ESTC_TAB_BUTTON_INACTIVE;
						break;

					case 2:
						skinTexCoords[buttonType] = decodeImageCoord( pXml );
						break;
					}
				}
			}
			else if ( isNodeName( pXml, "toolBar" ) )		// Toolbar
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "left" );
				nodeList.push_back( "middle" );
				nodeList.push_back( "right" );
				nodeList.push_back( "imageCoord" );
				s32 texCoordIndex = ESTC_ENUM_INVALID;
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					switch ( findResult )
					{
					case 0:
						texCoordIndex = ESTC_TOOLBAR_LEFT;
						break;

					case 1:
						texCoordIndex = ESTC_TOOLBAR_MIDDLE;
						break;

					case 2:
						texCoordIndex = ESTC_TOOLBAR_RIGHT;
						break;

					case 3:
						if ( ESTC_ENUM_INVALID != texCoordIndex )
						{
							skinTexCoords[texCoordIndex] = decodeImageCoord( pXml );
						}
						break;
					}
				}
			}
			else if ( isNodeName( pXml, "window" ) )		// Window
			{
				core::array< c8 * > nodeList;
				nodeList.push_back( "upperLeftCorner" );
				nodeList.push_back( "upperRightCorner" );
				nodeList.push_back( "lowerLeftCorner" );
				nodeList.push_back( "lowerRightCorner" );
				nodeList.push_back( "upperEdge" );
				nodeList.push_back( "lowerEdge" );
				nodeList.push_back( "leftEdge" );
				nodeList.push_back( "rightEdge" );
				nodeList.push_back( "interior" );
				nodeList.push_back( "titleBar" );
				nodeList.push_back( "imageCoord" );
				s32 texCoordIndex = ESTC_ENUM_INVALID;
				s32 nodeDepth = 0;

				for ( s32 findResult = findInterestedSubNode( pXml, nodeList, nodeDepth );
						  findResult != -1;
						  findResult = findInterestedSubNode( pXml, nodeList, nodeDepth ) )
				{
					switch ( findResult )
					{
					case 0:
						texCoordIndex = ESTC_WINDOW_UPPER_LEFT_CORNER;
						break;

					case 1:
						texCoordIndex = ESTC_WINDOW_UPPER_RIGHT_CORNER;
						break;

					case 2:
						texCoordIndex = ESTC_WINDOW_LOWER_LEFT_CORNER;
						break;

					case 3:
						texCoordIndex = ESTC_WINDOW_LOWER_RIGHT_CORNER;
						break;

					case 4:
						texCoordIndex = ESTC_WINDOW_UPPER_EDGE;
						break;

					case 5:
						texCoordIndex = ESTC_WINDOW_LOWER_EDGE;
						break;

					case 6:
						texCoordIndex = ESTC_WINDOW_LEFT_EDGE;
						break;

					case 7:
						texCoordIndex = ESTC_WINDOW_RIGHT_EDGE;
						break;

					case 8:
						texCoordIndex = ESTC_WINDOW_INTERIOR;
						tileWindowInterior = stringc( pXml->getAttributeValue( "tile" ) ).equals_ignore_case( stringc( "true" ) );
						break;

					case 9:
						texCoordIndex = ESTC_WINDOW_TITLEBAR;
						break;

					case 10:
						if ( ESTC_ENUM_INVALID != texCoordIndex )
						{
							skinTexCoords[texCoordIndex] = decodeImageCoord( pXml );
						}
						break;
					}
				}
			}
		}
	}	// end while pXml->read()

	// Drop the Xml reader to free it
	pXml->drop();

	return true;
}


bool CGUITexturedSkin::isNodeName( IXMLReaderUTF8 * pXml, const c8 * pNodeName )
{
	// do case insensitive string compare with current node name
	if ( stringc( pNodeName ).equals_ignore_case( stringc( pXml->getNodeName() ) ) )
	{
		return true;
	}
	return false;
}


s32 CGUITexturedSkin::findInterestedSubNode( IXMLReaderUTF8 * pXml, core::array< c8 * > nodeList, s32 & currNodeDepth )
{
	bool done = false;

	while ( pXml->read() && !done )
	{
		if ( EXN_ELEMENT == pXml->getNodeType() )
		{
			currNodeDepth++;
			for ( u32 i = 0; i < nodeList.size(); i++ )
			{
				if ( isNodeName( pXml, nodeList[i] ) )
				{
					return (s32)i;
				}
			}
		}
		else if ( EXN_ELEMENT_END == pXml->getNodeType() )
		{
			currNodeDepth--;
			if ( currNodeDepth < 0 )
			{
				done = true;
			}
		}
	}
	return -1;
}


core::rect<s32> CGUITexturedSkin::decodeImageCoord( IXMLReaderUTF8 * pXml )
{
	core::rect<s32> returnRect;
	const c8 * pCoordStr = 0;
	s32 index = 0;
	s32 tempValue = 0;
	// This stores number of coordinate values converted
	s32 numCoordObtained = 0;

	if ( pXml->read() && EXN_TEXT == pXml->getNodeType() )
	{
		pCoordStr = pXml->getNodeData();
	}
	else
	{
		return returnRect;
	}

	while ( pCoordStr[index] != 0 )
	{
		if ( pCoordStr[index] >= '0' && pCoordStr[index] <= '9' )
		{
			tempValue *= 10;
			tempValue += (s32) (pCoordStr[index] - '0');
		}
		else if ( pCoordStr[index] == ',' )
		{
			switch ( numCoordObtained )
			{
			case 0:
				returnRect.UpperLeftCorner.X = tempValue;
				numCoordObtained++;
				break;

			case 1:
				returnRect.UpperLeftCorner.Y = tempValue;
				numCoordObtained++;
				break;

			case 2:
				returnRect.LowerRightCorner.X = tempValue;
				numCoordObtained++;
				break;

			case 3:
				returnRect.LowerRightCorner.Y = tempValue;
				numCoordObtained++;
				break;
			}

			tempValue = 0;
		}

		if ( pCoordStr[index+1] == 0 )
		{
			// This will return the number when the end of string is reached
			returnRect.LowerRightCorner.Y = tempValue;
			numCoordObtained++;
		}

		index++;
	}

	return returnRect;
}


// Public function...


bool CGUITexturedSkin::setSkin( const c8 * guiSkinXmlFile )
{
	if ( !readSkinXml( guiSkinXmlFile ) )
	{
		// fail to load or parse xml file
		return false;
	}
	if ( pSkinTexture )
	{
		// release existing skin texture
		pSkinTexture->drop();
		pSkinTexture = 0;
	}
	pSkinTexture = pVideo->getTexture( skinTextureFilename.c_str() );
	if ( !pSkinTexture )
	{
		// fail to load texture
		return false;
	}
	pSkinTexture->grab();

	return true;
}


void CGUITexturedSkin::draw3DButtonPanePressed(IGUIElement*element, const core::rect<s32> &rect, const core::rect<s32> *clip)
{
	// Draw the left side
	core::rect<s32> buttonPressedLeft = skinTexCoords[ESTC_BUTTON_PRESSED_LEFT];
	f32 leftHtRatio = (f32)rect.getHeight() / buttonPressedLeft.getHeight();
	core::rect<s32> leftDestRect = rect;
	s32 leftWidth = (s32)(buttonPressedLeft.getWidth() * leftHtRatio);
	leftDestRect.LowerRightCorner.X = rect.UpperLeftCorner.X + (leftWidth<=rect.getWidth()?leftWidth:rect.getWidth());
	pVideo->draw2DImage(pSkinTexture,leftDestRect,buttonPressedLeft,0,0,true);

	// Draw the right side
	core::rect<s32> buttonPressedRight = skinTexCoords[ESTC_BUTTON_PRESSED_RIGHT];
	f32 rightHtRatio = (f32)rect.getHeight() / buttonPressedRight.getHeight();
	core::rect<s32> rightDestRect = rect;
	s32 rightWidth = (s32)(buttonPressedRight.getWidth() * rightHtRatio);
	rightDestRect.UpperLeftCorner.X = rect.LowerRightCorner.X - (rightWidth<=rect.getWidth()?rightWidth:rect.getWidth());
	pVideo->draw2DImage(pSkinTexture,rightDestRect,buttonPressedRight,0,0,true);

	// Draw the middle
	core::rect<s32> buttonPressedMiddle = skinTexCoords[ESTC_BUTTON_PRESSED_MIDDLE];
	core::rect<s32> middleDestRect = rect;
	middleDestRect.UpperLeftCorner.X = leftDestRect.LowerRightCorner.X;
	middleDestRect.LowerRightCorner.X = rightDestRect.UpperLeftCorner.X;
	pVideo->draw2DImage(pSkinTexture,middleDestRect,buttonPressedMiddle,0,0,true);
}


void CGUITexturedSkin::draw3DButtonPaneStandard(IGUIElement *element, const core::rect<s32> &rect, const core::rect<s32> *clip)
{
	// Draw the left side
	core::rect<s32> buttonNormalLeft = skinTexCoords[ESTC_BUTTON_NORMAL_LEFT];
	f32 leftHtRatio = (f32)rect.getHeight() / buttonNormalLeft.getHeight();
	core::rect<s32> leftDestRect = rect;
	s32 leftWidth = (s32)(buttonNormalLeft.getWidth() * leftHtRatio);
	leftDestRect.LowerRightCorner.X = rect.UpperLeftCorner.X + (leftWidth<=rect.getWidth()?leftWidth:rect.getWidth());
	pVideo->draw2DImage(pSkinTexture,leftDestRect,buttonNormalLeft,0,0,true);

	// Draw the right side
	core::rect<s32> buttonNormalRight = skinTexCoords[ESTC_BUTTON_NORMAL_RIGHT];
	f32 rightHtRatio = (f32)rect.getHeight() / buttonNormalRight.getHeight();
	core::rect<s32> rightDestRect = rect;
	s32 rightWidth = (s32)(buttonNormalRight.getWidth() * rightHtRatio);
	rightDestRect.UpperLeftCorner.X = rect.LowerRightCorner.X - (rightWidth<=rect.getWidth()?rightWidth:rect.getWidth());
	pVideo->draw2DImage(pSkinTexture,rightDestRect,buttonNormalRight,0,0,true);

	// Draw the middle
	core::rect<s32> buttonNormalMiddle = skinTexCoords[ESTC_BUTTON_NORMAL_MIDDLE];
	core::rect<s32> middleDestRect = rect;
	middleDestRect.UpperLeftCorner.X = leftDestRect.LowerRightCorner.X;
	middleDestRect.LowerRightCorner.X = rightDestRect.UpperLeftCorner.X;
	pVideo->draw2DImage(pSkinTexture,middleDestRect,buttonNormalMiddle,0,0,true);
}


void CGUITexturedSkin::draw3DMenuPane(IGUIElement *element, const core::rect<s32> &rect, const core::rect<s32> *clip)
{
	core::rect<s32> menuPane = skinTexCoords[ESTC_MENU_PANE];
	pVideo->draw2DImage(pSkinTexture,rect,menuPane,0,0,true);
}


void CGUITexturedSkin::draw3DSunkenPane(IGUIElement *element, video::SColor bgcolor, bool flat, bool fillBackGround, const core::rect<s32> &rect, const core::rect<s32> *clip)
{
	s32 type = ESTC_SUNKEN_PANE;
	if ( EGUIET_CHECK_BOX == element->getType() )
	{
		if ( ( (IGUICheckBox*)element)->isChecked() )
		{
			type = ESTC_CHECK_BOX_CHECKED;
		}
		else
		{
			type = ESTC_CHECK_BOX_UNCHECKED;
		}
	}
	core::rect<s32> sunkenPane = skinTexCoords[type];
	pVideo->draw2DImage(pSkinTexture,rect,sunkenPane,0,0,true);
}



void CGUITexturedSkin::draw3DTabBody(IGUIElement *element, bool border, bool background, const core::rect<s32> &rect, const core::rect<s32> *clip,s32 tabHeight, EGUI_ALIGNMENT alignment)

{

        core::rect<s32> tabBody = skinTexCoords[ESTC_TAB_BODY];

        core::rect<s32> destRect = rect;

        destRect.UpperLeftCorner.Y += getSize(EGDS_BUTTON_HEIGHT);

        pVideo->draw2DImage(pSkinTexture,destRect,tabBody,0,0,true);

}





void CGUITexturedSkin::draw3DTabButton(IGUIElement *element, bool active, const core::rect<s32> &rect, const core::rect<s32> *clip, EGUI_ALIGNMENT alignmentT)

{

        s32 buttonType = active ? ESTC_TAB_BUTTON_ACTIVE : ESTC_TAB_BUTTON_INACTIVE;

        core::rect<s32> tabButton = skinTexCoords[buttonType];

        pVideo->draw2DImage(pSkinTexture,rect,tabButton,0,0,true);

}

void CGUITexturedSkin::draw3DToolBar(IGUIElement *element, const core::rect<s32> &rect, const core::rect<s32> *clip)
{
	// Draw the left side
	core::rect<s32> toolBarLeft = skinTexCoords[ESTC_TOOLBAR_LEFT];
	f32 leftHtRatio = (f32)rect.getHeight() / toolBarLeft.getHeight();
	core::rect<s32> leftDestRect = rect;
	s32 leftWidth = (s32)(toolBarLeft.getWidth() * leftHtRatio);
	leftDestRect.LowerRightCorner.X = rect.UpperLeftCorner.X + (leftWidth<=rect.getWidth()?leftWidth:rect.getWidth());
	pVideo->draw2DImage(pSkinTexture,leftDestRect,toolBarLeft,0,0,true);

	// Draw the right side
	core::rect<s32> toolBarRight = skinTexCoords[ESTC_TOOLBAR_RIGHT];
	f32 rightHtRatio = (f32)rect.getHeight() / toolBarRight.getHeight();
	core::rect<s32> rightDestRect = rect;
	s32 rightWidth = (s32)(toolBarRight.getWidth() * rightHtRatio);
	rightDestRect.UpperLeftCorner.X = rect.LowerRightCorner.X - (rightWidth<=rect.getWidth()?rightWidth:rect.getWidth());
	pVideo->draw2DImage(pSkinTexture,rightDestRect,toolBarRight,0,0,true);

	// Draw the middle
	core::rect<s32> toolBarMiddle = skinTexCoords[ESTC_TOOLBAR_MIDDLE];
	core::rect<s32> middleDestRect = rect;
	middleDestRect.UpperLeftCorner.X = leftDestRect.LowerRightCorner.X;
	middleDestRect.LowerRightCorner.X = rightDestRect.UpperLeftCorner.X;
	pVideo->draw2DImage(pSkinTexture,middleDestRect,toolBarMiddle,0,0,true);
}


core::rect< s32 > CGUITexturedSkin::draw3DWindowBackground(IGUIElement *element, bool drawTitleBar, video::SColor titleBarColor, const core::rect< s32 > &rect, const core::rect< s32 > *clip,core::rect<s32>* checkClientArea)
{
	// Draw top left corner
	core::rect<s32> topLeftCorner = skinTexCoords[ESTC_WINDOW_UPPER_LEFT_CORNER];
	core::rect<s32> topLeftCornerDest = rect;
	topLeftCornerDest.LowerRightCorner.X = topLeftCornerDest.UpperLeftCorner.X + topLeftCorner.getWidth();
	topLeftCornerDest.LowerRightCorner.Y = topLeftCornerDest.UpperLeftCorner.Y + topLeftCorner.getHeight();
	pVideo->draw2DImage(pSkinTexture,topLeftCornerDest,topLeftCorner,0,0,true);

	// Draw top right corner
	core::rect<s32> topRightCorner = skinTexCoords[ESTC_WINDOW_UPPER_RIGHT_CORNER];
	core::rect<s32> topRightCornerDest = rect;
	topRightCornerDest.UpperLeftCorner.X = rect.LowerRightCorner.X - topRightCorner.getWidth();
	topRightCornerDest.LowerRightCorner.Y = rect.UpperLeftCorner.Y + topRightCorner.getHeight();
	pVideo->draw2DImage(pSkinTexture,topRightCornerDest,topRightCorner,0,0,true);

	// Draw bottom left corner
	core::rect<s32> bottomLeftCorner = skinTexCoords[ESTC_WINDOW_LOWER_LEFT_CORNER];
	core::rect<s32> bottomLeftCornerDest = rect;
	bottomLeftCornerDest.LowerRightCorner.X = rect.UpperLeftCorner.X + bottomLeftCorner.getWidth();
	bottomLeftCornerDest.UpperLeftCorner.Y = rect.LowerRightCorner.Y - bottomLeftCorner.getHeight();
	pVideo->draw2DImage(pSkinTexture,bottomLeftCornerDest,bottomLeftCorner,0,0,true);

	// Draw top right corner
	core::rect<s32> bottomRightCorner = skinTexCoords[ESTC_WINDOW_LOWER_RIGHT_CORNER];
	core::rect<s32> bottomRightCornerDest = rect;
	bottomRightCornerDest.UpperLeftCorner.X = rect.LowerRightCorner.X - bottomRightCorner.getWidth();
	bottomRightCornerDest.UpperLeftCorner.Y = rect.LowerRightCorner.Y - bottomRightCorner.getHeight();
	pVideo->draw2DImage(pSkinTexture,bottomRightCornerDest,bottomRightCorner,0,0,true);

	// Draw top edge
	core::rect<s32> topEdge = skinTexCoords[ESTC_WINDOW_UPPER_EDGE];
	core::rect<s32> topEdgeDest = rect;
	topEdgeDest.UpperLeftCorner.X = rect.UpperLeftCorner.X + topLeftCorner.getWidth();
	topEdgeDest.LowerRightCorner.X = rect.LowerRightCorner.X - topRightCorner.getWidth();
	topEdgeDest.LowerRightCorner.Y = rect.UpperLeftCorner.Y + topEdge.getHeight();
	pVideo->draw2DImage(pSkinTexture,topEdgeDest,topEdge,0,0,true);

	// Draw bottom edge
	core::rect<s32> bottomEdge = skinTexCoords[ESTC_WINDOW_LOWER_EDGE];
	core::rect<s32> bottomEdgeDest = rect;
	bottomEdgeDest.UpperLeftCorner.X = rect.UpperLeftCorner.X + bottomLeftCorner.getWidth();
	bottomEdgeDest.UpperLeftCorner.Y = rect.LowerRightCorner.Y - bottomEdge.getHeight();
	bottomEdgeDest.LowerRightCorner.X = rect.LowerRightCorner.X - bottomRightCorner.getWidth();
	pVideo->draw2DImage(pSkinTexture,bottomEdgeDest,bottomEdge,0,0,true);

	// Draw left edge
	core::rect<s32> leftEdge = skinTexCoords[ESTC_WINDOW_LEFT_EDGE];
	core::rect<s32> leftEdgeDest = rect;
	leftEdgeDest.UpperLeftCorner.Y = rect.UpperLeftCorner.Y + topLeftCorner.getHeight();
	leftEdgeDest.LowerRightCorner.X = rect.UpperLeftCorner.X + leftEdge.getWidth();
	leftEdgeDest.LowerRightCorner.Y = rect.LowerRightCorner.Y - bottomLeftCorner.getHeight();
	pVideo->draw2DImage(pSkinTexture,leftEdgeDest,leftEdge,0,0,true);

	// Draw right edge
	core::rect<s32> rightEdge = skinTexCoords[ESTC_WINDOW_RIGHT_EDGE];
	core::rect<s32> rightEdgeDest = rect;
	rightEdgeDest.UpperLeftCorner.X = rect.LowerRightCorner.X - rightEdge.getWidth();
	rightEdgeDest.UpperLeftCorner.Y = rect.UpperLeftCorner.Y + topRightCorner.getHeight();
	rightEdgeDest.LowerRightCorner.Y = rect.LowerRightCorner.Y - bottomRightCorner.getHeight();
	pVideo->draw2DImage(pSkinTexture,rightEdgeDest,rightEdge,0,0,true);

	// Draw interior
	core::rect<s32> interior = skinTexCoords[ESTC_WINDOW_INTERIOR];
	core::rect<s32> interiorDest = rect;
	interiorDest.UpperLeftCorner.X = rect.UpperLeftCorner.X + leftEdge.getWidth();
	interiorDest.UpperLeftCorner.Y = rect.UpperLeftCorner.Y + topEdge.getHeight();
	interiorDest.LowerRightCorner.X = rect.LowerRightCorner.X - rightEdge.getWidth();
	interiorDest.LowerRightCorner.Y = rect.LowerRightCorner.Y - bottomEdge.getHeight();
	pVideo->draw2DImage(pSkinTexture,interiorDest,interior,0,0,true);

	if (drawTitleBar)
	{
		// Draw title bar
		core::rect<s32> titleBar = skinTexCoords[ESTC_WINDOW_TITLEBAR];
		core::rect<s32> titleBarDest = rect;
		titleBarDest.UpperLeftCorner.X = rect.UpperLeftCorner.X + 3;
		titleBarDest.UpperLeftCorner.Y = rect.UpperLeftCorner.Y + 3;
		titleBarDest.LowerRightCorner.X = rect.UpperLeftCorner.X + titleBar.getWidth();
		titleBarDest.LowerRightCorner.Y = rect.UpperLeftCorner.Y + titleBar.getHeight();
		pVideo->draw2DImage(pSkinTexture,titleBarDest,titleBar,0,0,true);

		return titleBarDest;
	}

	return rect;
}


SColor CGUITexturedSkin::getColor(EGUI_DEFAULT_COLOR color) const
{
	if ((u32)color < EGDC_COUNT)
		return colors[color];
	else
		return video::SColor();
}


void CGUITexturedSkin::setColor(EGUI_DEFAULT_COLOR which, SColor newColor)
{
	if ((u32)which < EGDC_COUNT)
		colors[which] = newColor;
}


s32 CGUITexturedSkin::getSize(EGUI_DEFAULT_SIZE size) const
{
	if ((u32)size < EGDS_COUNT)
		return sizes[size];
	else
		return 0;
}


void CGUITexturedSkin::setSize(EGUI_DEFAULT_SIZE which, s32 size)
{
	if ((u32)which < EGDS_COUNT)
		sizes[which] = size;
}


const wchar_t * CGUITexturedSkin::getDefaultText(EGUI_DEFAULT_TEXT text) const
{
	if ((u32)text < EGDT_COUNT)
		return texts[text].c_str();
	else
		return texts[0].c_str();
}


void CGUITexturedSkin::setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t *newText)
{
	if ((u32)which < EGDT_COUNT)
		texts[which] = newText;
}


IGUIFont * CGUITexturedSkin::getFont(EGUI_DEFAULT_FONT which) const
{
	if (((u32)which < EGDS_COUNT) && fonts[which])
		return fonts[which];
	else
		return fonts[EGDF_DEFAULT];
}


void CGUITexturedSkin::setFont(IGUIFont *font, EGUI_DEFAULT_FONT which)
{
	if ((u32)which >= EGDS_COUNT)
		return;

	if (fonts[which])
		fonts[which]->drop();

	fonts[which] = font;

	if (fonts[which])
		fonts[which]->grab();
}


IGUISpriteBank* CGUITexturedSkin::getSpriteBank() const
{
	return SpriteBank;
}


void CGUITexturedSkin::setSpriteBank(IGUISpriteBank* bank)
{
	if (SpriteBank)
		SpriteBank->drop();

	if (bank)
		bank->grab();

	SpriteBank = bank;
}


u32 CGUITexturedSkin::getIcon(EGUI_DEFAULT_ICON icon) const
{
	if ((u32)icon < EGDI_COUNT)
		return icons[icon];
	else
		return 0;
}


void CGUITexturedSkin::setIcon(EGUI_DEFAULT_ICON icon, u32 index)
{
	if ((u32)icon < EGDI_COUNT)
		icons[icon] = index;
}


void CGUITexturedSkin::drawIcon(IGUIElement* element, EGUI_DEFAULT_ICON icon,
								const core::position2di position,
								u32 starttime, u32 currenttime,
								bool loop, const core::rect<s32>* clip)
{
	if (!SpriteBank)
		return;

	SpriteBank->draw2DSprite(icons[icon], position, clip,
		video::SColor(255,0,0,0), starttime, currenttime, loop, true);
}


void CGUITexturedSkin::draw2DRectangle(IGUIElement* element,
									   const video::SColor &color, const core::rect<s32>& pos,
									   const core::rect<s32>* clip)
{
	pVideo->draw2DRectangle(color, pos, clip);
}


void CGUITexturedSkin::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addString( "SkinFilename", skinFilename.c_str() );
}


void CGUITexturedSkin::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	irr::core::stringc file = in->getAttributeAsString( "SkinFilename" );
	setSkin( file.c_str() );
}


}	// end namespace gui
}	// end namespace irr
