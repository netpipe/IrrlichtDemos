
#include "CGUIPanel.h"
#include "CGUIEditorControl.h"
#include "IGUIFont.h"
#include "IGUIImage.h"
#include "IGUIButton.h"
#include "IGUIComboBox.h"
#include "IGUIEditBox.h"
#include "IGUICheckBox.h"
#include "IGUITreeCtrl.h"
#include "IGUITabControl.h"
#include "IGUIStaticText.h"
#include "IGUIElementFactory.h"
#include "IGUIFileOpenDialog.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IFileSystem.h"

using namespace irr;
using namespace gui;
using namespace core;

#include <sstream>
#include <cwchar>
using namespace std;

CGUIEditorControl*	CGUIEditorControl::Singleton = NULL;

rect<s32> ParseRectInput(const wchar_t* input)
{
	rect<s32> rc;
	s32 start, end = 0;
	stringw str = input;

	end = str.findNext(L',', 0);
//	rc.UpperLeftCorner.X = _wtoi(str.subString(0, end).c_str());
	start = end + 1;
	end = str.findNext(L',', start);
//	rc.UpperLeftCorner.Y = _wtoi(str.subString(start, end).c_str());
	start = end + 1;
	end = str.findNext(L',', start);
//	rc.LowerRightCorner.X = _wtoi(str.subString(start, end).c_str());
	start = end + 1;
//	rc.LowerRightCorner.Y = _wtoi(str.subString(start, str.size()).c_str());

	return rc;
}

stringw PrintRectOutput(const rect<s32> &rc)
{
	stringw str;

	str += rc.UpperLeftCorner.X;
	str += ", ";
	str += rc.UpperLeftCorner.Y;
	str += ", ";
	str += rc.LowerRightCorner.X;
	str += ", ";
	str += rc.LowerRightCorner.Y;

	return str;
}

CGUIEditorControl::CGUIEditorControl(IGUIEnvironment* environment, core::rect<s32> rectangle, IGUIElement* parent, s32 id)
: IGUIWindow(environment, (parent ? parent : environment->getRootGUIElement()), id, rectangle), Dragging(false), Skin(0)
{
	#ifdef _DEBUG
	setDebugName("CGUIWindow");
	#endif

	IGUISkin* skin = 0;
	//if (environment)
	//	skin = environment->getSkin();

	IGUISpriteBank* sprites = 0;
	video::SColor color(255,255,255,255);

	s32 buttonw = 15;
	if (skin)
	{
		buttonw = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
		sprites = skin->getSpriteBank();
		color = skin->getColor(EGDC_WINDOW_SYMBOL);
	}
	s32 posx = RelativeRect.getWidth() - buttonw - 4;

	CloseButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close" );
	CloseButton->setSubElement(true);
	CloseButton->setTabStop(false);
	CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	if (sprites)
	{
		CloseButton->setSpriteBank(sprites);
		CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), color);
		CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), color);
	}
	posx -= buttonw + 2;

	RestoreButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_RESTORE) : L"Restore" );
	RestoreButton->setVisible(false);
	RestoreButton->setSubElement(true);
	RestoreButton->setTabStop(false);
	RestoreButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	if (sprites)
	{
		RestoreButton->setSpriteBank(sprites);
		RestoreButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_RESTORE), color);
		RestoreButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_RESTORE), color);
	}
	posx -= buttonw + 2;

	MinButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_MINIMIZE) : L"Minimize" );
	MinButton->setVisible(false);
	MinButton->setSubElement(true);
	MinButton->setTabStop(false);
	MinButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	if (sprites)
	{
		MinButton->setSpriteBank(sprites);
		MinButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_MINIMIZE), color);
		MinButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_MINIMIZE), color);
	}

	MinButton->grab();
	RestoreButton->grab();
	CloseButton->grab();

	// this element is a tab group
	setTabGroup(true);
	setTabStop(true);
	setTabOrder(-1);

	stringw					str_resolution;
	rect<s32>				tab2rect(4, 40, rectangle.getWidth() - 12, rectangle.getHeight() - 72);
	rect<s32>				tab3rect(4, 4, rectangle.getWidth() - 12, rectangle.getHeight() - 72);
	rect<s32>				textrect(10, 10, 150, 30);
	rect<s32>				tabctrlrect(4, 24, rectangle.getWidth() - 4, rectangle.getHeight() - 4);
	rect<s32>				checkrect(10, 10, 30, 30);
	rect<s32>				buttonrect(10, 10, 110, 30);
	dimension2d<s32>		resolution(environment->getVideoDriver()->getScreenSize());

	str_resolution += resolution.Width;
	str_resolution += L",";
	str_resolution += resolution.Height;

	IGUITabControl* tabs	= environment->addTabControl(tabctrlrect, this);
	IGUITab* tab1			= tabs->addTab(L"general");
	IGUITab* tab2			= tabs->addTab(L"navi");
	IGUITab* tab3			= tabs->addTab(L"settings");

	//Tab 1
	environment->addButton(buttonrect + position2d<s32>(0, 0), tab1, 10101, L"Load GUI");
	environment->addButton(buttonrect + position2d<s32>(0, 30), tab1, 10102, L"Save GUI");
	environment->addEditBox(str_resolution.c_str(), textrect + position2d<s32>(0, 60), true, tab1, 10103);
	environment->addButton(buttonrect + position2d<s32>(0, 90), tab1, 10104, L"Scal GUI", L"newpos = (oldpos * actresolution / givenresolution)");

	//Tab 2
	TreeView			= AddGUITreeCtrl(environment, tab2rect, tab2, 10201);
	IGUIComboBox* box	= environment->addComboBox(buttonrect + position2d<s32>(0, 0), tab2, 10202);
	s32 count			= environment->getDefaultGUIElementFactory()->getCreatableGUIElementTypeCount();

	for(s32 counter = 0; counter < count; counter++)
		box->addItem(stringw(environment->getDefaultGUIElementFactory()->getCreateableGUIElementTypeName(counter)).c_str());

	environment->addButton(buttonrect + position2d<s32>(buttonrect.getWidth() + 10, 0), tab2, 10203, L"AddElement", L"adds the selected elementtype to the selected element as child");
	environment->addButton(buttonrect + position2d<s32>(buttonrect.getWidth() * 2 + 20, 0), tab2, 10204, L"delete element");

	//Tab 3
	CGUIPanel* panel = new CGUIPanel(environment, tab3, -1, tab3rect);

	environment->addStaticText(L"ID", textrect + position2d<s32>(0, 0), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 30), true, panel, 10301);
	environment->addStaticText(L"Text", textrect + position2d<s32>(0, 60), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 90), true, panel, 10302);
	environment->addStaticText(L"Rect", textrect + position2d<s32>(0, 120), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 180), true, panel, 10303);
	environment->addStaticText(L"Visible", textrect + position2d<s32>(0, 210), false, true, panel);
	environment->addCheckBox(false, checkrect + position2d<s32>(0, 240), panel, 10304);
	environment->addStaticText(L"Enabled", textrect + position2d<s32>(0, 270), false, true, panel);
	environment->addCheckBox(false, checkrect + position2d<s32>(0, 300), panel, 10305);
	environment->addStaticText(L"Border", textrect + position2d<s32>(0, 330), false, true, panel);
	environment->addCheckBox(false, checkrect + position2d<s32>(0, 360), panel, 10306);

	environment->addStaticText(L"Image", textrect + position2d<s32>(0, 390), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 420), true, panel, 10307);
	environment->addButton(buttonrect + position2d<s32>(textrect.getWidth() + 10, 420), panel, 10308, L"...");
	environment->addStaticText(L"ImageRect", textrect + position2d<s32>(0, 450), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 480), true, panel, 10309);

	environment->addStaticText(L"PressedImage", textrect + position2d<s32>(0, 510), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 540), true, panel, 10310);
	environment->addButton(buttonrect + position2d<s32>(textrect.getWidth() + 10, 540), panel, 10311, L"...");
	environment->addStaticText(L"PressedImageRect", textrect + position2d<s32>(0, 570), false, true, panel);
	environment->addEditBox(L"", textrect + position2d<s32>(0, 600), true, panel, 10312);

	UpdateTreeView();
	panel->updateAbsolutePosition();

	drop();
}


CGUIEditorControl::~CGUIEditorControl()
{
	if(Skin)
		Skin->drop();

	if(Singleton == this)
		Singleton = NULL;

	if (MinButton)
		MinButton->drop();

	if (RestoreButton)
		RestoreButton->drop();

	if (CloseButton)
		CloseButton->drop();
}

bool CGUIEditorControl::OnEvent(const SEvent& event)
{
	if(IsEnabled)
	{
		switch(event.EventType)
		{
			case EET_GUI_EVENT:
			{
				if(event.GUIEvent.Caller == TreeView && event.GUIEvent.EventType >= EGET_TREEVIEW_CHANGED_SELECTION)
				{
					TreeNode *node	= TreeView->FindNode(event.GUIEvent.EventType - EGET_TREEVIEW_CHANGED_SELECTION);
					LoadFromSelectedElement();
				}
				else switch(event.GUIEvent.EventType)
				{
					case EGET_BUTTON_CLICKED:
					{
						if (event.GUIEvent.Caller == CloseButton)
						{
							if (Parent)
							{
								// send close event to parent
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_ELEMENT_CLOSED;

								// if the event was not absorbed
								if (!Parent->OnEvent(e))
									remove();

								return true;

							}
							else
							{
								remove();
								return true;
							}
						}
						else switch(event.GUIEvent.Caller->getID())
						{
							case 10101:
							{
								IGUIFileOpenDialog* dia = Environment->addFileOpenDialog(L"Load GUI", false, this, 10901);
								dia->setNotClipped(true);
								dia->updateAbsolutePosition();
							}break;

							case 10102:
							{
								IGUIFileOpenDialog* dia = Environment->addFileOpenDialog(L"Save GUI", false, this, 10902);
								dia->setNotClipped(true);
								dia->updateAbsolutePosition();
							}break;

							case 10104:
							{
								dimension2d<s32> oldresolution;

								stringw str = static_cast<IGUIEditBox*>(getElementFromId(10103, true))->getText();
								s32 pos		= str.findFirst(L',');

								stringw width	= str.subString(0, pos);
								stringw height	= str.subString(pos, str.size());

//								oldresolution.Width		= _wtoi(width.c_str());
//								oldresolution.Height	= _wtoi(height.c_str());

								ScalGUI(oldresolution);
							}break;

							case 10203:
							{
								IGUIElement* el	= 0;
								s32 sel			= static_cast<IGUIComboBox*>(getElementFromId(10202, true))->getSelected();

								if(sel == -1)
									break;

								if(TreeView->GetSelected())
									el = static_cast<IGUIElement*>(TreeView->GetSelected()->Data);

								Environment->addGUIElement(Environment->getDefaultGUIElementFactory()->getCreateableGUIElementTypeName(sel), el);
								UpdateTreeView();
							}break;

							case 10308:
							{
								IGUIFileOpenDialog* dia = Environment->addFileOpenDialog(L"Load image", false, this, 10903);
								dia->setNotClipped(true);
								dia->updateAbsolutePosition();
							}break;

							case 10311:
							{
								IGUIFileOpenDialog* dia = Environment->addFileOpenDialog(L"Load pressedimage", false, this, 10904);
								dia->setNotClipped(true);
								dia->updateAbsolutePosition();
							}break;

							case 10204:
								{
									if(TreeView->GetSelected())
										static_cast<IGUIElement*>(TreeView->GetSelected()->Data)->remove();

									UpdateTreeView();
								}break;
						}
					}return true;

					case EGET_CHECKBOX_CHANGED:
					{
						switch(event.GUIEvent.Caller->getID())
						{
							case 10304:
							case 10305:
							case 10306:
								LoadToSelectedElement();
								LoadFromSelectedElement();
								break;
						}
					}return true;

					case EGET_FILE_SELECTED:
					{
						switch(event.GUIEvent.Caller->getID())
						{
							case 10901:
							{
								stringc filename = static_cast<IGUIFileOpenDialog*>(event.GUIEvent.Caller)->getFileName();

								grab();
								remove();
								Environment->clear();
								Environment->loadGUI(filename.c_str());
								Environment->getRootGUIElement()->addChild(this);
								drop();
								UpdateTreeView();
							}break;

							case 10902:
							{
								stringc filename = static_cast<IGUIFileOpenDialog*>(event.GUIEvent.Caller)->getFileName();

								grab();
								remove();
								Environment->saveGUI(filename.c_str());
								Environment->getRootGUIElement()->addChild(this);
								drop();
							}break;

							case 10903:
							{
								stringw filename = static_cast<IGUIFileOpenDialog*>(event.GUIEvent.Caller)->getFileName();

								static_cast<IGUIEditBox*>(getElementFromId(10307, true))->setText(filename.c_str());
								LoadToSelectedElement();
								LoadFromSelectedElement();
							}break;

							case 10904:
							{
								stringw filename = static_cast<IGUIFileOpenDialog*>(event.GUIEvent.Caller)->getFileName();

								static_cast<IGUIEditBox*>(getElementFromId(10309, true))->setText(filename.c_str());
								LoadToSelectedElement();
								LoadFromSelectedElement();
							}break;
						}
					}return true;

					case EGET_EDITBOX_ENTER:
					{
						switch(event.GUIEvent.Caller->getID())
						{
							case 10301:
							case 10302:
							case 10303:
							case 10307:
							case 10309:
							case 10310:
							case 10312:
								LoadToSelectedElement();
								LoadFromSelectedElement();
								break;
						}

					}return true;

					case EGET_ELEMENT_FOCUS_LOST:
					{
						Dragging = false;
					}break;

					case EGET_ELEMENT_FOCUSED:
					{
						if (Parent && ((event.GUIEvent.Caller == this) || isMyChild(event.GUIEvent.Caller)))
							Parent->bringToFront(this);
					}break;

					default:
						break;
				}
			}break;

			case EET_MOUSE_INPUT_EVENT:
			{
				switch(event.MouseInput.Event)
				{
					case EMIE_LMOUSE_PRESSED_DOWN:
						DragStart.X = event.MouseInput.X;
						DragStart.Y = event.MouseInput.Y;
						Dragging = true;
						if (Parent)
							Parent->bringToFront(this);
					return true;

					case EMIE_LMOUSE_LEFT_UP:
						Dragging = false;
					return true;

					case EMIE_MOUSE_MOVED:
						if (Dragging)
						{
							// gui window should not be dragged outside its parent
							if (Parent)
								if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
									event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
									event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
									event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1)

									return true;


							move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
							DragStart.X = event.MouseInput.X;
							DragStart.Y = event.MouseInput.Y;
							return true;
						}
					break;

					default:
						break;
				}
			}break;
		}
	}
	return IGUIElement::OnEvent(event);
}

void CGUIEditorControl::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* oldskin;
	IGUISkin* skin = Environment->getSkin();
	if(Skin)
	{
		oldskin	= skin;
		skin	= Skin;
		oldskin->grab();

		Environment->setSkin(skin);
	}

	core::rect<s32> rect = AbsoluteRect;
	core::rect<s32> *cl = &AbsoluteClippingRect;

	// draw body fast
	rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER),
		AbsoluteRect, &AbsoluteClippingRect);

	if (Text.size())
	{
		rect.UpperLeftCorner.X += skin->getSize(EGDS_TEXT_DISTANCE_X);
		rect.UpperLeftCorner.Y += skin->getSize(EGDS_TEXT_DISTANCE_Y);
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

		IGUIFont* font = skin->getFont(EGDF_WINDOW);
		if (font)
			font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true, cl);
	}

	IGUIElement::draw();

	if(Skin)
	{
		Environment->setSkin(oldskin);
		oldskin->drop();
	}
}

void CGUIEditorControl::UseBuiltInSkin(bool useit)
{
	if(Skin)	Skin->drop();
	if(useit)	Skin = Environment->createSkin(gui::EGST_WINDOWS_CLASSIC);
}

void CGUIEditorControl::ScalGUI(const core::dimension2d<s32> &oldresolution)
{
	core::dimension2d<s32> currentresolution = Environment->getVideoDriver()->getScreenSize();

	f32 x_factor = currentresolution.Width / (f32)oldresolution.Width;
	f32 y_factor = currentresolution.Height / (f32)oldresolution.Height;

	ScalElementsRecursive(x_factor, y_factor, Environment->getRootGUIElement());
}

void CGUIEditorControl::ScalElementsRecursive(irr::f32 x_factor, irr::f32 y_factor, IGUIElement *element)
{
	rect<s32> rc = getRelativePosition();

	rc.LowerRightCorner.X	= (s32)(rc.LowerRightCorner.X * x_factor + 0.5f);
	rc.LowerRightCorner.X	= (s32)(rc.LowerRightCorner.Y * y_factor + 0.5f);
	rc.UpperLeftCorner.X	= (s32)(rc.UpperLeftCorner.X * x_factor + 0.5f);
	rc.UpperLeftCorner.X	= (s32)(rc.UpperLeftCorner.Y * y_factor + 0.5f);

	setRelativePosition(rc);

	core::list<IGUIElement*> elements = element->getChildren();

	for(core::list<IGUIElement*>::Iterator it = elements.begin(); it != elements.end(); ++it)
		ScalElementsRecursive(x_factor, y_factor, *it);
}

void CGUIEditorControl::UpdateTreeView(void)
{
	TreeView->Clear();
	AddElementsRecursive(NULL, Environment->getRootGUIElement());
}

void CGUIEditorControl::AddElementsRecursive(TreeNode* treenode, IGUIElement* element)
{
	if(element == this)
		return;

	TreeNode* node = TreeView->InsertNode(GenNodeName(element).c_str(), treenode, 0, element);

	core::list<IGUIElement*> elements = element->getChildren();

	for(core::list<IGUIElement*>::Iterator it = elements.begin(); it != elements.end(); ++it)
		AddElementsRecursive(node, *it);
}

void CGUIEditorControl::LoadToSelectedElement(void)
{
	if(!TreeView->GetSelected())
		return;

	IGUIElement* SelectedElement = static_cast<IGUIElement*>(TreeView->GetSelected()->Data);

	if(SelectedElement == Environment->getRootGUIElement())
		return;


//	SelectedElement->setID(_wtoi(getElementFromId(10301, true)->getText()));
	SelectedElement->setText(getElementFromId(10302, true)->getText());
	SelectedElement->setRelativePosition(ParseRectInput(getElementFromId(10303, true)->getText()));
	SelectedElement->setVisible(static_cast<IGUICheckBox*>(getElementFromId(10304, true))->isChecked());
	SelectedElement->setEnabled(static_cast<IGUICheckBox*>(getElementFromId(10305, true))->isChecked());

	if(SelectedElement->getType() == EGUIET_BUTTON)
	{
		IGUIButton* ele = static_cast<IGUIButton*>(SelectedElement);

		video::ITexture* nimg = Environment->getVideoDriver()->getTexture(stringc(getElementFromId(10307, true)->getText()).c_str());
		video::ITexture* pimg = Environment->getVideoDriver()->getTexture(stringc(getElementFromId(10310, true)->getText()).c_str());

		ele->setDrawBorder(static_cast<IGUICheckBox*>(getElementFromId(10306, true))->isChecked());
		ele->setImage(nimg, ParseRectInput(getElementFromId(10309, true)->getText()));
		ele->setPressedImage(pimg, ParseRectInput(getElementFromId(10311, true)->getText()));
	}

	if(SelectedElement->getType() == EGUIET_EDIT_BOX)
		static_cast<IGUIEditBox*>(SelectedElement)->setDrawBorder(static_cast<IGUICheckBox*>(getElementFromId(10306, true))->isChecked());

	if(SelectedElement->getType() == EGUIET_STATIC_TEXT)
		static_cast<IGUIStaticText*>(SelectedElement)->setDrawBorder(static_cast<IGUICheckBox*>(getElementFromId(10306, true))->isChecked());

	if(SelectedElement->getType() == EGUIET_IMAGE)
	{
		IGUIImage* ele = static_cast<IGUIImage*>(SelectedElement);

		video::ITexture* nimg = Environment->getVideoDriver()->getTexture(stringc(getElementFromId(10307, true)->getText()).c_str());
		ele->setImage(nimg);
	}

	TreeView->GetSelected()->Text = GenNodeName(SelectedElement);
}

void CGUIEditorControl::LoadFromSelectedElement(void)
{
	if(!TreeView->GetSelected())
		return;

	IGUIElement* SelectedElement = static_cast<IGUIElement*>(TreeView->GetSelected()->Data);

	getElementFromId(10301, true)->setText(stringw(SelectedElement->getID()).c_str());
	getElementFromId(10302, true)->setText(stringw(SelectedElement->getText()).c_str());
	getElementFromId(10303, true)->setText(PrintRectOutput(SelectedElement->getRelativePosition()).c_str());

	static_cast<IGUICheckBox*>(getElementFromId(10304, true))->setChecked(SelectedElement->isVisible());
	static_cast<IGUICheckBox*>(getElementFromId(10305, true))->setChecked(SelectedElement->isEnabled());

	if(SelectedElement->getType() == EGUIET_BUTTON)
	{
		IGUIButton* ele = static_cast<IGUIButton*>(SelectedElement);

		io::IAttributes* out = Environment->getFileSystem()->createEmptyAttributes();
		ele->serializeAttributes(out);

		static_cast<IGUICheckBox*>(getElementFromId(10306, true))->setChecked(ele->isDrawingBorder());

		getElementFromId(10307, true)->setText(out->getAttributeAsStringW("Image").c_str());
		getElementFromId(10310, true)->setText(out->getAttributeAsStringW("PressedImage").c_str());

		getElementFromId(10309, true)->setText(out->getAttributeAsStringW("ImageRect").c_str());
		getElementFromId(10312, true)->setText(out->getAttributeAsStringW("PressedImageRect").c_str());
	}

	if(SelectedElement->getType() == EGUIET_EDIT_BOX)
		static_cast<IGUIEditBox*>(SelectedElement)->setDrawBorder(static_cast<IGUICheckBox*>(getElementFromId(10306, true))->isChecked());

	if(SelectedElement->getType() == EGUIET_STATIC_TEXT)
		static_cast<IGUIStaticText*>(SelectedElement)->setDrawBorder(static_cast<IGUICheckBox*>(getElementFromId(10306, true))->isChecked());

	if(SelectedElement->getType() == EGUIET_IMAGE)
	{
		IGUIImage* ele = static_cast<IGUIImage*>(SelectedElement);

		io::IAttributes* out = Environment->getFileSystem()->createEmptyAttributes();
		ele->serializeAttributes(out);

		getElementFromId(10307, true)->setText(out->getAttributeAsStringW("Image").c_str());

		getElementFromId(10309, true)->setText(out->getAttributeAsStringW("ImageRect").c_str());
	}
}

stringw CGUIEditorControl::GenNodeName(IGUIElement* element)
{
	stringw str;

	//workaround for bad ptr returned by getTypeName() for root-gui-node
	if(element != Environment->getRootGUIElement())
	{
		str += element->getText();
		str += " (";
		str += element->getTypeName();
		str += ", ";
		str += element->getID();
		str += ")";
	}
	else
		str += "RootNode";

	return str;
}
